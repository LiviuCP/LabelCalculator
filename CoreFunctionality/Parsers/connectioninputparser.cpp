#include <algorithm>
#include <sstream>

#include "errorcodes.h"
#include "coreutils.h"
#include "parserutils.h"
#include "deviceportsfactory.h"
#include "deviceport.h"
#include "connectioninputparser.h"

namespace Core = Utilities::Core;
namespace Parsers = Utilities::Parsers;

ConnectionInputParser::ConnectionInputParser(const InputStreamPtr pInputStream, const OutputStreamPtr pOutputStream, const ErrorStreamPtr pErrorStream)
    : Parser(pInputStream, pOutputStream, pErrorStream, Data::c_LabellingTableHeader)
{
}

// It is assumed that the user has already filled in the placeholders with useful connection data.
void ConnectionInputParser::_readPayload()
{
    while (!_isInputStreamConsumed())
    {
        _readLineAndAppendToInput();
    }
}

bool ConnectionInputParser::_parseInput()
{
    if (const size_t c_ConnectionInputRowsCount{_getInputRowsCount()}; c_ConnectionInputRowsCount > 0u)
    {
        mParsedRowsInfo.resize(c_ConnectionInputRowsCount);

        for (size_t rowIndex{0u}; rowIndex < c_ConnectionInputRowsCount; ++rowIndex)
        {
            _moveToInputRowStart(rowIndex);

            // the cable field should only be parsed before parsing any device on the row
            _parseCablePartNumber(rowIndex);

            int& unparsedPortsCount{mParsedRowsInfo[rowIndex].mUnparsedPortsCount};
            unparsedPortsCount = Parsers::c_DevicesPerConnectionInputRowCount; // devices that haven't been fully parsed on the current input csv row (maximum 2 - one connection)

            while (unparsedPortsCount > 0)
            {
                if (const bool c_CanContinueRowParsing{_parseDevicePort(rowIndex)}; !c_CanContinueRowParsing)
                {
                    break;
                }

                --unparsedPortsCount;
            }
        }
    }
    else
    {
        ErrorPtr pEmptyConnectionInputFileError{_logError(static_cast<Error_t>(ErrorCode::EMPTY_CONNECTION_INPUT_FILE), 1, true)};
        _storeParsingError(pEmptyConnectionInputFileError);
    }

    const bool c_ErrorsOccurred{_logParsingErrorsToFile()};

    return c_ErrorsOccurred;
}

void ConnectionInputParser::_buildOutput()
{
    if (const size_t c_ConnectionInputRowsCount{_getInputRowsCount()}; mParsedRowsInfo.size() == c_ConnectionInputRowsCount)
    {
        for (size_t rowIndex{0u}; rowIndex < c_ConnectionInputRowsCount; ++rowIndex)
        {
            std::string outputRow;

            if (const bool c_IsValidConnection{_buildOutputRow(rowIndex, outputRow)}; c_IsValidConnection)
            {
                _appendRowToOutput(outputRow);
            }
        }
    }
}

void ConnectionInputParser::_reset()
{
    mParsedRowsInfo.clear();
    Parser::_reset();
}

void ConnectionInputParser::_parseCablePartNumber(const size_t rowIndex)
{
    if (rowIndex < mParsedRowsInfo.size())
    {
        std::string& cablePartNumber{mParsedRowsInfo[rowIndex].mCablePartNumber};
        const std::string_view previousCablePartNumber{rowIndex > 0u ? mParsedRowsInfo[rowIndex - 1].mCablePartNumber : ""};

        if (const bool c_CellSuccessfullyRead{_readCurrentCell(rowIndex, cablePartNumber)}; c_CellSuccessfullyRead)
        {
            // if no cable PN entered on current row take the PN for previous row
            if (0u == cablePartNumber.size())
            {
                cablePartNumber = previousCablePartNumber;
            }

            if (0u == cablePartNumber.size() || Core::areInvalidCharactersContained(cablePartNumber))
            {
                cablePartNumber = Parsers::c_InvalidCablePNErrorText;
            }

            _moveToNextInputColumn(rowIndex);
        }
    }
}

bool ConnectionInputParser::_parseDevicePort(const size_t rowIndex)
{
    bool canContinueRowParsing{rowIndex < mParsedRowsInfo.size()};
    Data::DeviceTypeID deviceTypeID{Data::DeviceTypeID::NO_DEVICE};

    if (canContinueRowParsing)
    {
        deviceTypeID = _parseDeviceType(rowIndex);
        canContinueRowParsing = (Data::DeviceTypeID::UNKNOWN_DEVICE != deviceTypeID && Data::DeviceTypeID::NO_DEVICE != deviceTypeID);
    }

    std::string deviceUPosition;

    if (canContinueRowParsing)
    {
        _moveToNextInputColumn(rowIndex); // move to the U position column
        canContinueRowParsing = _parseDeviceUPosition(rowIndex, deviceUPosition);
    }

    if (canContinueRowParsing)
    {
        _moveToNextInputColumn(rowIndex); // move to the next column after U position, start parsing the actual device port parameters

        const int& unparsedPortsCount{mParsedRowsInfo[rowIndex].mUnparsedPortsCount};
        const size_t c_FileRowNumber{rowIndex + Parsers::c_RowNumberOffset};
        const bool c_IsSourceDevice{0 == unparsedPortsCount % Parsers::c_DevicesPerConnectionInputRowCount};

        _registerSubParser(DevicePortsFactory::createDevicePort(deviceTypeID, deviceUPosition, c_FileRowNumber, c_IsSourceDevice));

        std::vector<ErrorPtr> parsingErrors;
        _doSubParsing(rowIndex, unparsedPortsCount % Parsers::c_DevicesPerConnectionInputRowCount, parsingErrors);

        for(const auto& pError : parsingErrors)
        {
            _storeParsingError(pError);
        }

        if (parsingErrors.cend() != std::find_if(parsingErrors.cbegin(), parsingErrors.cend(), [](const ErrorPtr pError) {return pError && static_cast<Error_t>(ErrorCode::FEWER_CELLS) == pError->getErrorCode();}))
        {
            canContinueRowParsing = false; // the remaining row part should no longer be parsed if there are fewer cells (in total) than necessary
        }
    }

    return canContinueRowParsing;
}

