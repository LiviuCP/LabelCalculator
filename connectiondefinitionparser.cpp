#include <sstream>
#include <cassert>

#include "devicefactory.h"
#include "error.h"
#include "labelutils.h"
#include "connectiondefinitionparser.h"

const std::map<std::string, std::string> ConnectionDefinitionParser::scConnectionInputPlaceholders
{
    {"_pdu", "PLACEMENT,LOAD SEGMENT NUMBER,PORT NUMBER"},
    {"_ext", "PLACEMENT,PORT NUMBER,-"                  },
    {"_ups", "LOAD SEGMENT NUMBER,PORT NUMBER,-"        },
    {"_ps" , "POWER SUPPLY NUMBER,-,-"                  },
    {"svr" , "PORT TYPE,PORT NUMBER,-"                  },
    {"bld" , "MODULE TYPE,MODULE NUMBER,PORT NUMBER"    },
    {"sto" , "CONTROLLER NUMBER,PORT NUMBER,-"          },
    {"san" , "PORT NUMBER,-,-"                          },
    {"lan" , "PORT NUMBER,-,-"                          },
    {"ib"  , "PORT NUMBER,-,-"                          },
    {"kvm" , "PORT NUMBER,-,-"                          }
};

ConnectionDefinitionParser::ConnectionDefinitionParser(std::ifstream& inputStream, std::ofstream& outputStream, std::ofstream& errorStream)
    : mInputStream{inputStream}
    , mOutputStream{outputStream}
    , mErrorStream{errorStream}
    , mDiscoveredDevicesCount{0}
    , mIsResetRequired{false}
{
    assert(mInputStream.is_open());
    assert(mOutputStream.is_open());
    assert(mErrorStream.is_open());

    mMapping.resize(c_MaxNrOfRackUnits, c_NoDevice); // initial value: no device
}

bool ConnectionDefinitionParser::parse()
{
    if (mIsResetRequired)
    {
        _reset(); // cleanup required after first usage
    }
    else
    {
        mIsResetRequired = true;
    }

    _readConnectionDefinitions();

    const bool c_ParsingErrorsOccurred{_parseConnectionDefinitions()};

    if (!c_ParsingErrorsOccurred)
    {
        _buildConnectionsInputTemplate();
        writeOutputToFile(mOutputStream, mConnectionInputRows, c_InputHeader);
    }

    return c_ParsingErrorsOccurred;
}

void ConnectionDefinitionParser::_readConnectionDefinitions()
{
    std::string header;
    getline(mInputStream, header); // the header is not used further

    int connectionDefinitionRowsCount{0};

    while (!mInputStream.eof() && connectionDefinitionRowsCount < c_MaxNrOfRackUnits)
    {
        ++connectionDefinitionRowsCount;
        mConnectionDefinitionRows.resize(connectionDefinitionRowsCount);
        getline(mInputStream, mConnectionDefinitionRows[connectionDefinitionRowsCount - 1]);
    }
}

