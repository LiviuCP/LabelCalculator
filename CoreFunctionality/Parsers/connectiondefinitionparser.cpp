#include <sstream>
#include <charconv>
#include <cassert>

#include "errorcodes.h"
#include "parserdata.h"
#include "coreutils.h"
#include "parserutils.h"
#include "connectiondefinitionparser.h"

namespace Core = Utilities::Core;
namespace Parsers = Utilities::Parsers;

ConnectionDefinitionParser::ConnectionDefinitionParser(const InputStreamPtr pInputStream, const OutputStreamPtr pOutputStream, const ErrorStreamPtr pErrorStream)
    : Parser{pInputStream, pOutputStream, pErrorStream, Data::c_ConnectionInputHeader}
{
    mRackPositionToDeviceTypeMapping.resize(Data::c_MaxRackUnitsCount, Data::DeviceTypeID::NO_DEVICE); // initial value: no device
}

// Maximum 50 lines to be read from connection definition file (the rack can have maximum 50U)
void ConnectionDefinitionParser::_readPayload()
{
    size_t connectionDefinitionRowsCount{0u};

    while (!_isInputStreamConsumed() && connectionDefinitionRowsCount < Data::c_MaxRackUnitsCount)
    {
        const bool c_Success{_readLineAndAppendToInput()};

        if (c_Success)
        {
            ++connectionDefinitionRowsCount;
        }
    }
}

bool ConnectionDefinitionParser::_parseInput()
{
    const size_t c_ConnectionDefinitionRowsCount{_getInputRowsCount()};

    for (size_t rowIndex{0u}; rowIndex < c_ConnectionDefinitionRowsCount; ++rowIndex)
    {
        _moveToInputRowStart(rowIndex);
        std::string currentCell;

        // first cell on the row is ignored (contains the U number and is only used for informing the user about rack position; the row index is instead used in calculations in relationship with U number)
        _parseUPosition(rowIndex);

        // second cell on the row: device type
        const bool c_IsValidDeviceType{_parseDeviceType(rowIndex)};

        // read each device to which current device is connected, go to next row if no (more) connected devices are found
        if (c_IsValidDeviceType)
        {
            _parseDeviceConnections(rowIndex);
        }
    }

    // if no parsing errors occurred, check whether this happened because there are actually no connected devices
    if (!_parsingErrorsExist())
    {
        bool areDeviceConnectionsDefined{false};

        for (auto it{mConnections.cbegin()}; it != mConnections.cend(); ++it)
        {
            if (it->mConnectedDevices.size() > 0u)
            {
                areDeviceConnectionsDefined = true;
                break;
            }
        }

        if (!areDeviceConnectionsDefined)
        {
            ErrorPtr pEmptyConnectionsDefinitionsFileError{_logError(static_cast<Error_t>(ErrorCode::NO_CONNECTIONS_DEFINED), 1, true)};
            _storeParsingError(pEmptyConnectionsDefinitionsFileError);
        }
    }

    const bool c_ErrorsOccurred{_logParsingErrorsToFile()};

    return c_ErrorsOccurred;
}

// Placeholders are included and need to be subsequently filled in by user before running the application with option 2 for getting the final labelling table
void ConnectionDefinitionParser::_buildOutput()
{
    const size_t c_DevicesCount{mConnections.size()};

    if (c_DevicesCount > 0u)
    {
        _buildTemplateDeviceParameters();

        size_t outputRowsCount{0u};

        // traverse the rack from top to bottom and check if each discovered device is connected to devices placed at upper U positions
        for(auto deviceIter{mConnections.crbegin()}; deviceIter != mConnections.crend(); ++deviceIter)
        {
            assert(deviceIter->mSourceDevice > 0u); // U positions start from 1

            const size_t c_CurrentDeviceIndex{static_cast<size_t>(std::distance(deviceIter, mConnections.crend() - 1))};
            const size_t c_CurrentDeviceUPositionAsIndex{deviceIter->mSourceDevice - 1};
            const size_t c_CurrentDeviceConnectedDevicesCount{mConnections[c_CurrentDeviceIndex].mConnectedDevices.size()};

            if (c_CurrentDeviceConnectedDevicesCount > 0u)
            {
                for (auto connectedDevIter{mConnections[c_CurrentDeviceIndex].mConnectedDevices.cbegin()}; connectedDevIter != mConnections[c_CurrentDeviceIndex].mConnectedDevices.cend(); ++connectedDevIter)
                {
                    assert(connectedDevIter->first > 0u); // U positions start from 1

                    const size_t c_ConnectedDeviceIndex{static_cast<size_t>(std::distance(mConnections[c_CurrentDeviceIndex].mConnectedDevices.cbegin(), connectedDevIter))};

                    /* The output string per connection row is calculated by adding following substrings: cable part number placeholder and the template parameters for each connected device
                       The decrease by 1 is necessary due to vector indexing (which starts at 0)
                    */
                    std::string output{Data::c_CablePartNumberPlaceholder};
                    output.push_back(Data::c_CSVSeparator);
                    output.append(mTemplateDeviceParameters[c_CurrentDeviceUPositionAsIndex]);
                    output.push_back(Data::c_CSVSeparator);
                    output.append(mTemplateDeviceParameters[connectedDevIter->first - 1]);

                    outputRowsCount += mConnections[c_CurrentDeviceIndex].mConnectedDevices[c_ConnectedDeviceIndex].second;

                    // write the resulting output string a number of times equal to the number of connections between the two devices
                    for (size_t connectionNumber{outputRowsCount - mConnections[c_CurrentDeviceIndex].mConnectedDevices[c_ConnectedDeviceIndex].second}; connectionNumber < outputRowsCount; ++connectionNumber)
                    {
                        _appendRowToOutput(output);
                    }
                }
            }
        }
    }
}

