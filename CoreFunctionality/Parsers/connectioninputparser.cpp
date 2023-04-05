#include <sstream>
#include <cassert>

#include "errorcodes.h"
#include "coreutils.h"
#include "parserutils.h"
#include "connectioninputparser.h"

namespace Core = Utilities::Core;
namespace Parsers = Utilities::Parsers;

ConnectionInputParser::ConnectionInputParser(const InputStreamPtr pInputStream, const OutputStreamPtr pOutputStream, const ErrorStreamPtr pErrorStream)
    : Parser(pInputStream, pOutputStream, pErrorStream, Data::c_LabellingTableHeader)
    , mRowPortsStillNotParsedCount{0}
    , mpDevicePortsFactory{nullptr}
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
        // lazy initialization of device factory
        if (nullptr == mpDevicePortsFactory.get())
        {
            mpDevicePortsFactory = std::make_unique<DevicePortsFactory>();
        }
        else
        {
            mpDevicePortsFactory->reset();
        }

        mCablePartNumbersEntries.resize(c_ConnectionInputRowsCount);

        for (size_t rowIndex{0u}; rowIndex < c_ConnectionInputRowsCount; ++rowIndex)
        {
            _moveToInputRowStart(rowIndex);
            bool isFirstCellParsed{false}; // flag: has the cable part number been parsed on current row?

            mRowPortsStillNotParsedCount = Parsers::c_DevicesPerConnectionInputRowCount; // devices that haven't been fully parsed on the current input csv row (maximum 2 - one connection)

            while (mRowPortsStillNotParsedCount > 0)
            {
                // total number of csv cells from the connection row (cable + 2 devices) is less than required (parsing of the row should stop at once)
                if (!_isValidCurrentPosition(rowIndex) || _isEndOfInputDataRow(rowIndex))
                {
                    ErrorPtr pFewerCellsError{_logError(static_cast<Error_t>(ErrorCode::FEWER_CELLS), rowIndex + Parsers::c_RowNumberOffset)};
                    _storeParsingError(pFewerCellsError);
                    break;
                }

                // the cable field should only be parsed before parsing any device on the row
                if (!isFirstCellParsed)
                {
                    _parseCablePartNumber(rowIndex);
                    isFirstCellParsed = true;
                    continue;
                }

                const bool c_CanContinueRowParsing{_parseDevicePort(rowIndex)};

                if (!c_CanContinueRowParsing)
                {
                    break;
                }

                --mRowPortsStillNotParsedCount;
            }
        }
    }
    else
    {
        ErrorPtr pEmptyConnectionInputFileError{_logError(static_cast<Error_t>(ErrorCode::EMPTY_CONNECTION_INPUT_FILE), 1, true)};
        _storeParsingError(pEmptyConnectionInputFileError);
    }

    const bool c_ErrorsOccurred{_logParsingErrorsToFile()};

    if (!c_ErrorsOccurred)
    {
        // 1 cable, two connected devices
        assert(mpDevicePortsFactory->getCreatedDevicePortsCount() == mCablePartNumbersEntries.size() * Parsers::c_DevicesPerConnectionInputRowCount);
    }

    return c_ErrorsOccurred;
}

void ConnectionInputParser::_buildOutput()
{
    if (const size_t c_ConnectionInputRowsCount{_getInputRowsCount()}; mCablePartNumbersEntries.size() == c_ConnectionInputRowsCount)
    {
        size_t connectionNumber{1u}; // number of the connection to be written on each row of the output file

        for (size_t rowIndex{0u}; rowIndex < c_ConnectionInputRowsCount; ++rowIndex)
        {
            DevicePort* pFirstDevicePort{dynamic_cast<DevicePort*>(_getSubParser(rowIndex, 0))};
            DevicePort* pSecondDevicePort{dynamic_cast<DevicePort*>(_getSubParser(rowIndex, 1))};

            if (pFirstDevicePort && pSecondDevicePort)
            {
                pFirstDevicePort->updateDescriptionAndLabel();
                pSecondDevicePort->updateDescriptionAndLabel();

                std::string outputRow;
                connectionNumber = _buildConnectionEntry(connectionNumber,
                                                         mCablePartNumbersEntries[rowIndex],
                                                         pFirstDevicePort,
                                                         pSecondDevicePort,
                                                         outputRow);

                _appendRowToOutput(outputRow);
            }
        }
    }
}

void ConnectionInputParser::_reset()
{
    mCablePartNumbersEntries.clear();
    Parser::_reset();
}

void ConnectionInputParser::_parseCablePartNumber(const size_t rowIndex)
{
    if (const bool c_CellSuccessfullyRead{_readCurrentCell(rowIndex, mCablePartNumbersEntries[rowIndex])}; c_CellSuccessfullyRead)
    {
        // if no cable PN entered on current row take the PN for previous row
        if (0u == mCablePartNumbersEntries[rowIndex].size())
        {
            mCablePartNumbersEntries[rowIndex] = mCurrentCablePartNumber;
        }
        else
        {
            mCurrentCablePartNumber = mCablePartNumbersEntries[rowIndex];
        }

        if (0u == mCablePartNumbersEntries[rowIndex].size()                              ||
                Core::areInvalidCharactersContained(mCablePartNumbersEntries[rowIndex]))
        {
            mCablePartNumbersEntries[rowIndex] = Parsers::c_InvalidCablePNErrorText;
        }

        _moveToNextInputColumn(rowIndex);
    }
}