bool ConnectionDefinitionParser::_parseConnectionDefinitions()
{
    std::vector<Error*> parsingErrors;

    const int c_ConnectionDefinitionRowsCount{static_cast<int>(mConnectionDefinitionRows.size())};

    for (int rowIndex{0}; rowIndex < c_ConnectionDefinitionRowsCount; ++rowIndex)
    {
        int columnNumber{1}; // csv column
        std::string currentCell; // current csv cell

        // first cell on the row is ignored (contains the U number and is only used for informing the user about rack position; the row index is instead used in calculations in relationship with U number)
        int currentPosition{readDataField(mConnectionDefinitionRows[rowIndex],currentCell,0)};

        // second cell on the row: device type
        currentPosition = readDataField(mConnectionDefinitionRows[rowIndex],currentCell,currentPosition);

        if (0 == currentCell.size())
        {
            continue;
        }

        ++columnNumber;

        if (!DeviceFactory::isDeviceTypeValid(currentCell))
        {
            Error* pError{new UnknownDeviceError{mErrorStream}};
            pError->setRow(rowIndex + 2); // setting row index of the current cell (+2 deoarece in Excel (.csv) the rows start at 1 and first line is ignored);
            pError->setColumn(columnNumber); // setting column index for exact error localization

            parsingErrors.push_back(pError);

            continue;
        }

        // add discovered device to list of device U numbers
        ++mDiscoveredDevicesCount;
        mUNumbers.push_back(c_MaxNrOfRackUnits - rowIndex); // add the U number of the last discovered device

        // add device type to mapping table
        mMapping[c_MaxNrOfRackUnits - 1 - rowIndex] = currentCell;

        // adjust vectors of connected devices and number of connections between each two devices
        mConnectedTo.resize(mDiscoveredDevicesCount);
        mConnectionsCount.resize(mDiscoveredDevicesCount);

        // read each device to which current device is connected, go to next row if no (more) connected devices are found
        while(currentPosition > -1)
        {
            // read next cell (new current cell)
            ++columnNumber;
            currentPosition = readDataField(mConnectionDefinitionRows[rowIndex],currentCell,currentPosition);

            if (0 == currentCell.size())
            {
                break;
            }

            int secondDevice;
            int nrOfconnections;
            const bool isConnectionFormattingInvalid{!_parseConnectionFormatting(currentCell, secondDevice, nrOfconnections)};

            Error* pError{nullptr};

            if(isConnectionFormattingInvalid) // checking if the connection format is correct (e.g. 20/3: 3 connections to device located at U20)
            {
                pError = new WrongFormatError{mErrorStream};
            }
            else if (secondDevice <= 0 || secondDevice > c_MaxNrOfRackUnits) // checking if the device is in the accepted U interval within rack
            {
                pError = new WrongUNumberError{mErrorStream};
            }
            else if (c_NoDevice == mMapping[secondDevice - 1]) // check if the second device is actually placed within rack (contained in mapping table)
            {
                pError = new NoDevicePresentError{mErrorStream};
            }
            else if (c_MaxNrOfRackUnits - rowIndex == secondDevice) // connection of a device to itself (connection loop) is not allowed
            {
                pError = new DeviceConnectedToItselfError{mErrorStream};
            }
            else if (0 == nrOfconnections) // if the devices are marked as connected there should be minimum 1 connection between them
            {
                pError = new NoConnectionsError{mErrorStream};
            }
            else
            {
                mConnectedTo[mDiscoveredDevicesCount - 1].resize(columnNumber - 2); // add the device to the list of connected devices (to current device)
                mConnectionsCount[mDiscoveredDevicesCount - 1].resize(columnNumber - 2); // same for the number of connections
                mConnectedTo[mDiscoveredDevicesCount - 1][columnNumber - 3] = secondDevice; // add the U number of the second device
                mConnectionsCount[mDiscoveredDevicesCount - 1][columnNumber - 3] = nrOfconnections; // add the number of connections between the current device and the second device
            }

            if (nullptr != pError)
            {
                pError->setRow(rowIndex + 2);
                pError->setColumn(columnNumber);
                parsingErrors.push_back(pError);
            }
        }
    }

    const bool c_ErrorsOccurred{parsingErrors.size() > 0};

    for (auto& error: parsingErrors)
    {
        error->execute();
        delete error;
        error = nullptr;
    }

    return c_ErrorsOccurred;
}

