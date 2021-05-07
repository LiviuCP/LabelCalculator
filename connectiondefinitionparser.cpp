#include <sstream>
#include <cassert>

#include "errortypes.h"
#include "connectiondefinitionparser.h"

ConnectionDefinitionParser::ConnectionDefinitionParser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream)
    : Parser{pInputStream, pOutputStream, pErrorStream, c_InputHeader}
    , mDiscoveredDevicesCount{0}
{
    mMapping.resize(c_MaxNrOfRackUnits, DeviceTypeID::NO_DEVICE); // initial value: no device
}

// Maximum 50 lines to be read from connection definition file (the rack can have maximum 50U)
void ConnectionDefinitionParser::_readInput()
{
    std::string header;
    getline(*mpInputStream, header); // the header is not used further

    int connectionDefinitionRowsCount{0};

    while (!mpInputStream->eof() && connectionDefinitionRowsCount < c_MaxNrOfRackUnits)
    {
        ++connectionDefinitionRowsCount;
        mInputData.resize(connectionDefinitionRowsCount);
        getline(*mpInputStream, mInputData[connectionDefinitionRowsCount - 1]);
    }
}

bool ConnectionDefinitionParser::_parseInput()
{
    const int c_ConnectionDefinitionRowsCount{static_cast<int>(mInputData.size())};

    for (int rowIndex{0}; rowIndex < c_ConnectionDefinitionRowsCount; ++rowIndex)
    {
        mCurrentColumnNumber = 1;
        std::string currentCell;

        // first cell on the row is ignored (contains the U number and is only used for informing the user about rack position; the row index is instead used in calculations in relationship with U number)
        _parseUPosition(rowIndex);

        // second cell on the row: device type
        const bool c_IsValidDeviceType{_parseDeviceType(rowIndex)};

        // read each device to which current device is connected, go to next row if no (more) connected devices are found
        if (c_IsValidDeviceType)
        {
            _parseRowConnections(rowIndex);
        }
    }

    const bool c_ErrorsOccurred{_logParsingErrorsToFile()};

    return c_ErrorsOccurred;
}

// Placeholders are included and need to be subsequently filled in by user before running the application with option 2 for getting the final labelling table
void ConnectionDefinitionParser::_buildOutput()
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
        const int c_CurrentDeviceUPosition = mUNumbers[deviceIndex] - 1; // in mapping vector numbering starts at 0 so it is necessary to decrease the U number by 1
        const std::string c_DeviceType = getDeviceTypeAsString(mMapping[c_CurrentDeviceUPosition]);
        assert(c_DeviceType.size() > 0); // there should always be a non-empty string describing the device type
        deviceParameters[c_CurrentDeviceUPosition] += c_DeviceType + c_CSVSeparator; // appending device type
        str << c_CurrentDeviceUPosition + 1; // recover the original U number (real position in rack)
        str >> s;
        deviceParameters[c_CurrentDeviceUPosition] += s + c_CSVSeparator;
        deviceParameters[c_CurrentDeviceUPosition] += scConnectionInputPlaceholders.at(mMapping[c_CurrentDeviceUPosition]); // add the placeholders for the device parameters (to be filled in the next step (option 2) in connectioninput.csv so the final table can be calculated)
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
            mOutputData.resize(outputRowsCount);

            // write the resulting output string a number of times equal to the number of connections between the two devices
            for (int connectionNumber{outputRowsCount - mConnectionsCount[currentDeviceIndex][connectedDeviceIndex]}; connectionNumber < outputRowsCount; connectionNumber++)
            {
                mOutputData[connectionNumber] = output;
            }
        }
    }
}

void ConnectionDefinitionParser::_reset()
{
    mMapping.clear();
    mUNumbers.clear();
    mConnectedTo.clear();
    mConnectionsCount.clear();
    mDiscoveredDevicesCount = 0;

    mMapping.resize(c_MaxNrOfRackUnits, DeviceTypeID::NO_DEVICE); // initial value: no device

    Parser::_reset();
}

void ConnectionDefinitionParser::_parseUPosition(const int rowIndex)
{
    std::string currentCell;

    // first cell on the row is ignored (contains the U number and is only used for informing the user about rack position; the row index is instead used in calculations in relationship with U number)
    mCurrentPosition = readDataField(mInputData[rowIndex], currentCell, 0);
    ++mCurrentColumnNumber;
}

