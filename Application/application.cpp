#include "preparse.h"
#include "appsettings.h"
#include "coreutils.h"
#include "application.h"

namespace Core = Utilities::Core;
namespace Aux = Utilities::Other;

Application::Application()
    : mParserType{ParserCreator::ParserTypes::UNKNOWN}
    , mpInputStream{std::make_shared<std::ifstream>()}
    , mpOutputStream{std::make_shared<std::ofstream>()}
    , mpErrorStream{std::make_shared<std::ofstream>()}
    , mIsInitialized{false}
    , mIsFileIOEnabled{false}
    , mIsCSVParsingRequired{true}
    , mStatusCode{StatusCode::UNDEFINED}
{
    _init();
}

std::shared_ptr<Application> Application::getInstance()
{
    if (!s_pApplication)
    {
        s_pApplication.reset(new Application);
    }

    return s_pApplication;
}

int Application::run()
{
    if (mIsInitialized)
    {
        _displayGreetingAndVersion();
        _displayMenu();

        const bool c_UserInputProvided{_handleUserInput()};

        if (c_UserInputProvided)
        {
            _enableFileInputOutput();
        }
        else
        {
            mStatusCode = StatusCode::ABORTED_BY_USER;
        }

        if (mIsFileIOEnabled)
        {
            if (mIsCSVParsingRequired)
            {
                _parseInput();
            }
            else
            {
                Aux::createEmptyConnectionDefinitionsFile(mpOutputStream);
                mStatusCode = StatusCode::SUCCESS;
            }
        }
    }

    const int returnCode{_handleStatusCode()};

    return returnCode;
}

void Application::_parseInput()
{
    if (mIsCSVParsingRequired)
    {
        ParserCreator parserCreator;

        if (!parserCreator.isParserAlreadyCreated())
        {
            const ParserPtr pParser{parserCreator.createParser(mParserType, mpInputStream, mpOutputStream, mpErrorStream)};

            if (pParser)
            {
                const bool c_ParsingErrorsOccurred{pParser->parse()};
                mStatusCode = !c_ParsingErrorsOccurred ? StatusCode::SUCCESS : StatusCode::PARSING_ERROR;
            }
            else
            {
                mStatusCode = StatusCode::PARSER_NOT_CREATED;
            }
        }
    }
}

void Application::_init()
{
    if (!mIsInitialized)
    {
        const bool c_EnvironmentSuccessfullySetup{_setApplicationEnvironment()};

        if (c_EnvironmentSuccessfullySetup && mpErrorStream)
        {
            mpErrorStream->open(mParsingErrorsFile);

            if (mpErrorStream->is_open())
            {
                mIsInitialized = true;
            }
            else
            {
                mStatusCode = StatusCode::ERROR_FILE_NOT_OPENED;
            }
        }
    }
}

bool Application::_setApplicationEnvironment()
{
    bool success{false};

    if (AppSettings::getInstance()->areSettingsValid())
    {
        _retrieveDirPaths();

        const bool c_DirsSuccessfullySetup{_setDirectories()};

        if (c_DirsSuccessfullySetup)
        {
            _copyExamplesDir();
            _retrieveFilePaths();

            success = true;
        }
    }
    else
    {
        mStatusCode = StatusCode::INVALID_SETTINGS;
    }

    // it is always a good idea to provide the user with a good starting point, namely a connection definitions file ready to be filled-in
    if (success && !std::filesystem::exists(mConnectionDefinitionsFile))
    {
        OutputStreamPtr pEmptyFileStream{std::make_shared<std::ofstream>(mConnectionDefinitionsFile)};

        if (pEmptyFileStream->is_open())
        {
            Aux::createEmptyConnectionDefinitionsFile(pEmptyFileStream);
        }
        else
        {
            success = false;
        }
    }

    return success;
}

