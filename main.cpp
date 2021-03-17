#include <iostream>
#include <sstream>
#include <cassert>

#include "devicefactory.h"
#include "error.h"

/* This function creates an entry for a connection between two devices.
   String is written to labellingtable.csv in a subsequent operation.
*/
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

bool init(std::string& connectionsFilename, std::string& inputFilename, std::string& outputFilename, std::string& errorFilename, std::ofstream& errorStream)
{
    using namespace std;

    bool commonFilesSuccessfullyOpened{false}; // for each option corresponding files (e.g. connectioninput.csv) should be successfully opened in the desired mode (read/write)

    ifstream inputStream{c_ConfigurationFilePath};

    if(inputStream.is_open())
    {
        // username is used for determining the paths of all other relevant files (see below)
        string username;
        getline(inputStream, username);

        const string c_AppFilesDir{c_HomeDirParent + c_PathSeparator + username + c_PathSeparator + c_DocumentsDirName + c_PathSeparator};
        connectionsFilename = c_AppFilesDir + c_ConnectionDefinitionsFilename;
        inputFilename = c_AppFilesDir + c_ConnectionInputFilename;
        outputFilename = c_AppFilesDir + c_LabellingTableFilename;
        errorFilename = c_AppFilesDir + c_ErrorFilename;

        errorStream.open(errorFilename);

        if (errorStream.is_open())
        {
            commonFilesSuccessfullyOpened = true;
        }
        else
        {
            system(c_ClearScreenCommand.c_str());
            cerr << "Error! File " << c_ErrorFilename << " cannot be opened for writing" << endl;
            cerr << "File path: " << errorFilename << endl;
        }
    }
    else
    {
        system(c_ClearScreenCommand.c_str());
        cerr << "Error! File " << c_ConfigurationFilePath << " cannot be opened for reading" << endl;
    }

    return commonFilesSuccessfullyOpened;
}

bool enableReadWriteOperations(std::ifstream& inputStream, std::ofstream& outputStream, const std::string& inputFilename, const std::string& outputFilename)
{
    using namespace std;

    bool filesSuccessfullyOpened{false};

    inputStream.open(inputFilename);

    if (inputStream.is_open())
    {
        outputStream.open(outputFilename);

        if(outputStream.is_open())
        {
            filesSuccessfullyOpened = true;
        }
        else
        {
            system(c_ClearScreenCommand.c_str());
            cerr << "Error! Output file cannot be opened for writing" << endl;
            cerr << "File path: "<< outputFilename << endl;
        }
    }
    else
    {
        system(c_ClearScreenCommand.c_str());
        std::cerr << "Error! Input file cannot be opened for reading" << std::endl;
        std::cerr << "File path: "<< inputFilename << std::endl;
    }

    return filesSuccessfullyOpened;
}

void readConnectionDefinitions(std::ifstream& inputStream, std::vector<std::string>& connectionDefinitionRows)
{
    assert(inputStream.is_open());

    connectionDefinitionRows.clear();
    int connectionDefinitionRowsCount{0};

    while (!inputStream.eof() && connectionDefinitionRowsCount < c_MaxNrOfRackUnits)
    {
        ++connectionDefinitionRowsCount;
        connectionDefinitionRows.resize(connectionDefinitionRowsCount);
        getline(inputStream,connectionDefinitionRows[connectionDefinitionRowsCount - 1]);
    }
}