bool ConnectionDefinitionParser::_parseDeviceType(const int rowIndex)
{
    bool isValidDeviceType{false};
    std::string currentCell;

    // second cell on the row: device type
    mCurrentPosition = readDataField(mInputData[rowIndex], currentCell, mCurrentPosition);

    if (currentCell.size() > 0)
    {
        DeviceTypeID deviceTypeID{getDeviceTypeID(currentCell)};

        if (DeviceTypeID::UNKNOWN_DEVICE != deviceTypeID)
        {
            isValidDeviceType = true;

            // add discovered device to list of device U numbers
            ++mDiscoveredDevicesCount;
            mUNumbers.push_back(c_MaxNrOfRackUnits - rowIndex); // add the U number of the last discovered device

            // add device type to mapping table
            mMapping[c_MaxNrOfRackUnits - 1 - rowIndex] = deviceTypeID;

            // adjust vectors of connected devices and number of connections between each two devices
            mConnectedTo.resize(mDiscoveredDevicesCount);
            mConnectionsCount.resize(mDiscoveredDevicesCount);
        }
        else
        {
            ErrorPtr pError{std::make_shared<UnknownDeviceError>(*mpErrorStream)};

            _storeParsingErrorAndLocation(pError, rowIndex + c_RowNumberOffset, mCurrentColumnNumber);
        }

        ++mCurrentColumnNumber;
    }

    return isValidDeviceType;
}

void ConnectionDefinitionParser::_parseRowConnections(const int rowIndex)
{
    while(mCurrentPosition > -1)
    {
        // read next cell (new current cell)
        std::string currentCell;
        mCurrentPosition = readDataField(mInputData[rowIndex], currentCell, mCurrentPosition);

        if (0 == currentCell.size())
        {
            break;
        }

        int secondDevice;
        int nrOfconnections;
        const bool c_IsConnectionFormattingInvalid{!_parseConnectionFormatting(currentCell, secondDevice, nrOfconnections)};

        ErrorPtr pError{nullptr};

        if(c_IsConnectionFormattingInvalid) // checking if the connection format is correct (e.g. 20/3: 3 connections to device located at U20)
        {
            pError = std::make_shared<WrongFormatError>(*mpErrorStream);
        }
        else if (secondDevice <= 0 || secondDevice > c_MaxNrOfRackUnits) // checking if the device is in the accepted U interval within rack
        {
            pError = std::make_shared<WrongUNumberError>(*mpErrorStream);
        }
        else if (DeviceTypeID::NO_DEVICE == mMapping[secondDevice - 1]) // check if the second device is actually placed within rack (contained in mapping table)
        {
            pError = std::make_shared<InvalidTargetDevicePositionError>(*mpErrorStream);
        }
        else if (c_MaxNrOfRackUnits - rowIndex == secondDevice) // connection of a device to itself (connection loop) is not allowed
        {
            pError = std::make_shared<DeviceConnectedToItselfError>(*mpErrorStream);
        }
        else if (0 == nrOfconnections) // if the devices are marked as connected there should be minimum 1 connection between them
        {
            pError = std::make_shared<NoConnectionsError>(*mpErrorStream);
        }
        else
        {
            mConnectedTo[mDiscoveredDevicesCount - 1].resize(mCurrentColumnNumber - 2); // add the device to the list of connected devices (to current device)
            mConnectionsCount[mDiscoveredDevicesCount - 1].resize(mCurrentColumnNumber - 2); // same for the number of connections
            mConnectedTo[mDiscoveredDevicesCount - 1][mCurrentColumnNumber - 3] = secondDevice; // add the U number of the second device
            mConnectionsCount[mDiscoveredDevicesCount - 1][mCurrentColumnNumber - 3] = nrOfconnections; // add the number of connections between the current device and the second device
        }

        if (nullptr != pError)
        {
            _storeParsingErrorAndLocation(pError, rowIndex + c_RowNumberOffset, mCurrentColumnNumber);
        }

        ++mCurrentColumnNumber;
    }
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
        isFormattingValid = isFormattingValid && (slashIndex > 0 && slashIndex < sourceLength - 1);

        if (isFormattingValid)
        {
            secondDevice = stoi(source.substr(0, slashIndex));
            connectionsCount = stoi(source.substr(slashIndex + 1));
        }
    }
    else
    {
        isFormattingValid = false;
    }

    return isFormattingValid;
}