void ConnectionDefinitionParser::_buildConnectionsInputTemplate()
{
    using namespace std;

    /* Used for calculating the rows to be written into connectioninput.csv
       Each string represents the first device from each row, namely: device type, device name (lowest U occupied in rack), device parameters (e.g. port number)
    */
    vector<string> deviceParameters;

    deviceParameters.resize(c_MaxNrOfRackUnits); // resize so it has same size as the mapping vector

    // uNumbers is traversed starting with the device placed at highest U position within rack
    for (int deviceIndex{mDiscoveredDevicesCount - 1}; deviceIndex >= 0; --deviceIndex)
    {
        stringstream str;
        string s;
        int currentDeviceUPosition = mUNumbers[deviceIndex] - 1; // in mapping vector numbering starts at 0 so it is necessary to decrease the U number by 1
        deviceParameters[currentDeviceUPosition] += mMapping[currentDeviceUPosition]+','; // appending device type
        str << currentDeviceUPosition + 1; // recover the original U number (real position in rack)
        str >> s;
        deviceParameters[currentDeviceUPosition] += s + ',';
        deviceParameters[currentDeviceUPosition] += scConnectionInputPlaceholders.at(mMapping[currentDeviceUPosition]); // add the placeholders for the device parameters (to be filled in the next step (option 2) in connectioninput.csv so the final table can be calculated)
    }

    int outputRowsCount{0};

    // traverse the uNumbers vector again to check if each device is connected to devices placed in an upper U position
    for (int currentDeviceIndex{mDiscoveredDevicesCount - 1}; currentDeviceIndex >= 0; --currentDeviceIndex)
    {
        int currentDeviceUPosition{mUNumbers[currentDeviceIndex] - 1};
        int connectedToLength{static_cast<int>(mConnectedTo[currentDeviceIndex].size())};

        // proceed to next device if current device is not connected to anything
        if (0 == connectedToLength)
        {
            continue;
        }

        for (int connectedDeviceIndex{0}; connectedDeviceIndex < connectedToLength; ++connectedDeviceIndex)
        {
            /* The output string is calculated by adding following substrings: cable part number placeholder and the device parameters substrings calculated in previous step
               The decrease by 1 is necessary due to vector indexing (which starts at 0)
            */
            string output{c_CablePartNumberPlaceholder + "," + deviceParameters[currentDeviceUPosition] + "," + deviceParameters[mConnectedTo[currentDeviceIndex][connectedDeviceIndex] - 1]};
            outputRowsCount += mConnectionsCount[currentDeviceIndex][connectedDeviceIndex];
            mConnectionInputRows.resize(outputRowsCount);

            // write the resulting output string a number of times equal to the number of connections between the two devices
            for (int connectionNumber{outputRowsCount - mConnectionsCount[currentDeviceIndex][connectedDeviceIndex]}; connectionNumber < outputRowsCount; connectionNumber++)
            {
                mConnectionInputRows[connectionNumber] = output;
            }
        }
    }
}

void ConnectionDefinitionParser::_reset()
{
    mConnectionDefinitionRows.clear();
    mMapping.clear();
    mUNumbers.clear();
    mConnectedTo.clear();
    mConnectionsCount.clear();
    mConnectionInputRows.clear();
    mDiscoveredDevicesCount = 0;

    mMapping.resize(c_MaxNrOfRackUnits, c_NoDevice); // initial value: no device
}

bool ConnectionDefinitionParser::_parseConnectionFormatting(const std::string& source, int& secondDevice, int& connectionsCount)
{
    bool isFormattingValid{true};

    const int sourceLength{static_cast<int>(source.size())};
    int slashIndex{-1}; // index of '/'

    if (sourceLength > 0)
    {
        for (int index{0}; index < sourceLength; ++index)
        {
            if (isdigit(source[index]))
            {
                continue;
            }
            else if ('/' == source[index])
            {
                if (-1 != slashIndex) // more than one slash
                {
                    isFormattingValid = false;
                    break;
                }
                else
                {
                    slashIndex = index;
                    continue;
                }
            }
            else
            {
                isFormattingValid = false;
                break;
            }
        }

        // there should be exactly one slash character and it should not be located in the first/last string character position
        isFormattingValid = isFormattingValid && (slashIndex > 0 && slashIndex < sourceLength-1);

        if (isFormattingValid)
        {
            secondDevice = stoi(source.substr(0, slashIndex));
            connectionsCount = stoi(source.substr(slashIndex+1));
        }
    }
    else
    {
        isFormattingValid = false;
    }

    return isFormattingValid;
}
