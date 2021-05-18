#include <iostream>
#include <cassert>

#include "parser.h"
#include "application.h"

std::shared_ptr<Application> Application::s_pApplication = nullptr;

std::shared_ptr<Application> Application::getInstance()
{
    if (nullptr == s_pApplication)
    {
        s_pApplication.reset(new Application);
    }

    return s_pApplication;
}

int Application::run()
{
    using namespace std;

    ReturnCode result{ReturnCode::FILE_NOT_OPENED};

    if (mIsInitialized)
    {
        _displayVersion();
        _displayMenu();
        _handleUserInput();

        if (ParserCreator::ParserTypes::UNKNOWN != mParserType)
        {
            _enableCSVParsing();

            if (mIsCSVParsingEnabled)
            {
                ParserCreator parserCreator;
                assert(!parserCreator.isParserAlreadyCreated());

                ParserPtr pParser{parserCreator.createParser(mParserType, &mInputStream, &mOutputStream, &mErrorStream)};

                if (nullptr != pParser)
                {
                    const bool c_ParsingErrorsOccurred{pParser->parse()};

                    if (!c_ParsingErrorsOccurred)
                    {
                        const bool c_DisplayFurtherInstructions{ParserCreator::ParserTypes::CONNECTION_DEFINITION == mParserType ? true : false};
                        _displaySuccessMessage(c_DisplayFurtherInstructions);
                        result = ReturnCode::SUCCESS;
                    }
                    else
                    {
                        _displayParsingErrorMessage();
                        result = ReturnCode::PARSING_ERROR;
                    }
                }
                else
                {
                    result = ReturnCode::PARSER_NOT_CREATED;
                }
            }
        }
    }

    const int returnCode{static_cast<int>(result)};
    return returnCode;
}

Application::Application()
    : mParserType{ParserCreator::ParserTypes::UNKNOWN}
    , mIsInitialized{false}
    , mIsCSVParsingEnabled{false}
{
    _init();
}

void Application::_init()
{
    using namespace std;

    assert(!mIsCSVParsingEnabled);

    if (!mIsInitialized)
    {
        ifstream inputStream{c_ConfigurationFile};

        if(inputStream.is_open())
        {
            // username is used for determining the paths of all other relevant files (see below)
            string username;
            getline(inputStream, username);

            const string c_AppFilesDir{c_HomeDirParent + c_PathSeparator + username + c_PathSeparator + c_DocumentsDirName + c_PathSeparator};
            mConnectionDefinitionsFile = c_AppFilesDir + c_ConnectionDefinitionsFilename;
            mConnectionInputFile = c_AppFilesDir + c_ConnectionInputFilename;
            mLabellingOutputFile = c_AppFilesDir + c_LabellingTableFilename;
            mParsingErrorsFile = c_AppFilesDir + c_ErrorFilename;

            mErrorStream.open(mParsingErrorsFile);

            if (mErrorStream.is_open())
            {
                mIsInitialized = true;
            }
            else
            {
                _displayFileOpeningErrorMessage(FileType::ERROR);
            }
        }
        else
        {
            _displayFileOpeningErrorMessage(FileType::CONFIGURATION);
        }
    }
}

void Application::_enableCSVParsing()
{
    using namespace std;

    assert(mIsInitialized);
    assert(ParserCreator::ParserTypes::UNKNOWN != mParserType);

    if (!mIsCSVParsingEnabled)
    {
        const string c_InFile{_getInputFile()};
        const string c_OutFile{_getOutputFile()};

        mInputStream.open(c_InFile);

        if (mInputStream.is_open())
        {
            mOutputStream.open(c_OutFile);

            if(mOutputStream.is_open())
            {
                mIsCSVParsingEnabled = true;
            }
            else
            {
                _displayFileOpeningErrorMessage(FileType::OUTPUT);
            }
        }
        else
        {
            _displayFileOpeningErrorMessage(FileType::INPUT);
        }
    }
}

