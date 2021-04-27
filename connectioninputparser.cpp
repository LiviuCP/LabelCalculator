#include <sstream>
#include <cassert>

#include "deviceportsfactory.h"
#include "errortypes.h"
#include "connectioninputparser.h"

ConnectionInputParser::ConnectionInputParser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream)
    : Parser(pInputStream, pOutputStream, pErrorStream, c_OutputHeader)
    , mRowDevicesStillNotParsedCount{0}
{
}

// It is assumed that the user has already filled in the placeholders with useful connection data.
void ConnectionInputParser::_readInput()
{
    int connectionInputRowsCount{0};

    mpInputStream->seekg(0);
    std::string header;
    getline(*mpInputStream, header);

    while (!mpInputStream->eof())
    {
        ++connectionInputRowsCount;
        mInputData.resize(connectionInputRowsCount);
        getline(*mpInputStream, mInputData[connectionInputRowsCount - 1]);
    }

    // discard last empty row read from the input file if payload exists (trim)
    if(connectionInputRowsCount > 1 && 0 == mInputData[connectionInputRowsCount - 1].size())
    {
        --connectionInputRowsCount;
        mInputData.resize(connectionInputRowsCount);
    }
}

bool ConnectionInputParser::_parseInput()
{
    DevicePortsFactory deviceFactory;

    int cablePartNumbersEntriesCount{0};
    const int c_ConnectionInputRowsCount{static_cast<int>(mInputData.size())};

    for (int rowIndex{0}; rowIndex < c_ConnectionInputRowsCount; ++rowIndex)
    {
        const int c_InputRowsLength{static_cast<int>(mInputData[rowIndex].size())};
        int currentPosition{0}; // current position in the current input row
        int columnNumber{1}; // column number from connectioninput.csv
        bool isFirstCellParsed{false}; // flag: has the cable part number been parsed on current row?

        std::string cablePartNumber; // stores the cable part number written on previous row

        mRowDevicesStillNotParsedCount = c_DevicesPerConnectionInputRowCount; // devices that haven't been fully parsed on the current input csv row (maximum 2 - one connection)

        while (mRowDevicesStillNotParsedCount > 0)
        {
            // total number of csv cells from the connection row (cable + 2 devices) is less than required (parsing of the row should stop at once)
            if (currentPosition == c_InputRowsLength || -1 == currentPosition)
            {
                ErrorPtr pFewerCellsError{std::make_shared<FewerCellsError>(*mpErrorStream)};
                _storeParsingErrorAndLocation(pFewerCellsError, rowIndex + c_RowNumberOffset, columnNumber);
                break;
            }

            // the cable field should only be parsed before parsing any device on the row
            if (!isFirstCellParsed)
            {
                ++cablePartNumbersEntriesCount;
                mCablePartNumbersEntries.resize(cablePartNumbersEntriesCount);
                currentPosition = readDataField(mInputData[rowIndex], mCablePartNumbersEntries[cablePartNumbersEntriesCount - 1], currentPosition);

                // if no cable PN entered on current row take the PN for previous row
                if (0 == mCablePartNumbersEntries[cablePartNumbersEntriesCount - 1].size())
                {
                    mCablePartNumbersEntries[cablePartNumbersEntriesCount - 1] = cablePartNumber;
                }
                else
                {
                    cablePartNumber = mCablePartNumbersEntries[cablePartNumbersEntriesCount - 1];
                }

                isFirstCellParsed = true;
                ++columnNumber;
                continue;
            }

            std::string deviceType;
            currentPosition = readDataField(mInputData[rowIndex], deviceType, currentPosition);
            const DeviceTypeID deviceTypeID{getDeviceTypeID(deviceType)};

            // the device should both be known (correct device type string entered by user) and supported (instantiatable) by device factory (code should be in place for factory instantiating it)
            bool isDeviceKnown{false};

            if (DeviceTypeID::UNKNOWN_DEVICE != deviceTypeID)
            {
                // the U position of the device should be valid (1U - 50U)
                bool isDeviceUPositionValid{false};
                std::string deviceUPosition;

                currentPosition = readDataField(mInputData[rowIndex], deviceUPosition, currentPosition);

                if (deviceUPosition.size() > 0 &&
                    isDigitString(deviceUPosition))
                {
                    const int c_DeviceUPositionNum{std::stoi(deviceUPosition)};
                    if (c_DeviceUPositionNum > 0 && c_DeviceUPositionNum <= c_MaxNrOfRackUnits)
                    {
                        isDeviceUPositionValid = true;
                    }
                }

                if (isDeviceUPositionValid)
                {
                    const bool c_IsSourceDevice{0 == mRowDevicesStillNotParsedCount % c_DevicesPerConnectionInputRowCount};

                    DevicePortPtr pDevicePort{deviceFactory.createDevicePort(deviceTypeID, deviceUPosition, c_IsSourceDevice)};

                    if(nullptr != pDevicePort)
                    {
                        columnNumber += 2; // pass through the device type and device U position columns and move to the first device parameter column

                        pDevicePort->setCSVRowNumber(rowIndex + c_RowNumberOffset);     // +2: csv lines start at 1 and first row is ignored
                        pDevicePort->setCSVColumnNumber(columnNumber);
                        mDevicePorts.push_back(pDevicePort);

                        std::vector<ErrorPtr> parsingErrors;
                        currentPosition = pDevicePort->parseInputData(mInputData[rowIndex], currentPosition, parsingErrors, *mpErrorStream);

                        const bool c_FewerCellsErrorOccurred{_storeExternalParsingErrors(parsingErrors)};

                        if (c_FewerCellsErrorOccurred)
                        {
                            break;
                        }

                        columnNumber = pDevicePort->getCSVColumnNumber();
                        --mRowDevicesStillNotParsedCount;
                        isDeviceKnown = true;
                    }
                }
                else
                {
                    ErrorPtr pInvalidDeviceUPositionError{std::make_shared<InvalidDeviceUPositionError>(*mpErrorStream)};
                    _storeParsingErrorAndLocation(pInvalidDeviceUPositionError, rowIndex + c_RowNumberOffset, columnNumber + 1);
                    break;
                }
            }

            if (!isDeviceKnown)
            {
                ErrorPtr pUnknownDeviceError{std::make_shared<UnknownDeviceError>(*mpErrorStream)};
                _storeParsingErrorAndLocation(pUnknownDeviceError, rowIndex + c_RowNumberOffset, columnNumber);
                break;
            }
        }
    }

    assert(mCablePartNumbersEntries.size() == mInputData.size()); // 1 cable per connection

    const bool c_ErrorsOccurred{_logParsingErrorsToFile()};

    if (!c_ErrorsOccurred)
    {
        assert(deviceFactory.getCreatedDevicePortsCount() == static_cast<int>(mCablePartNumbersEntries.size()) * c_DevicesPerConnectionInputRowCount); // 1 cable, two connected devices
    }

    return c_ErrorsOccurred;
}