Data::DeviceTypeID ConnectionInputParser::_parseDeviceType(const size_t rowIndex)
{
    Data::DeviceTypeID deviceTypeID{Data::DeviceTypeID::NO_DEVICE};

    if (rowIndex < mParsedRowsInfo.size())
    {
        std::string deviceType;
        const bool c_CellSuccessfullyRead{_readCurrentCell(rowIndex, deviceType)};

        if (c_CellSuccessfullyRead)
        {
            deviceTypeID = Parsers::getDeviceTypeID(deviceType);
        }
        else
        {
            // total number of csv cells from the connection row (cable + 2 devices) is less than required (parsing of the row should stop at once)
            ErrorPtr pFewerCellsError{_logError(static_cast<Error_t>(ErrorCode::FEWER_CELLS), rowIndex + Parsers::c_RowNumberOffset)};
            _storeParsingError(pFewerCellsError);
        }

        // NO_DEVICE should normally not be a case, it's added just for defensive programming purposes (considered equivalent to UNKNOWN_DEVICE)
        if (Data::DeviceTypeID::NO_DEVICE == deviceTypeID)
        {
            deviceTypeID = Data::DeviceTypeID::UNKNOWN_DEVICE;
        }

        // the device should both be known (correct device type string entered by user) and supported (instantiatable) by device factory (code should be in place for factory instantiating it)
        if (c_CellSuccessfullyRead && Data::DeviceTypeID::UNKNOWN_DEVICE == deviceTypeID)
        {
            const size_t c_FileRowNumber{rowIndex + Parsers::c_RowNumberOffset};
            ErrorPtr pUnknownDeviceError{_logError(static_cast<Error_t>(ErrorCode::UNKNOWN_DEVICE), c_FileRowNumber)};
            _storeParsingError(pUnknownDeviceError);
        }
    }

    return deviceTypeID;
}

// the U position of the device should be valid (1U - 50U)
bool ConnectionInputParser::_parseDeviceUPosition(const size_t rowIndex, std::string& deviceUPosition)
{
    bool isDeviceUPositionValid{false};

    if (rowIndex < mParsedRowsInfo.size())
    {
        if (const bool c_CellSuccessfullyRead{_readCurrentCell(rowIndex, deviceUPosition)}; c_CellSuccessfullyRead)
        {
            if (deviceUPosition.size() > 0u && Core::isDigitString(deviceUPosition))
            {
                const Data::UNumber_t c_DeviceUPositionNum{static_cast<Data::UNumber_t>(std::stoi(deviceUPosition))};
                isDeviceUPositionValid = c_DeviceUPositionNum > 0u && c_DeviceUPositionNum <= Data::c_MaxRackUnitsCount;
            }

            if (!isDeviceUPositionValid)
            {
                const size_t c_FileRowNumber{rowIndex + Parsers::c_RowNumberOffset};
                ErrorPtr pInvalidUPositionValueError{_logError(static_cast<Error_t>(ErrorCode::INVALID_U_POSITION_VALUE), c_FileRowNumber)};
                _storeParsingError(pInvalidUPositionValueError);
            }
        }
        else
        {
            // total number of csv cells from the connection row (cable + 2 devices) is less than required (parsing of the row should stop at once)
            ErrorPtr pFewerCellsError{_logError(static_cast<Error_t>(ErrorCode::FEWER_CELLS), rowIndex + Parsers::c_RowNumberOffset)};
            _storeParsingError(pFewerCellsError);
        }
    }

    return isDeviceUPositionValid;
}

bool ConnectionInputParser::_buildOutputRow(const size_t rowIndex, std::string& currentRow)
{
    DevicePort* pFirstDevicePort{nullptr};
    DevicePort* pSecondDevicePort{nullptr};

    bool success{false};

    if (rowIndex < mParsedRowsInfo.size())
    {
        pFirstDevicePort = dynamic_cast<DevicePort*>(_getSubParser(rowIndex, 0));

        if (pFirstDevicePort)
        {
            pSecondDevicePort = dynamic_cast<DevicePort*>(_getSubParser(rowIndex, 1));
        }

        if (pSecondDevicePort)
        {
            pFirstDevicePort->updateDescriptionAndLabel();
            pSecondDevicePort->updateDescriptionAndLabel();

            std::stringstream str;

            // number of the connection to be written on each row of the output file
            const size_t c_ConnectionNumber{rowIndex + 1};

            currentRow.clear();
            str << c_ConnectionNumber;
            str >> currentRow;

            currentRow += Data::c_CSVSeparator;
            currentRow += mParsedRowsInfo[rowIndex].mCablePartNumber;
            currentRow += Data::c_CSVSeparator;
            currentRow += pFirstDevicePort->getDescription();
            currentRow += Data::c_CSVSeparator;
            currentRow += pFirstDevicePort->getLabel();
            currentRow += Data::c_CSVSeparator;
            currentRow += pSecondDevicePort->getDescription();
            currentRow += Data::c_CSVSeparator;
            currentRow += pSecondDevicePort->getLabel();

            success = true;
        }
    }

    return success;
}

ConnectionInputParser::ParsedRowInfo::ParsedRowInfo()
    : mUnparsedPortsCount{2}
{
}