void ConnectionDefinitionParser::_reset()
{
    mRackPositionToDeviceTypeMapping.clear();
    mConnections.clear();
    mTemplateDeviceParameters.clear();

    mRackPositionToDeviceTypeMapping.resize(Data::c_MaxRackUnitsCount, Data::DeviceTypeID::NO_DEVICE); // initial value: no device

    Parser::_reset();
}

void ConnectionDefinitionParser::_parseUPosition(const size_t rowIndex)
{
    if (rowIndex < Data::c_MaxRackUnitsCount)
    {
        std::string uPositionCell;

        // first cell on the row is ignored (contains the U number and is only used for informing the user about rack position; the row index is instead used in calculations in relationship with U number)
        if (const bool c_CellSuccessfullyRead{_readFirstCell(rowIndex, uPositionCell)}; c_CellSuccessfullyRead)
        {
            _moveToNextInputColumn(rowIndex);
        }
    }
}

bool ConnectionDefinitionParser::_parseDeviceType(const size_t rowIndex)
{
    bool isValidDeviceType{false};

    if (rowIndex < Data::c_MaxRackUnitsCount)
    {
        std::string currentCell;

        // second cell on the row: device type
        if (const bool c_CellSuccessfullyRead{_readCurrentCell(rowIndex, currentCell)};
            c_CellSuccessfullyRead && currentCell.size() > 0u)
        {
            const Data::DeviceTypeID c_DeviceTypeID{Parsers::getDeviceTypeID(currentCell)};

            if (Data::DeviceTypeID::UNKNOWN_DEVICE != c_DeviceTypeID)
            {
                isValidDeviceType = true;
                const Data::UNumber_t c_DeviceUPosition{Data::c_MaxRackUnitsCount - rowIndex};

                // add device type to mapping table
                mRackPositionToDeviceTypeMapping[c_DeviceUPosition - 1] = c_DeviceTypeID;

                // add discovered device to list of device U numbers and adjust vectors of connected devices and number of connections between each two devices
                mConnections.emplace_back(c_DeviceUPosition);
            }
            else
            {
                ErrorPtr pError{_logError(static_cast<Error_t>(ErrorCode::UNKNOWN_DEVICE), rowIndex + Parsers::c_RowNumberOffset)};
                _storeParsingError(pError);
            }

            _moveToNextInputColumn(rowIndex);
        }
    }

    return isValidDeviceType;
}

void ConnectionDefinitionParser::_parseDeviceConnections(const size_t rowIndex)
{
    if (const size_t c_DevicesCount{mConnections.size()};
        rowIndex < Data::c_MaxRackUnitsCount &&
        c_DevicesCount > 0 &&
        Data::c_MaxRackUnitsCount == mRackPositionToDeviceTypeMapping.size())
    {
        std::string currentCell; // read next cell (new current cell)

        while(_readCurrentCell(rowIndex, currentCell))
        {
            const size_t c_FileRowNumber{rowIndex + Parsers::c_RowNumberOffset};
            ErrorPtr pError{nullptr};

            // If the device type contained on the row (second column) is valid then all the connections of the device should be entered contiguously starting with the third column
            if (currentCell.empty())
            {
                // trigger error but continue parsing the next cells from the row
                if (Core::areParseableCharactersContained(_getUnparsedCellsContent(rowIndex)))
                {
                    pError = _logError(static_cast<Error_t>(ErrorCode::EMPTY_CELL), c_FileRowNumber);
                    _storeParsingError(pError);
                    _moveToNextInputColumn(rowIndex);
                    currentCell.clear();
                    continue;
                }

                break;
            }

            ConnectedDevice connectedDevice;

            if(const bool c_IsValidConnectionFormat{_parseConnectionFormatting(currentCell, connectedDevice)};
               !c_IsValidConnectionFormat) // checking if the connection format is correct (e.g. 20/3: 3 connections to device located at U20)
            {
                pError = _logError(static_cast<Error_t>(ErrorCode::INVALID_CONNECTION_FORMAT), c_FileRowNumber);
            }
            else if (connectedDevice.first <= 0 || connectedDevice.first > Data::c_MaxRackUnitsCount) // checking if the device is in the accepted U interval within rack
            {
                pError = _logError(static_cast<Error_t>(ErrorCode::DEVICE_U_POSITION_OUT_OF_RANGE), c_FileRowNumber);
            }
            else if (Data::DeviceTypeID::NO_DEVICE == mRackPositionToDeviceTypeMapping[connectedDevice.first - 1]) // check if the second device is actually placed within rack (contained in mapping table)
            {
                pError = _logError(static_cast<Error_t>(ErrorCode::TARGET_DEVICE_NOT_FOUND), c_FileRowNumber);
            }
            else if (Data::c_MaxRackUnitsCount - rowIndex == connectedDevice.first) // connection of a device to itself (connection loop) is not allowed
            {
                pError = _logError(static_cast<Error_t>(ErrorCode::DEVICE_CONNECTED_TO_ITSELF), c_FileRowNumber);
            }
            else if (0 == connectedDevice.second) // if the devices are marked as connected there should be minimum 1 connection between them
            {
                pError = _logError(static_cast<Error_t>(ErrorCode::NULL_NR_OF_CONNECTIONS), c_FileRowNumber);
            }
            else // add the U number of the second device to the list of devices connected to current device; also add the number of connections between the current device and the second device
            {
                mConnections[c_DevicesCount - 1].mConnectedDevices.emplace_back(connectedDevice.first, connectedDevice.second);
            }

            _storeParsingError(pError);
            _moveToNextInputColumn(rowIndex);
            currentCell.clear();
        }
    }
}

