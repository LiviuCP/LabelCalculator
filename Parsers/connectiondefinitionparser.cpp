#include <sstream>
#include <cassert>

#include "applicationutils.h"
#include "parserutils.h"
#include "connectiondefinitionparser.h"

ConnectionDefinitionParser::ConnectionDefinitionParser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream)
    : Parser{pInputStream, pOutputStream, pErrorStream, Data::c_ConnectionInputHeader}
{
    mMapping.resize(Data::c_MaxNrOfRackUnits, Data::DeviceTypeID::NO_DEVICE); // initial value: no device
}

// Maximum 50 lines to be read from connection definition file (the rack can have maximum 50U)
void ConnectionDefinitionParser::_readPayload()
{
    size_t connectionDefinitionRowsCount{0u};

    while (!mpInputStream->eof() && connectionDefinitionRowsCount < Data::c_MaxNrOfRackUnits)
    {
        ++connectionDefinitionRowsCount;
        mInputData.resize(connectionDefinitionRowsCount);
        getline(*mpInputStream, mInputData[connectionDefinitionRowsCount - 1]);
    }
}

bool ConnectionDefinitionParser::_parseInput()
{
    // lazy initialization of error handler
    _initializeErrorHandler();

    const size_t c_ConnectionDefinitionRowsCount{mInputData.size()};

    for (size_t rowIndex{0u}; rowIndex < c_ConnectionDefinitionRowsCount; ++rowIndex)
    {
        mFileColumnNumber = 1u;
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

    // if no parsing errors occurred, check whether this happened because there are actually no connected devices
    if (0u == mParsingErrors.size())
    {
        bool areDeviceConnectionsDefined{false};

        for (auto it{mConnectedTo.cbegin()}; it != mConnectedTo.cend(); ++it)
        {
            if (it->size() > 0u)
            {
                areDeviceConnectionsDefined = true;
                break;
            }
        }

        if (!areDeviceConnectionsDefined)
        {
            ErrorPtr pEmptyConnectionsInputFileError{mpErrorHandler->logError(ErrorCode::NO_CONNECTIONS_DEFINED, 1, 1, *mpErrorStream)};
            _storeParsingErrorAndLocation(pEmptyConnectionsInputFileError);
        }
    }

    const bool c_ErrorsOccurred{_logParsingErrorsToFile()};

    return c_ErrorsOccurred;
}

// Placeholders are included and need to be subsequently filled in by user before running the application with option 2 for getting the final labelling table
void ConnectionDefinitionParser::_buildOutput()
{
    const size_t c_DevicesCount{mUNumbers.size()};

    if (c_DevicesCount > 0u)
    {
        assert(c_DevicesCount == mConnectedTo.size() &&
               c_DevicesCount == mConnectionsCount.size());

        _buildTemplateDeviceParameters();

        size_t outputRowsCount{0u};

        // traverse the rack from top to bottom and check if each discovered device is connected to devices placed at upper U positions
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

            if (c_CurrentDeviceConnectedToLength > 0u)
            {
                for (auto connectedDevIter{mConnectedTo[c_CurrentDeviceIndex].cbegin()}; connectedDevIter != mConnectedTo[c_CurrentDeviceIndex].cend(); ++connectedDevIter)
                {
                    assert(*connectedDevIter > 0u); // U positions start from 1

                    const size_t c_ConnectedDeviceIndex{static_cast<size_t>(connectedDevIter - mConnectedTo[c_CurrentDeviceIndex].cbegin())};

                    /* The output string per connection row is calculated by adding following substrings: cable part number placeholder and the template parameters for each connected device
                       The decrease by 1 is necessary due to vector indexing (which starts at 0)
                    */
                    std::string output{Data::c_CablePartNumberPlaceholder +
                                       Data::c_CSVSeparator +
                                       mTemplateDeviceParameters[c_CurrentDeviceUPositionAsIndex] +
                                       Data::c_CSVSeparator +
                                       mTemplateDeviceParameters[*connectedDevIter - 1]};

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
}

void ConnectionDefinitionParser::_reset()
{
    mMapping.clear();
    mUNumbers.clear();
    mConnectedTo.clear();
    mConnectionsCount.clear();

    mMapping.resize(Data::c_MaxNrOfRackUnits, Data::DeviceTypeID::NO_DEVICE); // initial value: no device

    Parser::_reset();
}

void ConnectionDefinitionParser::_parseUPosition(const size_t rowIndex)
{
    assert(rowIndex < Data::c_MaxNrOfRackUnits);

    std::string currentCell;

    // first cell on the row is ignored (contains the U number and is only used for informing the user about rack position; the row index is instead used in calculations in relationship with U number)
    mCurrentPosition = Utilities::readDataField(mInputData[rowIndex], currentCell, 0);
    ++mFileColumnNumber;
}

bool ConnectionDefinitionParser::_parseDeviceType(const size_t rowIndex)
{
    assert(nullptr != mpErrorHandler.get());
    assert(rowIndex < Data::c_MaxNrOfRackUnits);

    bool isValidDeviceType{false};
    std::string currentCell;

    // second cell on the row: device type
    mCurrentPosition = Utilities::readDataField(mInputData[rowIndex], currentCell, mCurrentPosition);

    if (currentCell.size() > 0u)
    {
        Data::DeviceTypeID deviceTypeID{Utilities::getDeviceTypeID(currentCell)};

        if (Data::DeviceTypeID::UNKNOWN_DEVICE != deviceTypeID)
        {
            isValidDeviceType = true;

            // add discovered device to list of device U numbers
            mUNumbers.push_back(Data::c_MaxNrOfRackUnits - rowIndex);

            // add device type to mapping table
            mMapping[Data::c_MaxNrOfRackUnits - 1 - rowIndex] = deviceTypeID;

            // adjust vectors of connected devices and number of connections between each two devices
            mConnectedTo.push_back({});
            mConnectionsCount.push_back({});
        }
        else
        {
            ErrorPtr pError{mpErrorHandler->logError(ErrorCode::UNKNOWN_DEVICE, rowIndex + Utilities::c_RowNumberOffset, mFileColumnNumber, *mpErrorStream)};
            _storeParsingErrorAndLocation(pError);
        }

        ++mFileColumnNumber;
    }

    return isValidDeviceType;
}

void ConnectionDefinitionParser::_parseRowConnections(const size_t rowIndex)
{
    assert(nullptr != mpErrorHandler.get());

    const size_t c_DevicesCount{mUNumbers.size()};

    assert(c_DevicesCount > 0u &&
           c_DevicesCount == mConnectedTo.size() &&
           c_DevicesCount == mConnectionsCount.size());

    assert(rowIndex < Data::c_MaxNrOfRackUnits);

    const size_t c_FileRowNumber{rowIndex + Utilities::c_RowNumberOffset};

    while(mCurrentPosition > -1)
    {
        // read next cell (new current cell)
        std::string currentCell;
        mCurrentPosition = Utilities::readDataField(mInputData[rowIndex], currentCell, mCurrentPosition);

        if (0u == currentCell.size())
        {
            // If the device type contained on the row (second column) is valid then all the connections of the device should be entered contiguously starting with the third column
            if (mCurrentPosition > -1)
            {
                // unparsed cells on the row
                const std::string c_RemainingCells{mInputData[rowIndex].substr(static_cast<size_t>(mCurrentPosition))};

                if (Utilities::areParseableCharactersContained(c_RemainingCells))
                {
                    // trigger error but continue parsing the next cells from the row
                    ErrorPtr pEmptyCellError{mpErrorHandler->logError(ErrorCode::EMPTY_CELL, c_FileRowNumber, mFileColumnNumber, *mpErrorStream)};
                    _storeParsingErrorAndLocation(pEmptyCellError);
                    ++mFileColumnNumber;
                    continue;
                }
            }

            break;
        }

        Data::UNumber_t secondDevice;
        size_t connectionsCount;
        const bool c_IsConnectionFormattingInvalid{!_parseConnectionFormatting(currentCell, secondDevice, connectionsCount)};

        ErrorPtr pError{nullptr};

        if(c_IsConnectionFormattingInvalid) // checking if the connection format is correct (e.g. 20/3: 3 connections to device located at U20)
        {
            pError = mpErrorHandler->logError(ErrorCode::WRONG_CONNECTION_FORMAT, c_FileRowNumber, mFileColumnNumber, *mpErrorStream);
        }
        else if (secondDevice <= 0 || secondDevice > Data::c_MaxNrOfRackUnits) // checking if the device is in the accepted U interval within rack
        {

            pError = mpErrorHandler->logError(ErrorCode::DEVICE_U_POSITION_OUT_OF_RANGE, c_FileRowNumber, mFileColumnNumber, *mpErrorStream);
        }
        else if (Data::DeviceTypeID::NO_DEVICE == mMapping[secondDevice - 1]) // check if the second device is actually placed within rack (contained in mapping table)
        {
            pError = mpErrorHandler->logError(ErrorCode::TARGET_DEVICE_NOT_FOUND, c_FileRowNumber, mFileColumnNumber, *mpErrorStream);
        }
        else if (Data::c_MaxNrOfRackUnits - rowIndex == secondDevice) // connection of a device to itself (connection loop) is not allowed
        {
            pError = mpErrorHandler->logError(ErrorCode::DEVICE_CONNECTED_TO_ITSELF, c_FileRowNumber, mFileColumnNumber, *mpErrorStream);
        }
        else if (0 == connectionsCount) // if the devices are marked as connected there should be minimum 1 connection between them
        {
            pError = mpErrorHandler->logError(ErrorCode::NULL_NR_OF_CONNECTIONS, c_FileRowNumber, mFileColumnNumber, *mpErrorStream);
        }
        else
        {
            mConnectedTo[c_DevicesCount - 1].push_back(secondDevice); // add the U number of the second device to the list of connected devices (to current device)
            mConnectionsCount[c_DevicesCount - 1].push_back(connectionsCount); // add the number of connections between the current device and the second device
        }

        if (nullptr != pError)
        {
            _storeParsingErrorAndLocation(pError);
        }

        ++mFileColumnNumber;
    }
}

bool ConnectionDefinitionParser::_parseConnectionFormatting(const std::string& source, Data::UNumber_t& secondDevice, size_t& connectionsCount)
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

void ConnectionDefinitionParser::_buildTemplateDeviceParameters()
{
    mTemplateDeviceParameters.resize(Data::c_MaxNrOfRackUnits); // resize so it has same size as the mapping vector

    // uNumbers is traversed starting with the device placed at highest U position within rack
    for(auto deviceIter{mUNumbers.crbegin()}; deviceIter != mUNumbers.crend(); ++deviceIter)
    {
        assert(*deviceIter > 0u); // U positions start from 1

        std::stringstream stream;
        std::string currentDeviceUPosition;

        const size_t c_CurrentDeviceUPositionAsIndex{static_cast<size_t>(*deviceIter - 1)}; // in mapping vector numbering starts at 0 so it is necessary to decrease the U number by 1
        const std::string c_DeviceType = Utilities::getDeviceTypeAsString(mMapping[c_CurrentDeviceUPositionAsIndex]);

        assert(c_DeviceType.size() > 0u); // there should always be a non-empty string describing the device type

        mTemplateDeviceParameters[c_CurrentDeviceUPositionAsIndex] += c_DeviceType + Data::c_CSVSeparator; // appending device type
        stream << *deviceIter; // recover the original U number (real position in rack)
        stream >> currentDeviceUPosition;
        mTemplateDeviceParameters[c_CurrentDeviceUPositionAsIndex] += currentDeviceUPosition + Data::c_CSVSeparator;

        // add the placeholders for the device parameters (to be filled in the next step (option 2) in connectioninput.csv so the final table can be calculated)
        mTemplateDeviceParameters[c_CurrentDeviceUPositionAsIndex] += Data::scConnectionInputPlaceholders.at(mMapping[c_CurrentDeviceUPositionAsIndex]);
    }
}