void Application::_retrieveFilePaths()
{
    mConnectionDefinitionsFile = AppSettings::getInstance()->getConnectionDefinitionsFile();
    mConnectionInputFile = AppSettings::getInstance()->getConnectionInputFile();
    mLabellingOutputFile = AppSettings::getInstance()->getLabellingOutputFile();
    mParsingErrorsFile = AppSettings::getInstance()->getParsingErrorsFile();
}

void Application::_retrieveDirPaths()
{
    mAppDataDir = AppSettings::getInstance()->getAppDataDir();
    mInputBackupDir = AppSettings::getInstance()->getInputBackupDir();
    mOutputBackupDir = AppSettings::getInstance()->getOutputBackupDir();
}

bool Application::_setDirectories()
{
    bool success{_setDirectory(mAppDataDir)};

    if (!success)
    {
        mStatusCode = StatusCode::APP_DATA_DIR_NOT_SETUP;
    }

    if (success)
    {
        success = _setDirectory(mInputBackupDir);

        if (!success)
        {
            mStatusCode = StatusCode::INPUT_BACKUP_DIR_NOT_SETUP;
        }
    }

    if (success)
    {
        success = _setDirectory(mOutputBackupDir);

        if (!success)
        {
            mStatusCode = StatusCode::OUTPUT_BACKUP_DIR_NOT_SETUP;
        }
    }

    return success;
}

bool Application::_setDirectory(const Path_t& dirPath)
{
    bool success{false};

    if (!dirPath.empty())
    {
        if (std::filesystem::exists(dirPath) && std::filesystem::is_directory(dirPath))
        {
            success = true;
        }
        else
        {
            try
            {
                success = std::filesystem::create_directories(dirPath);
            }
            catch(std::filesystem::filesystem_error&)
            {
                success = false;
            }
        }
    }
    else
    {
        ASSERT(false, "Empty path, cannot create directory");
    }

    return success;
}

void Application::_copyExamplesDir()
{
    const Path_t c_AppExamplesDir{AppSettings::getInstance()->getAppExamplesDir()};
    const Path_t c_AppDataExamplesDir{AppSettings::getInstance()->getAppDataExamplesDir()};

    if (std::filesystem::exists(c_AppExamplesDir) && std::filesystem::is_directory(c_AppExamplesDir) && !std::filesystem::exists(c_AppDataExamplesDir))
    {
        std::filesystem::copy(c_AppExamplesDir, c_AppDataExamplesDir, std::filesystem::copy_options::recursive);
    }
}

void Application::_enableFileInputOutput()
{
    if (mIsInitialized && !mIsFileIOEnabled)
    {
        // move existing output file to the appropriate backup folder to ensure it doesn't get overwritten
        _moveOutputFileToBackupDir();

        bool isOutputEnabled{false};

        if (mpOutputStream)
        {
            mpOutputStream->open(_getOutputFile());
            isOutputEnabled = mpOutputStream->is_open();
        }

        // for option 3 output enabling is sufficient (no input is required)
        if (isOutputEnabled)
        {
            mIsFileIOEnabled = !mIsCSVParsingRequired;
        }
        else
        {
            mStatusCode = StatusCode::OUTPUT_FILE_NOT_OPENED;
        }

        // for options 1 and 2 input should be enabled as well
        if (isOutputEnabled && mIsCSVParsingRequired)
        {
            bool isInputEnabled{false};

            if (mpInputStream)
            {
                mpInputStream->open(_getInputFile());
                isInputEnabled = mpInputStream->is_open();
            }

            mIsFileIOEnabled = isInputEnabled;

            if (!isInputEnabled)
            {
                mStatusCode = StatusCode::INPUT_FILE_NOT_OPENED;
            }
        }
    }
}