bool ConnectionInputParser::_parseDevicePort(const size_t rowIndex)
{
    const size_t c_FileRowNumber{rowIndex + Parsers::c_RowNumberOffset};
    bool canContinueRowParsing{true};

    std::string deviceType;
    (void)_readCurrentCell(rowIndex, deviceType); // TODO: refactor the whole method by taking into account the case when the _readCurrentCell() returns false!

    const Data::DeviceTypeID deviceTypeID{Parsers::getDeviceTypeID(deviceType)};

    // the device should both be known (correct device type string entered by user) and supported (instantiatable) by device factory (code should be in place for factory instantiating it)
    bool isDeviceKnown{false};

    // NO_DEVICE should normally not be a case, it's added just for defensive programming purposes (considered equivalent to UNKNOWN_DEVICE)
    if (Data::DeviceTypeID::UNKNOWN_DEVICE != deviceTypeID && Data::DeviceTypeID::NO_DEVICE != deviceTypeID)
    {
        isDeviceKnown = true;

        // the U position of the device should be valid (1U - 50U)
        bool isDeviceUPositionValid{false};
        std::string deviceUPosition;

        (void)_readCurrentCell(rowIndex, deviceUPosition); // TODO: refactor the whole method by taking into account the case when the _readCurrentCell() returns false!

        if (deviceUPosition.size() > 0u &&
            Core::isDigitString(deviceUPosition))
        {
            const Data::UNumber_t c_DeviceUPositionNum{static_cast<Data::UNumber_t>(std::stoi(deviceUPosition))};
            if (c_DeviceUPositionNum > 0u && c_DeviceUPositionNum <= Data::c_MaxRackUnitsCount)
            {
                isDeviceUPositionValid = true;
            }
        }

        if (isDeviceUPositionValid)
        {
            const bool c_IsSourceDevice{0 == mRowPortsStillNotParsedCount % Parsers::c_DevicesPerConnectionInputRowCount};

            // new CSV column number: pass through the device type and device U position columns and move to the first device parameter column
            _moveToNextInputColumn(rowIndex);
            _moveToNextInputColumn(rowIndex);

            if (mpDevicePortsFactory)
            {
                _registerSubParser(mpDevicePortsFactory->createDevicePort(deviceTypeID, deviceUPosition, c_FileRowNumber, c_IsSourceDevice));

                std::vector<ErrorPtr> parsingErrors;
                _doSubParsing(rowIndex, mRowPortsStillNotParsedCount % Parsers::c_DevicesPerConnectionInputRowCount, parsingErrors);

                bool fewerCellsErrorOccurred{false};

                for(const auto& pError : parsingErrors)
                {
                    _storeParsingError(pError);

                    if (pError && static_cast<Error_t>(ErrorCode::FEWER_CELLS) == pError->getErrorCode())
                    {
                        fewerCellsErrorOccurred = true;
                    }
                }

                // the remaining row part (second device) should no longer be parsed if there are fewer cells (in total) than necessary
                if (fewerCellsErrorOccurred && Parsers::c_DevicesPerConnectionInputRowCount == mRowPortsStillNotParsedCount)
                {
                    canContinueRowParsing = false;
                }
            }
            else
            {
                assert(false);
            }
        }
        else
        {
            _moveToNextInputColumn(rowIndex);
            ErrorPtr pInvalidUPositionValueError{_logError(static_cast<Error_t>(ErrorCode::INVALID_U_POSITION_VALUE), c_FileRowNumber)};
            _storeParsingError(pInvalidUPositionValueError);
            canContinueRowParsing = false;
        }
    }

    if (canContinueRowParsing)
    {
        if (!isDeviceKnown)
        {
            ErrorPtr pUnknownDeviceError{_logError(static_cast<Error_t>(ErrorCode::UNKNOWN_DEVICE), c_FileRowNumber)};
            _storeParsingError(pUnknownDeviceError);
            canContinueRowParsing = false;
        }
    }

    return canContinueRowParsing;
}

size_t ConnectionInputParser::_buildConnectionEntry(const size_t currentEntryNumber,
                                                    const std::string_view cablePartNumber,
                                                    const DevicePort* pFirstDevicePort,
                                                    const DevicePort* pSecondDevicePort,
                                                    std::string& currentEntry)
{
    std::stringstream str;

    currentEntry.clear();
    str << currentEntryNumber;
    str >> currentEntry;

    currentEntry += Data::c_CSVSeparator;
    currentEntry += cablePartNumber;
    currentEntry += Data::c_CSVSeparator;
    currentEntry += pFirstDevicePort->getDescription();
    currentEntry += Data::c_CSVSeparator;
    currentEntry += pFirstDevicePort->getLabel();
    currentEntry += Data::c_CSVSeparator;
    currentEntry += pSecondDevicePort->getDescription();
    currentEntry += Data::c_CSVSeparator;
    currentEntry += pSecondDevicePort->getLabel();

    const size_t c_NewEntryNumber{currentEntryNumber + 1};

    return c_NewEntryNumber;
}
