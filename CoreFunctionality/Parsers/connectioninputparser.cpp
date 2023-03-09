#include <sstream>
#include <cassert>

#include "coreutils.h"
#include "parserutils.h"
#include "connectioninputparser.h"

namespace Core = Utilities::Core;
namespace Parsers = Utilities::Parsers;

ConnectionInputParser::ConnectionInputParser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream)
    : Parser(pInputStream, pOutputStream, pErrorStream, Data::c_LabellingTableHeader)
    , mRowPortsStillNotParsedCount{0}
    , mpDevicePortsFactory{nullptr}
{
}

// It is assumed that the user has already filled in the placeholders with useful connection data.
void ConnectionInputParser::_readPayload()
{
    size_t connectionInputRowsCount{0u};

    while (!mpInputStream->eof())
    {
        ++connectionInputRowsCount;
        mInputData.resize(connectionInputRowsCount);
        getline(*mpInputStream, mInputData[connectionInputRowsCount - 1]);
    }

    // discard last empty row read from the input file if payload exists (trim)
    if(connectionInputRowsCount > 0u &&
       !Core::areParseableCharactersContained(mInputData[connectionInputRowsCount - 1]))
    {
        --connectionInputRowsCount;
        mInputData.pop_back();
    }

    if (connectionInputRowsCount > 0u)
    {
        for (auto it{mInputData.cbegin()}; it != mInputData.cend(); ++it)
        {
            if (!Core::areParseableCharactersContained(*it))
            {
                --connectionInputRowsCount;
            }
        }

        // if no parseable characters are contained in the payload then the connections input file is considered empty
        if (0u == connectionInputRowsCount)
        {
            mInputData.clear();
        }
    }
}