bool ConnectionDefinitionParser::_parseConnectionFormatting(const std::string_view source, ConnectedDevice& connectedDevice)
{
    bool isFormattingValid{true};

    const size_t c_SourceLength{source.size()};
    Index_t slashCharIndex; // index of '/'

    if (c_SourceLength > 0)
    {
        for (size_t index{0}; index < c_SourceLength; ++index)
        {
            if (isdigit(source[index]))
            {
                continue;
            }
            else if ('/' == source[index])
            {
                if (slashCharIndex.has_value()) // more than one slash
                {
                    isFormattingValid = false;
                    break;
                }
                else
                {
                    slashCharIndex = index;
                    continue;
                }
            }
            else
            {
                isFormattingValid = false;
                break;
            }
        }

        // there should be exactly one slash character and it should NOT be located in the first/last string character position
        isFormattingValid = isFormattingValid && slashCharIndex.has_value();

        if (isFormattingValid)
        {
            isFormattingValid = isFormattingValid && (slashCharIndex > 0 && slashCharIndex < c_SourceLength - 1);

            if (isFormattingValid)
            {
                const char* const pSource{source.data()};
                const size_t c_SlashCharIndex{slashCharIndex.value()};

                std::from_chars(pSource, pSource + c_SlashCharIndex, connectedDevice.first);
                std::from_chars(pSource + c_SlashCharIndex + 1, pSource + c_SourceLength, connectedDevice.second);
            }
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
    mTemplateDeviceParameters.resize(Data::c_MaxRackUnitsCount); // resize so it has same size as the mapping vector

    // uNumbers is traversed starting with the device placed at highest U position within rack
    for(auto deviceIter{mConnections.crbegin()}; deviceIter != mConnections.crend(); ++deviceIter)
    {
        assert(deviceIter->mSourceDevice > 0u); // U positions start from 1

        std::stringstream stream;
        std::string currentDeviceUPosition;

        const size_t c_CurrentDeviceUPositionAsIndex{deviceIter->mSourceDevice - 1}; // in mapping vector numbering starts at 0 so it is necessary to decrease the U number by 1
        const std::string c_DeviceType = Parsers::getDeviceTypeAsString(mRackPositionToDeviceTypeMapping[c_CurrentDeviceUPositionAsIndex]);

        assert(c_DeviceType.size() > 0u); // there should always be a non-empty string describing the device type

        mTemplateDeviceParameters[c_CurrentDeviceUPositionAsIndex] += c_DeviceType + Data::c_CSVSeparator; // appending device type
        stream << deviceIter->mSourceDevice; // recover the original U number (real position in rack)
        stream >> currentDeviceUPosition;
        mTemplateDeviceParameters[c_CurrentDeviceUPositionAsIndex] += currentDeviceUPosition + Data::c_CSVSeparator;

        // add the placeholders for the device parameters (to be filled in the next step (option 2) in connectioninput.csv so the final table can be calculated)
        mTemplateDeviceParameters[c_CurrentDeviceUPositionAsIndex] += Data::c_ConnectionInputPlaceholders.at(mRackPositionToDeviceTypeMapping[c_CurrentDeviceUPositionAsIndex]);
    }
}

ConnectionDefinitionParser::DeviceConnections::DeviceConnections(Data::UNumber_t sourceDevice)
    : mSourceDevice{sourceDevice}
{
}
