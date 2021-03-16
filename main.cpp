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

bool init(std::string& connectionsFilename, std::string& inputFilename, std::string& outputFilename, std::ofstream& writeToOutput)
{
    using namespace std;

    bool commonFilesSuccessfullyOpened{false}; // for each option corresponding files (e.g. connectioninput.csv) should be successfully opened in the desired mode (read/write)

    ifstream readInput{c_ConfigurationFilePath};

    if(readInput.is_open())
    {
        // username is used for determining the paths of all other relevant files (see below)
        string username;
        getline(readInput, username);

        const string c_AppFilesDir{c_HomeDirParent + c_PathSeparator + username + c_PathSeparator + c_DocumentsDirName + c_PathSeparator};
        connectionsFilename = c_AppFilesDir + c_ConnectionDefinitionsFilename;
        inputFilename = c_AppFilesDir + c_ConnectionInputFilename;
        outputFilename = c_AppFilesDir + c_LabellingTableFilename;

        writeToOutput.open(outputFilename);

        // labellingtable.csv should be opened from the very beginning as any errors for any chosen option are written here! (TODO: create dedicated error file)
        if (writeToOutput.is_open())
        {
            commonFilesSuccessfullyOpened = true;
        }
        else
        {
            system(c_ClearScreenCommand.c_str());
            cerr << "Error! File " << c_LabellingTableFilename << " cannot be opened for writing" << endl;
            cerr << "File path: " << outputFilename << endl;
        }
    }
    else
    {
        system(c_ClearScreenCommand.c_str());
        cerr << "Error! File " << c_ConfigurationFilePath << " cannot be opened for reading" << endl;
    }

    return commonFilesSuccessfullyOpened;
}

bool enableReadingConnectionDefinitions(std::ifstream& readConnections, std::ofstream& writeToInput, const std::string& connectionsFilename, const std::string& inputFilename)
{
    using namespace std;

    bool filesSuccessfullyOpened{false};

    readConnections.open(connectionsFilename);

    if (readConnections.is_open())
    {
        writeToInput.open(inputFilename);

        if (writeToInput.is_open())
        {
            filesSuccessfullyOpened = true;
        }
        else
        {
            system(c_ClearScreenCommand.c_str());
            cerr << "Error! File " << c_ConnectionInputFilename << " cannot be opened for writing" << endl;
            cerr << "File path: "<< inputFilename << endl;
        }
    }
    else
    {
        system(c_ClearScreenCommand.c_str());
        cerr << "Error! File  " << c_ConnectionDefinitionsFilename << " cannot be opened for reading" << endl;
        cerr << "File path: " << connectionsFilename << endl;
    }

    return filesSuccessfullyOpened;
}

bool enableReadingConnectionInput(std::ifstream& readInput, const std::string& inputFilename)
{
    bool filesSuccessfullyOpened{false};

    readInput.open(inputFilename);

    if (readInput.is_open())
    {
        filesSuccessfullyOpened = true;
    }
    else
    {
        system(c_ClearScreenCommand.c_str());
        std::cerr << "Error! File " << c_ConnectionInputFilename << " cannot be opened for reading" << std::endl;
        std::cerr << "File path: "<< inputFilename << std::endl;
    }

    return filesSuccessfullyOpened;
}

void readConnectionDefinitions(std::ifstream& readConnections, std::vector<std::string>& connectionDefinitionRows, int& connectionDefinitionRowsCount)
{
    assert(readConnections.is_open());

    connectionDefinitionRows.clear();
    connectionDefinitionRowsCount = 0;

    while (!readConnections.eof() && connectionDefinitionRowsCount < c_MaxNrOfRackUnits)
    {
        ++connectionDefinitionRowsCount;
        connectionDefinitionRows.resize(connectionDefinitionRowsCount);
        getline(readConnections,connectionDefinitionRows[connectionDefinitionRowsCount-1]);
    }

    readConnections.close();
}