void Application::_moveOutputFileToBackupDir()
{
    const Path_t c_OutputFile{_getOutputFile()};

    if (!c_OutputFile.empty() && std::filesystem::exists(c_OutputFile))
    {
        const Path_t c_BackupDir{c_OutputFile == mLabellingOutputFile ? mOutputBackupDir : mInputBackupDir};

        if (!c_BackupDir.empty() && std::filesystem::exists(c_BackupDir) && std::filesystem::is_directory(c_BackupDir))
        {
            Path_t movedOutputFile{c_BackupDir};

            // prepend timestamp (last modified date/time) to output file name in order to differentiate it from other files stored in the backup dir
            movedOutputFile /= Core::getDateTimeString(std::chrono::system_clock::now());
            movedOutputFile += "_";
            movedOutputFile += c_OutputFile.filename();

            std::filesystem::rename(c_OutputFile, movedOutputFile);
        }
        else
        {
            ASSERT(false, "The path of the backup directory is empty or the file does not exist/is not a directory");
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
        else if ("3" == option)
        {
            mIsCSVParsingRequired = false;
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

int Application::_handleStatusCode()
{
    switch(mStatusCode)
    {
    case StatusCode::UNDEFINED:
        ASSERT(false, "Undefined application status");
        break;
    case StatusCode::SUCCESS:
        _displaySuccessMessage(ParserCreator::ParserTypes::CONNECTION_DEFINITION == mParserType);
        break;
    case StatusCode::INVALID_SETTINGS:
        _displayInvalidSettingsMessage();
        break;
    case StatusCode::APP_DATA_DIR_NOT_SETUP:
    case StatusCode::INPUT_BACKUP_DIR_NOT_SETUP:
    case StatusCode::OUTPUT_BACKUP_DIR_NOT_SETUP:
        _displayDirectoryNotSetupMessage();
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

    _removeUnnecessaryFiles();

    // user abort is not considered an error so the success code is returned
    const int returnCode{StatusCode::ABORTED_BY_USER == mStatusCode ? 0 : static_cast<int>(mStatusCode)};

    return returnCode;
}

void Application::_removeUnnecessaryFiles()
{
    Path_t fileToRemove{mParsingErrorsFile};

    // appropriate stream should be closed prior to erasing the file
    if (StatusCode::PARSING_ERROR != mStatusCode)
    {
        if(mpErrorStream)
        {
            mpErrorStream->close();
        }
    }
    else
    {
        if (mpOutputStream)
        {
            mpOutputStream->close();
        }

        fileToRemove = _getOutputFile();
    }

    // for some statuses the errors file path might be empty (the output file should't but the check is made for consistency)
    if (!fileToRemove.empty())
    {
        std::filesystem::remove(fileToRemove);
    }
}

void Application::_displayInvalidSettingsMessage()
{
    system(scClearScreenCommand.data());
    std::cout << "Invalid settings detected. Please check your system settings and try again\n\n";
}

void Application::_displayParserNotCreatedMessage()
{
    system(scClearScreenCommand.data());
    std::cout << "Error in initializing parsing functionality. Application aborted\n\n";
}

void Application::_displayMenu()
{
    std::cout << "Please choose between following options:\n\n";
    std::cout << "1 + ENTER: read the defined connections from file connectiondefinitions.csv and write the partial input data into file connectioninput.csv\n";
    std::cout << "2 + ENTER: read the input data from file connectioninput.csv and write the labeling information into file labellingtable.csv\n";
    std::cout << "3 + ENTER: create an empty connection definitions file or clear the existing one\n\n";
    std::cout << "Press ENTER to exit the application\n\n";
}

void Application::_displayGreetingAndVersion()
{
    system(scClearScreenCommand.data());
    std::cout << "LabelCalculator v1.1\n\n";
    std::cout << "Hello, " << AppSettings::getInstance()->getUsername() << "!\n\n";
}

void Application::_displayInvalidInputMessage()
{
    system(scClearScreenCommand.data());
    std::cout << "Invalid input. Please try again\n\n";
}

void Application::_displayAbortMessage()
{
    system(scClearScreenCommand.data());
    std::cout << "Application terminated by user\n\n";
}

void Application::_displayDirectoryNotSetupMessage() const
{
    Path_t dirPath;
    std::string dirType;

    switch(mStatusCode)
    {
    case StatusCode::APP_DATA_DIR_NOT_SETUP:
        dirPath = mAppDataDir;
        dirType = "application data";
        break;
    case StatusCode::INPUT_BACKUP_DIR_NOT_SETUP:
        dirPath = mInputBackupDir;
        dirType = "input backup";
        break;
    case StatusCode::OUTPUT_BACKUP_DIR_NOT_SETUP:
        dirPath = mOutputBackupDir;
        dirType = "output backup";
        break;
    default:
        ASSERT(false, "Undefined application status at directory setup");
    }

    system(scClearScreenCommand.data());
    std::cerr << "Error! The " << dirType << " directory cannot be setup.\n";
    std::cerr << "Path: " << dirPath.string() << "\n\n";
}

void Application::_displayFileOpeningErrorMessage() const
{
    Path_t file;

    switch(mStatusCode)
    {
    case StatusCode::INPUT_FILE_NOT_OPENED:
    case StatusCode::OUTPUT_FILE_NOT_OPENED:
        if (ParserCreator::ParserTypes::UNKNOWN != mParserType)
        {
            file = StatusCode::INPUT_FILE_NOT_OPENED == mStatusCode ? _getInputFile() : _getOutputFile();
        }
        else
        {
            ASSERT(false, "Parser is invalid");
        }
        break;
    case StatusCode::ERROR_FILE_NOT_OPENED:
        file = mParsingErrorsFile;
        break;
    default:
        ASSERT(false, "Undefined application status at file opening");
    }

    const std::string c_Operation{StatusCode::INPUT_FILE_NOT_OPENED == mStatusCode ? "reading" : "writing"};

    system(scClearScreenCommand.data());
    std::cerr << "Error! File cannot be opened for " << c_Operation << ".\n\n";
    std::cerr << "File path: "<< file.string() << "\n\n";
    std::cerr << "The file might not exist or the user might not have the required permissions to open it.\n\n";
}

void Application::_displayParsingErrorMessage() const
{
    system(scClearScreenCommand.data());

    std::cerr << "One or more errors occured!\n\n";
    std::cerr << "Please check the error report in the error file: \n\n";
    std::cerr << mParsingErrorsFile.string() << "\n\n";
    std::cerr << "Please correct the input file and then try again\n\n";
    std::cerr << "Input file: \n\n" << _getInputFile().string() << "\n\n";
    std::cerr << "Thank you for using LabelCalculator!\n\n";
}

void Application::_displaySuccessMessage(bool additionalOutputRequired) const
{
    system(scClearScreenCommand.data());

    std::cout << "The program ended succesfully. \n\n";
    std::cout << "Please view the output file: \n\n";
    std::cout << _getOutputFile().string() << "\n\n";

    if (additionalOutputRequired)
    {
        std::cout << "Please go on with the next step by filling in the connectioninput.csv file with the needed info\n\n";
    }

    std::cout << "Thank you for using LabelCalculator!\n\n";
}

Path_t Application::_getInputFile() const
{
    Path_t inputFile;

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
        ASSERT(false, "Parser is invalid");
    }

    return inputFile;
}

Path_t Application::_getOutputFile() const
{
    Path_t outputFile;

    if (ParserCreator::ParserTypes::CONNECTION_DEFINITION == mParserType)
    {
        outputFile = mConnectionInputFile;
    }
    else if (ParserCreator::ParserTypes::CONNECTION_INPUT == mParserType)
    {
        outputFile = mLabellingOutputFile;
    }
    else if (!mIsCSVParsingRequired)
    {
        outputFile = mConnectionDefinitionsFile;
    }
    else
    {
        ASSERT(false, "Parser is invalid");
    }

    return outputFile;
}

std::shared_ptr<Application> Application::s_pApplication = nullptr;
