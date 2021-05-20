#include <iostream>
#include <cassert>

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

    if (mIsInitialized)
    {
        _displayGreetingAndVersion();
        _displayMenu();

        const bool c_InputProvided{_handleUserInput()};

        if (c_InputProvided)
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
                        mStatusCode = StatusCode::SUCCESS;
                        const bool c_DisplayFurtherInstructions{ParserCreator::ParserTypes::CONNECTION_DEFINITION == mParserType ? true : false};
                        _displaySuccessMessage(c_DisplayFurtherInstructions);
                    }
                    else
                    {
                        mStatusCode = StatusCode::PARSING_ERROR;
                        _displayParsingErrorMessage();
                    }
                }
                else
                {
                    mStatusCode = StatusCode::PARSER_NOT_CREATED;
                    _displayParserNotCreatedMessage();
                }
            }
        }
        else
        {
            mStatusCode = StatusCode::ABORTED_BY_USER;
            _displayAbortMessage();
        }
    }

    // user abort is not considered an error so the success code is returned
    const int returnCode{StatusCode::ABORTED_BY_USER == mStatusCode ? 0 : static_cast<int>(mStatusCode)};

    return returnCode;
}

Application::Application()
    : mParserType{ParserCreator::ParserTypes::UNKNOWN}
    , mIsInitialized{false}
    , mIsCSVParsingEnabled{false}
    , mStatusCode{StatusCode::UNINITIALIZED}
{
    _init();
}

void Application::_init()
{
    using namespace std;

    assert(!mIsCSVParsingEnabled);

    if (!mIsInitialized)
    {
        // username is used for determining the paths of all other relevant files (see below)
        const string username{getUsername()};

        if (username.size() > 0u)
        {
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
                mStatusCode = StatusCode::FILE_NOT_OPENED;
                _displayFileOpeningErrorMessage(FileType::ERROR);
            }
        }
        else
        {
            mStatusCode = StatusCode::MISSING_USERNAME;
            _displayMissingUsernameMessage();
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
                mStatusCode = StatusCode::FILE_NOT_OPENED;
                _displayFileOpeningErrorMessage(FileType::OUTPUT);
            }
        }
        else
        {
            mStatusCode = StatusCode::FILE_NOT_OPENED;
            _displayFileOpeningErrorMessage(FileType::INPUT);
        }
    }
}

bool Application::_handleUserInput()
{
    bool validInputProvided{false};

    while (!validInputProvided)
    {
        std::string option;
        getline(std::cin, option);

        if ("1" == option)
        {
            mParserType = ParserCreator::ParserTypes::CONNECTION_DEFINITION;
            validInputProvided = true;
        }
        else if ("2" == option)
        {
            mParserType = ParserCreator::ParserTypes::CONNECTION_INPUT;
            validInputProvided = true;
        }
        else if (0u == option.size())
        {
            break;
        }
        else
        {
            _displayInvalidInputMessage();
            _displayMenu();
        }
    }

    return validInputProvided;
}

void Application::_displaySuccessMessage(bool additionalOutputRequired) const
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

void Application::_displayParsingErrorMessage() const
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

void Application::_displayFileOpeningErrorMessage(Application::FileType fileType) const
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
    default:
        assert(false);
    }

    const string c_Operation{FileType::OUTPUT == fileType || FileType::ERROR == fileType ? "writing" : "reading"};

    system(c_ClearScreenCommand.c_str());
    cerr << "Error! File cannot be opened for " << c_Operation << "." << endl << endl;
    cerr << "File path: "<< file << endl << endl;
    cerr << "The file might not exist or the user might not have the required permissions to open it." << endl << endl;
}

void Application::_displayInvalidInputMessage()
{
    system(c_ClearScreenCommand.c_str());
    std::cout << "Invalid input. Please try again" << std::endl << std::endl;
}

void Application::_displayMissingUsernameMessage()
{
    system(c_ClearScreenCommand.c_str());
    std::cout << "The username could not be retrieved. Please check your system settings and try again" << std::endl << std::endl;
}

void Application::_displayParserNotCreatedMessage()
{
    system(c_ClearScreenCommand.c_str());
    std::cout << "Error in initializing parsing functionality. Application aborted" << std::endl << std::endl;
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
    cout << "Press ENTER to exit the application" << endl << endl;
}

void Application::_displayGreetingAndVersion()
{
    system(c_ClearScreenCommand.c_str());
    std::cout << "LabelCalculator v1.0" << std::endl << std::endl;
    std::cout << "Hello, " << getUsername() << "!" << std::endl << std::endl;
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
