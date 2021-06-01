#include <iostream>
#include <cassert>

#include "appsettings.h"
#include "application.h"

Application::Application()
    : mParserType{ParserCreator::ParserTypes::UNKNOWN}
    , mIsInitialized{false}
    , mIsCSVParsingEnabled{false}
    , mStatusCode{StatusCode::UNINITIALIZED}
{
    _init();
}

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
                    }
                    else
                    {
                        mStatusCode = StatusCode::PARSING_ERROR;
                    }
                }
                else
                {
                    mStatusCode = StatusCode::PARSER_NOT_CREATED;
                }
            }
        }
        else
        {
            mStatusCode = StatusCode::ABORTED_BY_USER;
        }
    }

    const int returnCode{_handleStatusCode()};

    return returnCode;
}

void Application::_init()
{
    assert(!mIsCSVParsingEnabled);

    if (!mIsInitialized)
    {
        if (AppSettings::getInstance()->areSettingsValid())
        {
            mConnectionDefinitionsFile = AppSettings::getInstance()->getConnectionDefinitionsFile();
            mConnectionInputFile = AppSettings::getInstance()->getConnectionInputFile();
            mLabellingOutputFile = AppSettings::getInstance()->getLabellingOutputFile();
            mParsingErrorsFile = AppSettings::getInstance()->getParsingErrorsFile();

            mErrorStream.open(mParsingErrorsFile);

            if (mErrorStream.is_open())
            {
                mIsInitialized = true;
            }
            else
            {
                mStatusCode = StatusCode::ERROR_FILE_NOT_OPENED;
            }
        }
        else
        {
            mStatusCode = StatusCode::INVALID_SETTINGS;
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
                mStatusCode = StatusCode::OUTPUT_FILE_NOT_OPENED;
            }
        }
        else
        {
            mStatusCode = StatusCode::INPUT_FILE_NOT_OPENED;
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

int Application::_handleStatusCode() const
{
    switch(mStatusCode)
    {
    case StatusCode::UNINITIALIZED:
        assert(false);
        break;
    case StatusCode::SUCCESS:
    {
        assert(ParserCreator::ParserTypes::UNKNOWN != mParserType);
        const bool c_DisplayFurtherInstructions{ParserCreator::ParserTypes::CONNECTION_DEFINITION == mParserType ? true : false};
        _displaySuccessMessage(c_DisplayFurtherInstructions);
    }
        break;
    case StatusCode::INVALID_SETTINGS:
        _displayInvalidSettingsMessage();
        break;
    case StatusCode::INPUT_FILE_NOT_OPENED:
    case StatusCode::OUTPUT_FILE_NOT_OPENED:
    case StatusCode::ERROR_FILE_NOT_OPENED:
        _displayFileOpeningErrorMessage();
        break;
    case StatusCode::PARSER_NOT_CREATED:
        _displayParserNotCreatedMessage();
        break;
    case StatusCode::PARSING_ERROR:
        _displayParsingErrorMessage();
        break;
    case StatusCode::ABORTED_BY_USER:
        _displayAbortMessage();
        break;
    }

    // user abort is not considered an error so the success code is returned
    const int returnCode{StatusCode::ABORTED_BY_USER == mStatusCode ? 0 : static_cast<int>(mStatusCode)};

    return returnCode;
}

void Application::_displayInvalidSettingsMessage()
{
    system(scClearScreenCommand.c_str());
    std::cout << "Invalid settings detected. Please check your system settings and try again" << std::endl << std::endl;
}

void Application::_displayParserNotCreatedMessage()
{
    system(scClearScreenCommand.c_str());
    std::cout << "Error in initializing parsing functionality. Application aborted" << std::endl << std::endl;
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
    system(scClearScreenCommand.c_str());
    std::cout << "LabelCalculator v1.0" << std::endl << std::endl;
    std::cout << "Hello, " << AppSettings::getInstance()->getUsername() << "!" << std::endl << std::endl;
}

void Application::_displayInvalidInputMessage()
{
    system(scClearScreenCommand.c_str());
    std::cout << "Invalid input. Please try again" << std::endl << std::endl;
}

void Application::_displayAbortMessage()
{
    system(scClearScreenCommand.c_str());
    std::cout << "Application terminated by user" << std::endl << std::endl;
}

void Application::_displayFileOpeningErrorMessage() const
{
    using namespace std;

    string file;

    switch(mStatusCode)
    {
    case StatusCode::INPUT_FILE_NOT_OPENED:
        assert(ParserCreator::ParserTypes::UNKNOWN != mParserType);
        file = _getInputFile();
        break;
    case StatusCode::OUTPUT_FILE_NOT_OPENED:
        assert(ParserCreator::ParserTypes::UNKNOWN != mParserType);
        file = _getOutputFile();
        break;
    case StatusCode::ERROR_FILE_NOT_OPENED:
        file = mParsingErrorsFile;
        break;
    default:
        assert(false);
    }

    const string c_Operation{StatusCode::INPUT_FILE_NOT_OPENED == mStatusCode ? "reading" : "writing"};

    system(scClearScreenCommand.c_str());
    cerr << "Error! File cannot be opened for " << c_Operation << "." << endl << endl;
    cerr << "File path: "<< file << endl << endl;
    cerr << "The file might not exist or the user might not have the required permissions to open it." << endl << endl;
}

void Application::_displayParsingErrorMessage() const
{
    using namespace std;

    const string c_InputFile{_getInputFile()};

    system(scClearScreenCommand.c_str());
    cerr << "One or more errors occured!" << endl << endl;
    cerr << "Please check the error report in the error file: " << endl << endl;
    cerr << mParsingErrorsFile << endl << endl;
    cerr << "Please correct the input file and then try again" << endl << endl;
    cerr << "Input file: " << endl << endl << c_InputFile << endl << endl;
    cerr << "Thank you for using LabelCalculator!" << endl << endl;
}

void Application::_displaySuccessMessage(bool additionalOutputRequired) const
{
    using namespace std;

    const string c_OutputFile{_getOutputFile()};

    system(scClearScreenCommand.c_str());
    cout << "The program ended succesfully. " << endl << endl;
    cout << "Please view the output file: " << endl << endl;
    cout << c_OutputFile << endl << endl;

    if (additionalOutputRequired)
    {
        cout << "Please go on with the next step by filling in the connectioninput.csv file with the needed info" << endl << endl;
    }

    cout << "Thank you for using LabelCalculator!" << endl << endl;
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

std::shared_ptr<Application> Application::s_pApplication = nullptr;

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
const std::string Application::scClearScreenCommand{"clear"};
#else
const std::string Application::scClearScreenCommand{"cls"};
#endif