bool parseConnectionDefinitions(std::ofstream& writeToError,
                                std::vector<std::string>& mapping,
                                std::vector<int>& uNumbers,
                                std::vector<std::vector<int>>& connectedTo,
                                std::vector<std::vector<int>>& connectionsCount,
                                int& devicesCount,
                                const std::vector<std::string>& connectionDefinitionRows,
                                const int connectionDefinitionRowsCount)
{
    mapping.clear();
    uNumbers.clear();
    connectedTo.clear();
    connectionsCount.clear();
    devicesCount = 0;

    assert(writeToError.is_open());

    mapping.resize(c_MaxNrOfRackUnits, c_NoDevice); // initial value: no device

    std::vector<Error*> parsingErrors;

    // each connection.csv row is parsed and checked for errors
    for (int rowIndex{0}; rowIndex < connectionDefinitionRowsCount; ++rowIndex)
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
            Error* pError{new UnknownDeviceError{writeToError}};
            pError->setRow(rowIndex + 2); // setting row index of the current cell (+2 deoarece in Excel (.csv) the rows start at 1 and first line is ignored);
            pError->setColumn(columnNumber); // setting column index for exact error localization

            parsingErrors.push_back(pError);

            continue;
        }

        // add discovered device to list of device U numbers
        ++devicesCount;
        uNumbers.resize(devicesCount);
        uNumbers[devicesCount - 1] = c_MaxNrOfRackUnits - rowIndex; // add the U number of the last discovered device

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

            int secondDevice;    // temporarily stores each device that is connected to the current device
            int nrOfconnections; // temporarily stores the number of connections of each device to the current one
            const bool isConnectionFormattingInvalid{!parseConnectionFormatting(currentCell,secondDevice,nrOfconnections)};

            Error* pError{nullptr};

            if(isConnectionFormattingInvalid) // checking if the connection format is correct
            {
                pError = new WrongFormatError{writeToError};
            }
            else if (secondDevice <= 0 || secondDevice > c_MaxNrOfRackUnits) // checking if the device is in the accepted U interval within rack
            {
                pError = new WrongUNumberError{writeToError};
            }
            else if (c_NoDevice == mapping[secondDevice - 1]) // check if the second device is in the mapping table (otherwise the connection is to a non-existing device)
            {
                pError = new NoDevicePresentError{writeToError};
            }
            else if (c_MaxNrOfRackUnits - rowIndex == secondDevice) // connection of a device to itself is not allowed
            {
                pError = new DeviceConnectedToItselfError{writeToError};
            }
            else if (0 == nrOfconnections) // if the devices are marked as connected there should be minimum 1 connection between them
            {
                pError = new NoConnectionsError{writeToError};
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

void readConnectionInput(std::ifstream& readInput, std::vector<std::string>& connectionInputRows, int& connectionInputRowsCount)
{
    assert(readInput.is_open());

    connectionInputRows.clear();
    connectionInputRowsCount = 0;

    readInput.seekg(0);
    std::string header;
    getline(readInput, header);

    while (!readInput.eof())
    {
        ++connectionInputRowsCount;
        connectionInputRows.resize(connectionInputRowsCount);
        getline(readInput,connectionInputRows[connectionInputRowsCount-1]);
    }

    readInput.close();

    // discard last empty row read from the input file if payload exists (trim)
    if(connectionInputRowsCount > 1 && 0 == connectionInputRows[connectionInputRowsCount - 1].size())
    {
        --connectionInputRowsCount;
        connectionInputRows.resize(connectionInputRowsCount);
    }
}

bool parseConnectionInput(std::ofstream& writeToError,
                          std::vector<Device*>& devices,
                          std::vector<std::string>& cablePartNumbersEntries,
                          int& numberOfDevices,
                          int& cablePartNumbersEntriesCount,
                          const std::vector<std::string>& connectionInputRows,
                          const int connectionInputRowsCount)
{
    std::vector<Error*> allParsingErrors; // gather all parsing errors here and write them to output file once parsing is complete (if any errors)

    devices.clear();
    cablePartNumbersEntries.clear();
    cablePartNumbersEntriesCount = 0;

    assert(writeToError.is_open());

    DeviceFactory deviceFactory;

    for (int rowIndex{0}; rowIndex < connectionInputRowsCount; ++rowIndex)
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
                Error* pFewerCellsError{new FewerCellsError{writeToError}};
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

                    std::vector<Error*> deviceParsingErrors{pDevice->parseInputData(connectionInputRows[rowIndex], currentPosition, writeToError)};

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
                    Error* pUnknownDeviceError{new UnknownDeviceError{writeToError}};
                    pUnknownDeviceError->setRow(rowIndex + 2);
                    pUnknownDeviceError->setColumn(columnNumber);
                    allParsingErrors.push_back(pUnknownDeviceError);
                    break;
                }
            }
        }
    }

    numberOfDevices = deviceFactory.getCreatedDevicesCount();

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
                                   int& outputRowsCount,
                                   const std::vector<std::string>& mapping,
                                   const std::vector<int>& uNumbers,
                                   const std::vector<std::vector<int>>& connectedTo,
                                   const std::vector<std::vector<int>>& connectionsCount,
                                   const int devicesCount)
{
    using namespace std;

    outputRows.clear();
    outputRowsCount = 0;

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
            outputRowsCount += connectionsCount[currentDeviceIndex][connectedDeviceIndex]; //se actualizeaza dimensiunea vectorului out_ln
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
                          const std::vector<std::string>& cablePartNumbersEntries,
                          const int numberOfDevices,
                          const int connectionInputRowsCount)
{
    outputRows.clear();

    for (int deviceIndex{0}; deviceIndex < numberOfDevices; ++deviceIndex)
    {
        devices[deviceIndex]->buildDescriptionText(); // for each device the description is built by considering the even/odd index (even, e.g. 0: first device on the row; odd, e.g. 3: second device on the row)
        devices[deviceIndex]->buildLabelText(); // same for labels
    }

    outputRows.resize(connectionInputRowsCount); // number of output rows should match the number of input rows

    int connectionNumber{1}; // number of the connection to be written on each row of the output file
    int firstDeviceIndex{0}; // index of the first device of the connection
    int secondDeviceIndex{1}; // index of the second device of the connection

    // calculate the row strings for the output file (labellingtable.csv)
    for (int connectionIndex{0}; connectionIndex < connectionInputRowsCount; ++connectionIndex)
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

void writeOutputToFile(std::ofstream& output, const std::vector<std::string>& connectionInputRows, const int payloadRowsCount, const std::string& header)
{
    assert(output.is_open());

    output << header << std::endl;

    for (int rowIndex{0}; rowIndex < payloadRowsCount; ++rowIndex)
    {
        output << connectionInputRows[rowIndex] << std::endl;
    }
}

void displayErrorMessage(const std::string& inputFilename, const std::string& outputFilename)
{
    using namespace std;

    system(c_ClearScreenCommand.c_str());

    cout << "One or more errors occured!" << endl << endl;
    cout << "Please check the error report in the output file: " << endl << endl; //TODO: create error.csv for errors (instead of labellingtable.csv)
    cout << outputFilename << endl << endl;
    cout << "Please correct the input file and then try again" << endl << endl;
    cout << "Input file: " << endl << endl << inputFilename << endl << endl;
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

    ifstream readInput; // for reading from connectioninput.csv and configuration.txt
    ofstream writeToOutput; //for writing into labellingtable.csv

    string connectionsFilename; // connectiondefinitions.csv (for each device it contains connected devices and number of connections)
    string inputFilename; // connectioninput.csv (contains each connection generated based on connectiondefinitions.csv - user should replace placeholders by filling in the required data)
    string outputFilename; // labellingtable.csv (contains labelling table generated based on connectioninput.csv or any errors that occured when choosing option 1 or 2)

    /* DETERMINE PATH OF THE FILES AND PROVIDING USER THE AVAILABLE OPTIONS */
    displayVersion();

    bool commonFilesSuccessfullyOpened{init(connectionsFilename, inputFilename, outputFilename, writeToOutput)};

    if (commonFilesSuccessfullyOpened)
    {
        displayMenu();

        string option;
        getline(cin, option);

        if ("1" == option) /* OPTION 1 + ENTER: READ connectiondefinitions.csv AND HANDLE ANY ERRORS; build connectioninput.csv content (including placeholders) and write it into file */
        {
            ifstream readConnections; // for reading from connectiondefinitions.csv
            ofstream writeToInput; // for writing into connectioninput.csv

            bool firstOptionFilesSuccessfullyOpened{enableReadingConnectionDefinitions(readConnections, writeToInput, connectionsFilename, inputFilename)};

            if (firstOptionFilesSuccessfullyOpened)
            {
                string header;
                getline(readConnections, header); // the header is not used further

                vector<string> connectionDefinitionRows; // stores rows read from connectiondefinitions.csv
                int connectionDefinitionRowsCount; // stores the current number of elements of the connectionDefinitionRows vector

                // maximum 50 lines are read from connectiondefinitions.csv (the rack can have maximum 50U)
                readConnectionDefinitions(readConnections, connectionDefinitionRows, connectionDefinitionRowsCount);

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

                bool parsingErrorsOccured{parseConnectionDefinitions(writeToOutput, mapping, uNumbers, connectedTo, connectionsCount, devicesCount, connectionDefinitionRows, connectionDefinitionRowsCount)};

                if (!parsingErrorsOccured)
                {
                    vector<string> connectionInputRows; // stores rows to be written to connectioninput.csv
                    int connectionInputRowsCount; // dimension of the vector containing the output strings to be written into connectioninput.csv

                    buildConnectionsInputTemplate(connectionInputRows, connectionInputRowsCount, mapping, uNumbers, connectedTo, connectionsCount, devicesCount);
                    writeOutputToFile(writeToInput, connectionInputRows, connectionInputRowsCount, c_InputHeader);
                    displaySuccessMessage(inputFilename, true);
                }
                else
                {
                    displayErrorMessage(connectionsFilename, outputFilename);
                }

                writeToInput.close();
                writeToOutput.close();
            }
        }
        else if ("2" == option) /* OPTION 2 + ENTER: READ connectioninput.csv, CHECK ERRORS and write final output to labellingtable.csv */
        {
            bool secondOptionFilesSuccessfullyOpened{enableReadingConnectionInput(readInput, inputFilename)};

            if (secondOptionFilesSuccessfullyOpened)
            {
                vector<string> connectionInputRows; // stores rows read from connectioninput.csv
                int connectionInputRowsCount; // stores the current number of elements of the connectionInputRows vector; final value will be used as the number of output rows (if no errors occured)

                readConnectionInput(readInput, connectionInputRows, connectionInputRowsCount);

                vector<Device*> devices; // all created Device objects (except the ones used for checking device validity which are destroyed immediately)
                vector<string> cablePartNumbersEntries; // the cable part number of each connection to be stored here
                int numberOfDevices; // stores the current number of elements of the devices vector
                int cablePartNumbersEntriesCount; // stores the current number of elements of the cablePartNumbersEntries vector

                bool parsingErrorsOccured{parseConnectionInput(writeToOutput, devices, cablePartNumbersEntries, numberOfDevices, cablePartNumbersEntriesCount, connectionInputRows, connectionInputRowsCount)};

                if(!parsingErrorsOccured)
                {
                    vector<string> outputRows; // stores rows to be written to labellingtable.csv

                    buildLabellingOutput(outputRows, devices, cablePartNumbersEntries, numberOfDevices, connectionInputRowsCount);

                    assert(connectionInputRowsCount == static_cast<int>(outputRows.size()));

                    writeOutputToFile(writeToOutput, outputRows, connectionInputRowsCount, c_OutputHeader);
                    displaySuccessMessage(outputFilename, false);
                }
                else
                {
                    displayErrorMessage(inputFilename, outputFilename);
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

                readInput.close();
                writeToOutput.close();
            }
        }
        else
        {
            system(c_ClearScreenCommand.c_str());
            cout << "Application terminated by user" << endl << endl;
        }

        cout << "Thank you for using LabelCalculator!" << endl << endl;
    }

    return 0;
}