void Application::_handleUserInput()
{
    std::string option;
    getline(std::cin, option);

    if ("1" == option)
    {
        mParserType = ParserCreator::ParserTypes::CONNECTION_DEFINITION;
    }
    else if ("2" == option)
    {
        mParserType = ParserCreator::ParserTypes::CONNECTION_INPUT;
    }
    else
    {
        _displayAbortMessage();
    }
}

void Application::_displaySuccessMessage(bool additionalOutputRequired)
{
    using namespace std;

    const string c_OutputFile{_getOutputFile()};

    system(c_ClearScreenCommand.c_str());
    cout << "The program ended succesfully. " << endl << endl;
    cout << "Please view the output file: " << endl << endl;
    cout << c_OutputFile << endl << endl;

    if (additionalOutputRequired)
    {
        cout << "Please go on with the next step by filling in the connectioninput.csv file with the needed info" << endl << endl;
    }

    cout << "Thank you for using LabelCalculator!" << endl << endl;
}

void Application::_displayParsingErrorMessage()
{
    using namespace std;

    const string c_InputFile{_getInputFile()};

    system(c_ClearScreenCommand.c_str());
    cerr << "One or more errors occured!" << endl << endl;
    cerr << "Please check the error report in the error file: " << endl << endl;
    cerr << mParsingErrorsFile << endl << endl;
    cerr << "Please correct the input file and then try again" << endl << endl;
    cerr << "Input file: " << endl << endl << c_InputFile << endl << endl;
    cerr << "Thank you for using LabelCalculator!" << endl << endl;
}

void Application::_displayFileOpeningErrorMessage(Application::FileType fileType)
{
    using namespace std;

    string file;

    switch(fileType)
    {
    case FileType::INPUT:
        assert(ParserCreator::ParserTypes::UNKNOWN != mParserType);
        file = _getInputFile();
        break;
    case FileType::OUTPUT:
        assert(ParserCreator::ParserTypes::UNKNOWN != mParserType);
        file = _getOutputFile();
        break;
    case FileType::ERROR:
        file = mParsingErrorsFile;
        break;
    case FileType::CONFIGURATION:
        file = c_ConfigurationFile;
        break;
    default:
        assert(false);
    }

    const string c_Operation{FileType::OUTPUT == fileType || FileType::ERROR == fileType ? "writing" : "reading"};

    system(c_ClearScreenCommand.c_str());
    cerr << "Error! File cannot be opened for " << c_Operation << "." << endl << endl;
    cerr << "File path: "<< file << endl << endl;
}

void Application::_displayAbortMessage()
{
    system(c_ClearScreenCommand.c_str());
    std::cout << "Application terminated by user" << std::endl << std::endl;
}

void Application::_displayMenu()
{
    using namespace std;

    cout << "Please choose between following options:" << endl << endl;
    cout << "1 + ENTER: read the defined connections from file connectiondefinitions.csv and write the partial input data into file connectioninput.csv" << endl;
    cout << "2 + ENTER: read the input data from file connectioninput.csv and write the labeling information into file labellingtable.csv" << endl << endl;
    cout << "Choose any other combination + ENTER or directly press ENTER to exit the application" << endl << endl;
}

void Application::_displayVersion()
{
    system(c_ClearScreenCommand.c_str());
    std::cout << "LabelCalculator v1.0" << std::endl << std::endl;
}

std::string Application::_getInputFile() const
{
    std::string inputFile;

    if (ParserCreator::ParserTypes::CONNECTION_DEFINITION == mParserType)
    {
        inputFile = mConnectionDefinitionsFile;
    }
    else if (ParserCreator::ParserTypes::CONNECTION_INPUT == mParserType)
    {
        inputFile = mConnectionInputFile;
    }
    else
    {
        assert(false);
    }

    return inputFile;
}

std::string Application::_getOutputFile() const
{
    std::string outputFile;

    if (ParserCreator::ParserTypes::CONNECTION_DEFINITION == mParserType)
    {
        outputFile = mConnectionInputFile;
    }
    else if (ParserCreator::ParserTypes::CONNECTION_INPUT == mParserType)
    {
        outputFile = mLabellingOutputFile;
    }
    else
    {
        assert(false);
    }

    return outputFile;
}
