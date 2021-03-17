#include <sstream>

#include "devicefactory.h"
#include "error.h"
#include "connectioninput.h"

void readConnectionInput(std::ifstream& inputStream, std::vector<std::string>& connectionInputRows)
{
    assert(inputStream.is_open());

    connectionInputRows.clear();
    int connectionInputRowsCount{0};

    inputStream.seekg(0);
    std::string header;
    getline(inputStream, header);

    while (!inputStream.eof())
    {
        ++connectionInputRowsCount;
        connectionInputRows.resize(connectionInputRowsCount);
        getline(inputStream,connectionInputRows[connectionInputRowsCount-1]);
    }

    // discard last empty row read from the input file if payload exists (trim)
    if(connectionInputRowsCount > 1 && 0 == connectionInputRows[connectionInputRowsCount - 1].size())
    {
        --connectionInputRowsCount;
        connectionInputRows.resize(connectionInputRowsCount);
    }
}

bool parseConnectionInput(std::ofstream& errorStream,
                          std::vector<Device*>& devices,
                          std::vector<std::string>& cablePartNumbersEntries,
                          const std::vector<std::string>& connectionInputRows)
{
    std::vector<Error*> allParsingErrors; // gather all parsing errors here and write them to output file once parsing is complete (if any errors)

    devices.clear();
    cablePartNumbersEntries.clear();

    assert(errorStream.is_open());

    DeviceFactory deviceFactory;

    int cablePartNumbersEntriesCount{0};
    int c_ConnectionInputRowsCount{static_cast<int>(connectionInputRows.size())};

    for (int rowIndex{0}; rowIndex < c_ConnectionInputRowsCount; ++rowIndex)
    {
        const int c_InputRowsLength{static_cast<int>(connectionInputRows[rowIndex].size())};
        const int c_MaxNrOfDevicesPerRow{2};
        int currentPosition{0}; // current position in the current input row
        int columnNumber{1}; // column number from connectioninput.csv
        int devicesStillNotParsedCount{c_MaxNrOfDevicesPerRow}; // devices that haven't been fully parsed on the current input csv row (maximum 2 - one connection)
        bool isFirstCellParsed{false}; // flag: has the cable part number been parsed on current row?

        std::string cablePartNumber; // stores the cable part number written on previous row

        while (devicesStillNotParsedCount > 0)
        {
            // total number of csv cells from the connection row (cable + 2 devices) is less than required (parsing of the row should stop at once)
            if (currentPosition == c_InputRowsLength || -1 == currentPosition)
            {
                Error* pFewerCellsError{new FewerCellsError{errorStream}};
                pFewerCellsError->setRow(rowIndex + 2);
                pFewerCellsError->setColumn(columnNumber);
                allParsingErrors.push_back(pFewerCellsError);
                break;
            }

            // the cable field should only be parsed before parsing any device on the row
            if (!isFirstCellParsed)
            {
                ++cablePartNumbersEntriesCount;
                cablePartNumbersEntries.resize(cablePartNumbersEntriesCount);
                currentPosition = readDataField(connectionInputRows[rowIndex], cablePartNumbersEntries[cablePartNumbersEntriesCount - 1], currentPosition);

                // if no cable PN entered on current row take the PN for previous row
                if (0 == cablePartNumbersEntries[cablePartNumbersEntriesCount - 1].size())
                {
                    cablePartNumbersEntries[cablePartNumbersEntriesCount - 1] = cablePartNumber;
                }
                else
                {
                    cablePartNumber = cablePartNumbersEntries[cablePartNumbersEntriesCount - 1];
                }

                isFirstCellParsed = true;
                ++columnNumber;
                continue;
            }

            Device* pDevice{nullptr};

            std::string deviceType;
            currentPosition = readDataField(connectionInputRows[rowIndex], deviceType, currentPosition);

            if (deviceType.size() > 0U)
            {
                const bool c_IsSourceDevice{0 == devicesStillNotParsedCount % c_MaxNrOfDevicesPerRow};

                pDevice = deviceFactory.createDevice(deviceType, c_IsSourceDevice);

                if (nullptr != pDevice)
                {
                    ++columnNumber;

                    pDevice->setRow(rowIndex + 2);     // +2: csv lines start at 1 and first row is ignored
                    pDevice->setColumn(columnNumber);
                    devices.push_back(pDevice);

                    std::vector<Error*> deviceParsingErrors{pDevice->parseInputData(connectionInputRows[rowIndex], currentPosition, errorStream)};

                    bool shouldStopConnectionParsing{false};

                    for(auto pError : deviceParsingErrors)
                    {
                        if (nullptr != pError)
                        {
                            // the remaining row part (second device) should no longer be parsed if there are fewer cells (in total) than necessary
                            if (nullptr != dynamic_cast<FewerCellsError*>(pError) && c_MaxNrOfDevicesPerRow == devicesStillNotParsedCount)
                            {
                                shouldStopConnectionParsing = true;
                            }

                            allParsingErrors.push_back(pError);
                        }
                    }

                    if (shouldStopConnectionParsing)
                    {
                        break;
                    }

                    columnNumber = pDevice->getColumn();
                    --devicesStillNotParsedCount;
                }
                else
                {
                    Error* pUnknownDeviceError{new UnknownDeviceError{errorStream}};
                    pUnknownDeviceError->setRow(rowIndex + 2);
                    pUnknownDeviceError->setColumn(columnNumber);
                    allParsingErrors.push_back(pUnknownDeviceError);
                    break;
                }
            }
        }
    }

    const bool c_ErrorsOccurred{allParsingErrors.size() > 0};

    for(auto& pError : allParsingErrors)
    {
        pError->execute();
        delete pError;
        pError = nullptr;
    }

    return c_ErrorsOccurred;
}