void ConnectionInputParser::_buildOutput()
{
    const int c_DevicesCount{static_cast<int>(mDevicePorts.size())};
    const int c_CablePartNumbersEntriesCount{static_cast<int>(mCablePartNumbersEntries.size())};

    for (int deviceIndex{0}; deviceIndex < c_DevicesCount; ++deviceIndex)
    {
        // for each device the description and lable are built by considering the even/odd index (even, e.g. 0: first device on the row; odd, e.g. 3: second device on the row)
        mDevicePorts[deviceIndex]->computeDescriptionAndLabel();
    }

    mOutputData.resize(c_CablePartNumbersEntriesCount); // number of output rows should match the number of input rows

    int connectionNumber{1}; // number of the connection to be written on each row of the output file
    int firstDeviceIndex{0}; // index of the first device of the connection
    int secondDeviceIndex{1}; // index of the second device of the connection

    // calculate the row strings for the output file (labellingtable.csv)
    for (int connectionIndex{0}; connectionIndex < c_CablePartNumbersEntriesCount; ++connectionIndex)
    {
        _buildConnectionEntry(mOutputData[connectionIndex],
                             connectionNumber,
                             mDevicePorts[firstDeviceIndex],
                             mDevicePorts[secondDeviceIndex],
                             mCablePartNumbersEntries[connectionIndex]);

        firstDeviceIndex += 2;
        secondDeviceIndex += 2;
    }
}

void ConnectionInputParser::_reset()
{
    mDevicePorts.clear();
    mCablePartNumbersEntries.clear();

    Parser::_reset();
}

bool ConnectionInputParser::_storeExternalParsingErrors(const std::vector<ErrorPtr>& deviceParsingErrors)
{
    bool areFewerCellsOnCurrentRow{false};

    for(const auto& pError : deviceParsingErrors)
    {
        if (nullptr != pError.get())
        {
            // the remaining row part (second device) should no longer be parsed if there are fewer cells (in total) than necessary
            if (nullptr != dynamic_cast<FewerCellsError*>(pError.get()) &&
                c_DevicesPerConnectionInputRowCount == mRowDevicesStillNotParsedCount)
            {
                areFewerCellsOnCurrentRow = true;
            }

            mParsingErrors.push_back(pError);
        }
    }

    return areFewerCellsOnCurrentRow;
}

void ConnectionInputParser::_buildConnectionEntry(std::string& entry,
                                                  int& entryNumber,
                                                  const DevicePortPtr pFirstDevicePort,
                                                  const DevicePortPtr pSecondDevicePort,
                                                  const std::string& cablePartNumber)
{
    std::stringstream str;

    entry.clear();
    str << entryNumber;
    str >> entry;

    entry += c_CSVSeparator;
    entry += cablePartNumber;
    entry += c_CSVSeparator;
    entry += pFirstDevicePort->getDescription();
    entry += c_CSVSeparator;
    entry += pFirstDevicePort->getLabel();
    entry += c_CSVSeparator;
    entry += pSecondDevicePort->getDescription();
    entry += c_CSVSeparator;
    entry += pSecondDevicePort->getLabel();

    ++entryNumber;
}