bool ConnectionInputParser::_parseInput()
{
    // lazy initialization of error handler
    _initializeErrorHandler();

    const size_t c_ConnectionInputRowsCount{mInputData.size()};

    if (c_ConnectionInputRowsCount > 0u)
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
            mCurrentPosition = 0; // current position in the current input row
            mFileColumnNumber = 1u; // column number from connectioninput.csv
            bool isFirstCellParsed{false}; // flag: has the cable part number been parsed on current row?

            mRowPortsStillNotParsedCount = Parsers::c_DevicesPerConnectionInputRowCount; // devices that haven't been fully parsed on the current input csv row (maximum 2 - one connection)

            while (mRowPortsStillNotParsedCount > 0)
            {
                // total number of csv cells from the connection row (cable + 2 devices) is less than required (parsing of the row should stop at once)
                if (!mCurrentPosition.has_value() || (mInputData[rowIndex].size() == mCurrentPosition))
                {
                    ErrorPtr pFewerCellsError{mpErrorHandler->logError(ErrorCode::FEWER_CELLS, rowIndex + Parsers::c_RowNumberOffset, mFileColumnNumber, *mpErrorStream)};
                    _storeParsingError(pFewerCellsError);
                    break;
                }

                // the cable field should only be parsed before parsing any device on the row
                if (!isFirstCellParsed)
                {
                    mCurrentPosition = _parseCablePartNumber(rowIndex, mCurrentPosition);
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
        ErrorPtr pEmptyConnectionInputFileError{mpErrorHandler->logError(ErrorCode::EMPTY_CONNECTION_INPUT_FILE, 1, 1, *mpErrorStream)};
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
    const size_t c_CablePartNumbersEntriesCount{mCablePartNumbersEntries.size()};

    if (c_CablePartNumbersEntriesCount > 0u)
    {
        // obviously 1 cable per connection between 2 device ports
        assert(Parsers::c_DevicesPerConnectionInputRowCount * c_CablePartNumbersEntriesCount == mDevicePorts.size());

        for (auto deviceIter{mDevicePorts.cbegin()}; deviceIter != mDevicePorts.cend(); ++deviceIter)
        {
            // for each device the description and lable are built by considering the even/odd index (even, e.g. 0: first device on the row; odd, e.g. 3: second device on the row)
            (*deviceIter)->updateDescriptionAndLabel();
        }

        mOutputData.resize(c_CablePartNumbersEntriesCount); // number of output rows should match the number of input rows

        size_t connectionNumber{1u}; // number of the connection to be written on each row of the output file
        size_t srcDeviceIndex{0u}; // index of the first device of the connection
        size_t destDeviceIndex{1u}; // index of the second device of the connection

        // calculate the row strings for the output file (labellingtable.csv)
        for (size_t connectionIndex{0u}; connectionIndex < c_CablePartNumbersEntriesCount; ++connectionIndex)
        {
            connectionNumber = _buildConnectionEntry(connectionNumber,
                                                     mCablePartNumbersEntries[connectionIndex],
                                                     mDevicePorts[srcDeviceIndex],
                                                     mDevicePorts[destDeviceIndex],
                                                     mOutputData[connectionIndex]);

            srcDeviceIndex += Parsers::c_DevicesPerConnectionInputRowCount;
            destDeviceIndex += Parsers::c_DevicesPerConnectionInputRowCount;
        }
    }
}

void ConnectionInputParser::_reset()
{
    mDevicePorts.clear();
    mCablePartNumbersEntries.clear();

    Parser::_reset();
}

Index_t ConnectionInputParser::_parseCablePartNumber(const size_t rowIndex, const Index_t currentPosition)
{
    assert(rowIndex < mInputData.size());

    const Index_t resultingPosition{Core::readDataField(mInputData[rowIndex], mCablePartNumbersEntries[rowIndex], currentPosition)};

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

    ++mFileColumnNumber;

    return resultingPosition;
}

bool ConnectionInputParser::_parseDevicePort(const size_t rowIndex)
{
    assert(nullptr != mpErrorHandler.get());

    const size_t c_FileRowNumber{rowIndex + Parsers::c_RowNumberOffset};
    bool canContinueRowParsing{true};

    std::string deviceType;
    mCurrentPosition = Core::readDataField(mInputData[rowIndex], deviceType, mCurrentPosition);

    const Data::DeviceTypeID deviceTypeID{Parsers::getDeviceTypeID(deviceType)};

    // the device should both be known (correct device type string entered by user) and supported (instantiatable) by device factory (code should be in place for factory instantiating it)
    bool isDeviceKnown{false};

    if (Data::DeviceTypeID::UNKNOWN_DEVICE != deviceTypeID)
    {
        // the U position of the device should be valid (1U - 50U)
        bool isDeviceUPositionValid{false};
        std::string deviceUPosition;

        mCurrentPosition = Core::readDataField(mInputData[rowIndex], deviceUPosition, mCurrentPosition);

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
            const size_t c_NewColumnNumber{mFileColumnNumber + Parsers::c_DevicePortParamsColumnOffset};

            DevicePortPtr pDevicePort{mpDevicePortsFactory->createDevicePort(deviceTypeID, deviceUPosition, c_FileRowNumber, c_NewColumnNumber, c_IsSourceDevice)};

            if(nullptr != pDevicePort)
            {
                pDevicePort->init();

                mFileColumnNumber = c_NewColumnNumber; // new CSV column number to be stored once device port is created
                mDevicePorts.push_back(pDevicePort);

                std::vector<ErrorPtr> parsingErrors;
                mCurrentPosition = pDevicePort->parseInputData(mInputData[rowIndex], mCurrentPosition, *mpErrorHandler, *mpErrorStream, parsingErrors);

                _storeMultipleParsingErrors(parsingErrors);



                // the remaining row part (second device) should no longer be parsed if there are fewer cells (in total) than necessary
                if (c_FileRowNumber == mpErrorHandler->getLastLoggingRowNumber() &&
                    mpErrorHandler->fewerCellsErrorLogged() &&
                    Parsers::c_DevicesPerConnectionInputRowCount == mRowPortsStillNotParsedCount)
                {
                    canContinueRowParsing = false;
                }
                else
                {
                    mFileColumnNumber = pDevicePort->getFileColumnNumber();
                    isDeviceKnown = true;
                }
            }
        }
        else
        {
            ErrorPtr pInvalidUPositionValueError{mpErrorHandler->logError(ErrorCode::INVALID_U_POSITION_VALUE, c_FileRowNumber, mFileColumnNumber + 1, *mpErrorStream)};
            _storeParsingError(pInvalidUPositionValueError);
            canContinueRowParsing = false;
        }
    }

    if (canContinueRowParsing)
    {
        if (!isDeviceKnown)
        {
            ErrorPtr pUnknownDeviceError{mpErrorHandler->logError(ErrorCode::UNKNOWN_DEVICE, c_FileRowNumber, mFileColumnNumber, *mpErrorStream)};
            _storeParsingError(pUnknownDeviceError);
            canContinueRowParsing = false;
        }
    }

    return canContinueRowParsing;
}

void ConnectionInputParser::_storeMultipleParsingErrors(const std::vector<ErrorPtr>& parsingErrors)
{
    for(const auto& pError : parsingErrors)
    {
        if (nullptr != pError.get())
        {
            mParsingErrors.push_back(pError);
        }
    }
}

size_t ConnectionInputParser::_buildConnectionEntry(const size_t currentEntryNumber,
                                                    const std::string_view cablePartNumber,
                                                    const DevicePortPtr pFirstDevicePort,
                                                    const DevicePortPtr pSecondDevicePort,
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