bool parseConnectionDefinitions(std::ofstream& errorStream,
                                std::vector<std::string>& mapping,
                                std::vector<int>& uNumbers,
                                std::vector<std::vector<int>>& connectedTo,
                                std::vector<std::vector<int>>& connectionsCount,
                                int& devicesCount,
                                const std::vector<std::string>& connectionDefinitionRows)
{
    mapping.clear();
    uNumbers.clear();
    connectedTo.clear();
    connectionsCount.clear();
    devicesCount = 0;

    assert(errorStream.is_open());

    mapping.resize(c_MaxNrOfRackUnits, c_NoDevice); // initial value: no device

    std::vector<Error*> parsingErrors;

    const int c_ConnectionDefinitionRowsCount{static_cast<int>(connectionDefinitionRows.size())};

    for (int rowIndex{0}; rowIndex < c_ConnectionDefinitionRowsCount; ++rowIndex)
    {
        int columnNumber{1}; // csv column
        std::string currentCell; // current csv cell

        // first cell on the row is ignored (contains the U number and is only used for informing the user about rack position; the row index is instead used in calculations in relationship with U number)
        int currentPosition{readDataField(connectionDefinitionRows[rowIndex],currentCell,0)};

        // second cell on the row: device type
        currentPosition = readDataField(connectionDefinitionRows[rowIndex],currentCell,currentPosition);

        if (0 == currentCell.size())
        {
            continue;
        }

        ++columnNumber;

        if (!DeviceFactory::isDeviceTypeValid(currentCell))
        {
            Error* pError{new UnknownDeviceError{errorStream}};
            pError->setRow(rowIndex + 2); // setting row index of the current cell (+2 deoarece in Excel (.csv) the rows start at 1 and first line is ignored);
            pError->setColumn(columnNumber); // setting column index for exact error localization

            parsingErrors.push_back(pError);

            continue;
        }

        // add discovered device to list of device U numbers
        ++devicesCount;
        uNumbers.push_back(c_MaxNrOfRackUnits - rowIndex); // add the U number of the last discovered device

        // add device type to mapping table
        mapping[c_MaxNrOfRackUnits - 1 - rowIndex] = currentCell;

        // adjust vectors of connected devices and number of connections between each two devices
        connectedTo.resize(devicesCount);
        connectionsCount.resize(devicesCount);

        // read each device to which current device is connected, go to next row if no (more) connected devices are found
        while(currentPosition > -1)
        {
            // read next cell (new current cell)
            ++columnNumber;
            currentPosition = readDataField(connectionDefinitionRows[rowIndex],currentCell,currentPosition);

            if (0 == currentCell.size())
            {
                break;
            }

            int secondDevice;
            int nrOfconnections;
            const bool isConnectionFormattingInvalid{!parseConnectionFormatting(currentCell,secondDevice,nrOfconnections)};

            Error* pError{nullptr};

            if(isConnectionFormattingInvalid) // checking if the connection format is correct (e.g. 20/3: 3 connections to device located at U20)
            {
                pError = new WrongFormatError{errorStream};
            }
            else if (secondDevice <= 0 || secondDevice > c_MaxNrOfRackUnits) // checking if the device is in the accepted U interval within rack
            {
                pError = new WrongUNumberError{errorStream};
            }
            else if (c_NoDevice == mapping[secondDevice - 1]) // check if the second device is actually placed within rack (contained in mapping table)
            {
                pError = new NoDevicePresentError{errorStream};
            }
            else if (c_MaxNrOfRackUnits - rowIndex == secondDevice) // connection of a device to itself (connection loop) is not allowed
            {
                pError = new DeviceConnectedToItselfError{errorStream};
            }
            else if (0 == nrOfconnections) // if the devices are marked as connected there should be minimum 1 connection between them
            {
                pError = new NoConnectionsError{errorStream};
            }
            else
            {
                connectedTo[devicesCount - 1].resize(columnNumber - 2); // add the device to the list of connected devices (to current device)
                connectionsCount[devicesCount - 1].resize(columnNumber - 2); // same for the number of connections
                connectedTo[devicesCount - 1][columnNumber - 3] = secondDevice; // add the U number of the second device
                connectionsCount[devicesCount - 1][columnNumber - 3] = nrOfconnections; // add the number of connections between the current device and the second device
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

void buildConnectionsInputTemplate(std::vector<std::string>& outputRows,
                                   const std::vector<std::string>& mapping,
                                   const std::vector<int>& uNumbers,
                                   const std::vector<std::vector<int>>& connectedTo,
                                   const std::vector<std::vector<int>>& connectionsCount,
                                   const int devicesCount)
{
    using namespace std;

    outputRows.clear();

    /* Used for calculating the rows to be written into connectioninput.csv
       Each string represents the first device from each row, namely: device type, device name (lowest U occupied in rack), device parameters (e.g. port number)
    */
    vector<string> deviceParameters;

    deviceParameters.resize(c_MaxNrOfRackUnits); // resize so it has same size as the mapping vector

    // uNumbers is traversed starting with the device placed at highest U position within rack
    for (int deviceIndex{devicesCount-1}; deviceIndex >= 0; --deviceIndex)
    {
        stringstream str;
        string s;
        int currentDeviceUPosition = uNumbers[deviceIndex] - 1; // in mapping vector numbering starts at 0 so it is necessary to decrease the U number by 1
        deviceParameters[currentDeviceUPosition] += mapping[currentDeviceUPosition]+','; // appending device type
        str << currentDeviceUPosition + 1; // recover the original U number (real position in rack)
        str >> s;
        deviceParameters[currentDeviceUPosition] += s + ',';
        createPlaceholders(mapping[currentDeviceUPosition], deviceParameters[currentDeviceUPosition]); // add the placeholders for the device parameters (to be filled in the next step (option 2) in connectioninput.csv so the final table can be calculated)
    }

    int outputRowsCount{0};

    // traverse the uNumbers vector again to check if each device is connected to devices placed in an upper U position
    for (int currentDeviceIndex{devicesCount-1}; currentDeviceIndex >= 0; --currentDeviceIndex)
    {
        int currentDeviceUPosition{uNumbers[currentDeviceIndex] - 1};
        int connectedToLength{static_cast<int>(connectedTo[currentDeviceIndex].size())};

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
            string output{c_CablePartNumberPlaceholder + "," + deviceParameters[currentDeviceUPosition] + "," + deviceParameters[connectedTo[currentDeviceIndex][connectedDeviceIndex] - 1]};
            outputRowsCount += connectionsCount[currentDeviceIndex][connectedDeviceIndex];
            outputRows.resize(outputRowsCount);

            // write the resulting output string a number of times equal to the number of connections between the two devices
            for (int connectionNumber{outputRowsCount - connectionsCount[currentDeviceIndex][connectedDeviceIndex]}; connectionNumber < outputRowsCount; connectionNumber++)
            {
                outputRows[connectionNumber] = output;
            }
        }
    }
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

void writeOutputToFile(std::ofstream& outputStream, const std::vector<std::string>& connectionInputRows, const std::string& header)
{
    assert(outputStream.is_open());

    outputStream << header << std::endl;

    for (const auto& payloadRow : connectionInputRows)
    {
        outputStream << payloadRow << std::endl;
    }
}

void displayErrorMessage(const std::string& inputFilename, const std::string& errorFilename)
{
    using namespace std;

    system(c_ClearScreenCommand.c_str());

    cerr << "One or more errors occured!" << endl << endl;
    cerr << "Please check the error report in the error file: " << endl << endl;
    cerr << errorFilename << endl << endl;
    cerr << "Please correct the input file and then try again" << endl << endl;
    cerr << "Input file: " << endl << endl << inputFilename << endl << endl;
    cerr << "Thank you for using LabelCalculator!" << endl << endl;
}

void displaySuccessMessage(const std::string& outputFilename, bool displayFurtherInstructions)
{
    using namespace std;

    system(c_ClearScreenCommand.c_str());

    cout << "The program ended succesfully. " << endl << endl;
    cout << "Please view the output file: " << endl << endl;
    cout << outputFilename << endl << endl;

    if (displayFurtherInstructions)
    {
        cout << "Please go on with the next step by filling in the connectioninput.csv file with the needed info" << endl;
    }

    cout << "Thank you for using LabelCalculator!" << endl << endl;
}

void displayAbortMessage()
{
    system(c_ClearScreenCommand.c_str());
    std::cout << "Application terminated by user" << std::endl << std::endl;
}

void displayMenu()
{
    using namespace std;

    cout << "Please choose between following options:" << endl << endl;
    cout << "1 + ENTER: read the defined connections from file connectiondefinitions.csv and write the partial input data into file connectioninput.csv" << endl;
    cout << "2 + ENTER: read the input data from file connectioninput.csv and write the labeling information into file labellingtable.csv" << endl << endl;
    cout << "Choose any other combination + ENTER or directly press ENTER to exit the application" << endl << endl;
}

void displayVersion()
{
    system(c_ClearScreenCommand.c_str());
    std::cout << "LabelCalculator v1.0" << std::endl << std::endl;
}

int main()
{
    using namespace std;

    ofstream errorStream; // for writing into error.csv

    string connectionsFilename; // connectiondefinitions.csv (for each device it contains connected devices and number of connections)
    string inputFilename; // connectioninput.csv (contains each connection generated based on connectiondefinitions.csv - user should replace placeholders by filling in the required data)
    string outputFilename; // labellingtable.csv (contains labelling table generated based on connectioninput.csv)
    string errorFilename; // error.csv (contains any errors that occured when choosing option 1 or 2)

    /* DETERMINE PATH OF THE FILES AND PROVIDING USER THE AVAILABLE OPTIONS */
    displayVersion();

    bool commonFilesSuccessfullyOpened{init(connectionsFilename, inputFilename, outputFilename, errorFilename, errorStream)};

    if (commonFilesSuccessfullyOpened)
    {
        displayMenu();

        string option;
        getline(cin, option);

        if ("1" == option) /* OPTION 1 + ENTER: READ connectiondefinitions.csv AND HANDLE ANY ERRORS; build connectioninput.csv content (including placeholders) and write it into file */
        {
            ifstream inputStream; // for reading from connectiondefinitions.csv
            ofstream outputStream; // for writing into connectioninput.csv

            bool firstOptionFilesSuccessfullyOpened{enableReadWriteOperations(inputStream, outputStream, connectionsFilename, inputFilename)};

            if (firstOptionFilesSuccessfullyOpened)
            {
                string header;
                getline(inputStream, header); // the header is not used further

                vector<string> connectionDefinitionRows; // stores rows read from connectiondefinitions.csv

                // maximum 50 lines are read from connectiondefinitions.csv (the rack can have maximum 50U)
                readConnectionDefinitions(inputStream, connectionDefinitionRows);

                /* Stores devices contained in the rack.
                   For each device the type will be memorized at the index representing the lowest U position occupied within rack.
                   The vector will have 50 elements (maximum rack size).
                */
                vector<string> mapping;

                /* Stores the U positions (lowest in rack, e.g. U5 for a device occupying U5-10) of all discovered devices (in the order of their discovery)
                   Storing occurs in decreasing order of their appearance (first device is the one from the highest U position)
                */
                vector<int> uNumbers;

                vector<vector<int>> connectedTo; // stores the devices to which each device from a csv line connects
                vector<vector<int>> connectionsCount; // stores the number of connections to each device mentioned in previous vector

                int devicesCount; // total number of devices discovered in the rack

                bool parsingErrorsOccured{parseConnectionDefinitions(errorStream, mapping, uNumbers, connectedTo, connectionsCount, devicesCount, connectionDefinitionRows)};

                if (!parsingErrorsOccured)
                {
                    vector<string> connectionInputRows; // stores rows to be written to connectioninput.csv

                    buildConnectionsInputTemplate(connectionInputRows, mapping, uNumbers, connectedTo, connectionsCount, devicesCount);
                    writeOutputToFile(outputStream, connectionInputRows, c_InputHeader);
                    displaySuccessMessage(inputFilename, true);
                }
                else
                {
                    displayErrorMessage(connectionsFilename, errorFilename);
                }
            }
        }
        else if ("2" == option) /* OPTION 2 + ENTER: READ connectioninput.csv, CHECK ERRORS and write final output to labellingtable.csv */
        {
            ifstream inputStream; // for reading from connectioninput.csv
            ofstream outputStream; //for writing into labellingtable.csv

            bool secondOptionFilesSuccessfullyOpened{enableReadWriteOperations(inputStream, outputStream, inputFilename, outputFilename)};

            if (secondOptionFilesSuccessfullyOpened)
            {
                vector<string> connectionInputRows; // stores rows read from connectioninput.csv

                readConnectionInput(inputStream, connectionInputRows);

                vector<Device*> devices; // all created Device objects (except the ones used for checking device validity which are destroyed immediately)
                vector<string> cablePartNumbersEntries; // the cable part number of each connection to be stored here

                bool parsingErrorsOccured{parseConnectionInput(errorStream, devices, cablePartNumbersEntries, connectionInputRows)};

                assert(cablePartNumbersEntries.size() == connectionInputRows.size());

                if(!parsingErrorsOccured)
                {
                    vector<string> outputRows; // stores rows to be written to labellingtable.csv

                    buildLabellingOutput(outputRows, devices, cablePartNumbersEntries);
                    writeOutputToFile(outputStream, outputRows, c_OutputHeader);
                    displaySuccessMessage(outputFilename, false);
                }
                else
                {
                    displayErrorMessage(inputFilename, errorFilename);
                }

                //TODO: implement smart pointers
                for(auto& pDevice : devices)
                {
                    if (nullptr != pDevice)
                    {
                        delete pDevice;
                        pDevice = nullptr;
                    }
                }
            }
        }
        else
        {
            displayAbortMessage();
        }
    }

    return 0;
}