void buildLabellingOutput(std::vector<std::string>& outputRows,
                          const std::vector<Device*>& devices,
                          const std::vector<std::string>& cablePartNumbersEntries)
{
    outputRows.clear();

    const int c_DevicesCount{static_cast<int>(devices.size())};
    const int c_CablePartNumbersEntriesCount{static_cast<int>(cablePartNumbersEntries.size())};

    for (int deviceIndex{0}; deviceIndex < c_DevicesCount; ++deviceIndex)
    {
        devices[deviceIndex]->buildDescriptionText(); // for each device the description is built by considering the even/odd index (even, e.g. 0: first device on the row; odd, e.g. 3: second device on the row)
        devices[deviceIndex]->buildLabelText(); // same for labels
    }

    outputRows.resize(c_CablePartNumbersEntriesCount); // number of output rows should match the number of input rows

    int connectionNumber{1}; // number of the connection to be written on each row of the output file
    int firstDeviceIndex{0}; // index of the first device of the connection
    int secondDeviceIndex{1}; // index of the second device of the connection

    // calculate the row strings for the output file (labellingtable.csv)
    for (int connectionIndex{0}; connectionIndex < c_CablePartNumbersEntriesCount; ++connectionIndex)
    {
        buildConnectionEntry(outputRows[connectionIndex],
                             connectionNumber,
                             devices[firstDeviceIndex],
                             devices[secondDeviceIndex],
                             cablePartNumbersEntries[connectionIndex]);

        firstDeviceIndex += 2;
        secondDeviceIndex += 2;
    }
}

void buildConnectionEntry(std::string& entry, int& entryNumber, const Device* const pFirstDevice, const Device* const pSecondDevice, const std::string& cablePartNumber)
{
    std::stringstream str;

    entry.clear();
    str << entryNumber;
    str >> entry;
    entry += ',';
    entry += cablePartNumber;
    entry += ',';
    pFirstDevice->writeDescriptionAndLabel(entry);
    entry += ',';
    pSecondDevice->writeDescriptionAndLabel(entry);

    ++entryNumber;
}
