#include <iostream>
#include <cassert>

#include "parsercreator.h"
#include "parser.h"
#include "labelutils.h"
#include "displayutils.h"

int main()
{
    using namespace std;

    ofstream errorStream;       // for writing into error.txt

    string connectionsFilename; // connectiondefinitions.csv (for each device it contains connected devices and number of connections)
    string inputFilename;       // connectioninput.csv (contains each connection generated based on connectiondefinitions.csv - user should replace placeholders by filling in the required data)
    string outputFilename;      // labellingtable.csv (contains labelling table generated based on connectioninput.csv)
    string errorFilename;       // error.txt (contains any errors that occured when choosing option 1 or 2)

    displayVersion();

    bool commonFilesSuccessfullyOpened{init(connectionsFilename, inputFilename, outputFilename, errorFilename, errorStream)};

    if (commonFilesSuccessfullyOpened)
    {
        displayMenu();

        string option;
        getline(cin, option);

        ParserCreator::ParserTypes parserType{ParserCreator::ParserTypes::UNKNOWN};
        bool displayFurtherInstructions{false};

        if ("1" == option)
        {
            parserType = ParserCreator::ParserTypes::CONNECTION_DEFINITION;
            displayFurtherInstructions = true;
        }
        else if ("2" == option)
        {
            parserType = ParserCreator::ParserTypes::CONNECTION_INPUT;
        }
        else
        {
            displayAbortMessage();
        }

        if (ParserCreator::ParserTypes::UNKNOWN != parserType)
        {
            const string c_InFile{ParserCreator::ParserTypes::CONNECTION_DEFINITION == parserType ? connectionsFilename : inputFilename};
            const string c_OutFile{ParserCreator::ParserTypes::CONNECTION_DEFINITION == parserType ? inputFilename : outputFilename};
            ifstream inputStream;
            ofstream outputStream;

            const bool inOutFilesSuccessfullyOpened{enableReadWriteOperations(inputStream, outputStream, c_InFile, c_OutFile)};

            if (inOutFilesSuccessfullyOpened)
            {
                ParserCreator parserCreator;
                assert(!parserCreator.isParserAlreadyCreated());

                ParserPtr pParser{parserCreator.createParser(parserType, &inputStream, &outputStream, &errorStream)};

                if (nullptr != pParser)
                {
                    const bool c_ParsingErrorsOccurred{pParser->parse()};

                    if (!c_ParsingErrorsOccurred)
                    {
                        displaySuccessMessage(c_OutFile, displayFurtherInstructions);
                    }
                    else
                    {
                        displayErrorMessage(c_InFile, errorFilename);
                    }
                }
            }
        }
    }

    return 0;
}
