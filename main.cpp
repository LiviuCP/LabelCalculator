#include <iostream>
#include <cassert>

#include "connectiondefinitionparser.h"
#include "connectioninputparser.h"
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
                ConnectionDefinitionParser connectionDefinitionsParser{&inputStream, &outputStream, &errorStream};
                const bool c_ParsingErrorsOccurred{connectionDefinitionsParser.parse()};

                if (!c_ParsingErrorsOccurred)
                {
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
                ConnectionInputParser connectionInputParser{&inputStream, &outputStream, &errorStream};
                const bool c_ParsingErrorsOccurred{connectionInputParser.parse()};

                if (!c_ParsingErrorsOccurred)
                {
                    displaySuccessMessage(outputFilename, false);
                }
                else
                {
                    displayErrorMessage(inputFilename, errorFilename);
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
