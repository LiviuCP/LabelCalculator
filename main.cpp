#include <iostream>
#include <cassert>

#include "connectiondefinition.h"
#include "connectioninput.h"
#include "labelutils.h"
#include "displayutils.h"

int main()
{
    using namespace std;

    ofstream errorStream; // for writing into error.txt

    string connectionsFilename; // connectiondefinitions.csv (for each device it contains connected devices and number of connections)
    string inputFilename; // connectioninput.csv (contains each connection generated based on connectiondefinitions.csv - user should replace placeholders by filling in the required data)
    string outputFilename; // labellingtable.csv (contains labelling table generated based on connectioninput.csv)
    string errorFilename; // error.txt (contains any errors that occured when choosing option 1 or 2)

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
