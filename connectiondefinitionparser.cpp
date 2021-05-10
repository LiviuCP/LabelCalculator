#include <sstream>
#include <cassert>

#include "errortypes.h"
#include "connectiondefinitionparser.h"

ConnectionDefinitionParser::ConnectionDefinitionParser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream)
    : Parser{pInputStream, pOutputStream, pErrorStream, c_InputHeader}
{
    mMapping.resize(c_MaxNrOfRackUnits, DeviceTypeID::NO_DEVICE); // initial value: no device
}

// Maximum 50 lines to be read from connection definition file (the rack can have maximum 50U)
void ConnectionDefinitionParser::_readInput()
{
    std::string header;
    getline(*mpInputStream, header); // the header is not used further

    size_t connectionDefinitionRowsCount{0u};

    while (!mpInputStream->eof() && connectionDefinitionRowsCount < c_MaxNrOfRackUnits)
    {
        ++connectionDefinitionRowsCount;
        mInputData.resize(connectionDefinitionRowsCount);
        getline(*mpInputStream, mInputData[connectionDefinitionRowsCount - 1]);
    }
}

bool ConnectionDefinitionParser::_parseInput()
{
    const size_t c_ConnectionDefinitionRowsCount{mInputData.size()};

    for (size_t rowIndex{0u}; rowIndex < c_ConnectionDefinitionRowsCount; ++rowIndex)
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
    const size_t c_DevicesCount{mUNumbers.size()};

    if (c_DevicesCount > 0u)
    {
        assert(c_DevicesCount == mConnectedTo.size() &&
               c_DevicesCount == mConnectionsCount.size());

        vector<string> deviceParameters;
        deviceParameters.resize(c_MaxNrOfRackUnits); // resize so it has same size as the mapping vector

        // uNumbers is traversed starting with the device placed at highest U position within rack
        for(auto deviceIter{mUNumbers.crbegin()}; deviceIter != mUNumbers.crend(); ++deviceIter)
        {
            assert(*deviceIter > 0); // U positions start from 1

            stringstream stream;
            string currentDeviceUPosition;

            const size_t c_CurrentDeviceUPositionAsIndex{static_cast<size_t>(*deviceIter - 1)}; // in mapping vector numbering starts at 0 so it is necessary to decrease the U number by 1
            const string c_DeviceType = getDeviceTypeAsString(mMapping[c_CurrentDeviceUPositionAsIndex]);

            assert(c_DeviceType.size() > 0); // there should always be a non-empty string describing the device type

            deviceParameters[c_CurrentDeviceUPositionAsIndex] += c_DeviceType + c_CSVSeparator; // appending device type
            stream << *deviceIter; // recover the original U number (real position in rack)
            stream >> currentDeviceUPosition;
            deviceParameters[c_CurrentDeviceUPositionAsIndex] += currentDeviceUPosition + c_CSVSeparator;
            deviceParameters[c_CurrentDeviceUPositionAsIndex] += scConnectionInputPlaceholders.at(mMapping[c_CurrentDeviceUPositionAsIndex]); // add the placeholders for the device parameters (to be filled in the next step (option 2) in connectioninput.csv so the final table can be calculated)
        }

        size_t outputRowsCount{0u};

        // traverse the uNumbers vector again to check if each device is connected to devices placed in an upper U position
        for(auto deviceIter{mUNumbers.crbegin()}; deviceIter != mUNumbers.crend(); ++deviceIter)
        {
            const size_t c_CurrentDeviceIndex{static_cast<size_t>(mUNumbers.crend() - 1 - deviceIter)};
            const size_t c_CurrentDeviceUPositionAsIndex{static_cast<size_t>(*deviceIter - 1)};
            const size_t c_CurrentDeviceConnectedToLength{mConnectedTo[c_CurrentDeviceIndex].size()};

            if(c_CurrentDeviceConnectedToLength != mConnectionsCount[c_CurrentDeviceIndex].size())
            {
                fprintf(stderr, "mConnectedTo and mConnectionsCount have different sizes at index %d", static_cast<int>(c_CurrentDeviceIndex));
                assert(false);
            }

            // proceed to next device if current device is not connected to anything
            if (0u == c_CurrentDeviceConnectedToLength)
            {
                continue;
            }

            for (auto connectedDevIter{mConnectedTo[c_CurrentDeviceIndex].cbegin()}; connectedDevIter != mConnectedTo[c_CurrentDeviceIndex].cend(); ++connectedDevIter)
            {
                assert(*connectedDevIter > 0); // U positions start from 1

                const size_t c_ConnectedDeviceIndex{static_cast<size_t>(connectedDevIter - mConnectedTo[c_CurrentDeviceIndex].cbegin())};

                /* The output string is calculated by adding following substrings: cable part number placeholder and the device parameters substrings calculated in previous step
                   The decrease by 1 is necessary due to vector indexing (which starts at 0)
                */
                string output{c_CablePartNumberPlaceholder + "," + deviceParameters[c_CurrentDeviceUPositionAsIndex] + "," + deviceParameters[*connectedDevIter - 1]};

                outputRowsCount += mConnectionsCount[c_CurrentDeviceIndex][c_ConnectedDeviceIndex];
                mOutputData.resize(outputRowsCount);

                // write the resulting output string a number of times equal to the number of connections between the two devices
                for (size_t connectionNumber{outputRowsCount - mConnectionsCount[c_CurrentDeviceIndex][c_ConnectedDeviceIndex]}; connectionNumber < outputRowsCount; ++connectionNumber)
                {
                    mOutputData[connectionNumber] = output;
                }
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

    mMapping.resize(c_MaxNrOfRackUnits, DeviceTypeID::NO_DEVICE); // initial value: no device

    Parser::_reset();
}

void ConnectionDefinitionParser::_parseUPosition(const size_t rowIndex)
{
    assert(rowIndex < c_MaxNrOfRackUnits);

    std::string currentCell;

    // first cell on the row is ignored (contains the U number and is only used for informing the user about rack position; the row index is instead used in calculations in relationship with U number)
    mCurrentPosition = readDataField(mInputData[rowIndex], currentCell, 0);
    ++mCurrentColumnNumber;
}

bool ConnectionDefinitionParser::_parseDeviceType(const size_t rowIndex)
{
    assert(rowIndex < c_MaxNrOfRackUnits);

    bool isValidDeviceType{false};
    std::string currentCell;

    // second cell on the row: device type
    mCurrentPosition = readDataField(mInputData[rowIndex], currentCell, mCurrentPosition);

    if (currentCell.size() > 0u)
    {
        DeviceTypeID deviceTypeID{getDeviceTypeID(currentCell)};

        if (DeviceTypeID::UNKNOWN_DEVICE != deviceTypeID)
        {
            isValidDeviceType = true;

            // add discovered device to list of device U numbers
            mUNumbers.push_back(c_MaxNrOfRackUnits - rowIndex); // add the U number of the last discovered device

            // add device type to mapping table
            mMapping[c_MaxNrOfRackUnits - 1 - rowIndex] = deviceTypeID;

            // adjust vectors of connected devices and number of connections between each two devices
            mConnectedTo.push_back({});
            mConnectionsCount.push_back({});
        }
        else
        {
            ErrorPtr pError{std::make_shared<UnknownDeviceError>(*mpErrorStream)};

            _storeParsingErrorAndLocation(pError, static_cast<int>(rowIndex + c_RowNumberOffset), mCurrentColumnNumber); //TODO: row, column number to change to size_t in error functionality
        }

        ++mCurrentColumnNumber;
    }

    return isValidDeviceType;
}

void ConnectionDefinitionParser::_parseRowConnections(const size_t rowIndex)
{
    const size_t c_DevicesCount{mUNumbers.size()};

    assert(c_DevicesCount > 0u &&
           c_DevicesCount == mConnectedTo.size() &&
           c_DevicesCount == mConnectionsCount.size());

    assert(rowIndex < c_MaxNrOfRackUnits);

    while(mCurrentPosition > -1)
    {
        // read next cell (new current cell)
        std::string currentCell;
        mCurrentPosition = readDataField(mInputData[rowIndex], currentCell, mCurrentPosition);

        if (0 == currentCell.size())
        {
            break;
        }

        UNumber_t secondDevice;
        size_t connectionsCount;
        const bool c_IsConnectionFormattingInvalid{!_parseConnectionFormatting(currentCell, secondDevice, connectionsCount)};

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
        else if (0 == connectionsCount) // if the devices are marked as connected there should be minimum 1 connection between them
        {
            pError = std::make_shared<NoConnectionsError>(*mpErrorStream);
        }
        else
        {
            mConnectedTo[c_DevicesCount - 1].push_back(secondDevice); // add the U number of the second device to the list of connected devices (to current device)
            mConnectionsCount[c_DevicesCount - 1].push_back(connectionsCount); // add the number of connections between the current device and the second device
        }

        if (nullptr != pError)
        {
            _storeParsingErrorAndLocation(pError, static_cast<int>(rowIndex + c_RowNumberOffset), mCurrentColumnNumber); //TODO: row, column number to size_t in error
        }

        ++mCurrentColumnNumber;
    }
}

bool ConnectionDefinitionParser::_parseConnectionFormatting(const std::string& source, UNumber_t& secondDevice, size_t& connectionsCount)
{
    bool isFormattingValid{true};

    const int sourceLength{static_cast<int>(source.size())};
    int slashIndex{-1}; // index of '/'

    if (sourceLength > 0)
    {
        for (int index{0}; index < sourceLength; ++index)
        {
            if (isdigit(source[static_cast<size_t>(index)]))
            {
                continue;
            }
            else if ('/' == source[static_cast<size_t>(index)])
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
            secondDevice = static_cast<size_t>(stoi(source.substr(0, static_cast<size_t>(slashIndex))));
            connectionsCount = static_cast<size_t>(stoi(source.substr(static_cast<size_t>(slashIndex + 1))));
        }
    }
    else
    {
        isFormattingValid = false;
    }

    return isFormattingValid;
}
