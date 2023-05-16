#ifndef APPLICATION_H
#define APPLICATION_H

#include "parsercreator.h"

class Application
{

public:
    static std::shared_ptr<Application> getInstance();
    int run();

private:
    // underlying type int for compatibility with the main() return type
    enum class StatusCode : int
    {
        UNDEFINED = -1,
        SUCCESS,
        INVALID_SETTINGS,
        APP_DATA_DIR_NOT_SETUP,
        INPUT_BACKUP_DIR_NOT_SETUP,
        OUTPUT_BACKUP_DIR_NOT_SETUP,
        INPUT_FILE_NOT_OPENED,
        OUTPUT_FILE_NOT_OPENED,
        ERROR_FILE_NOT_OPENED,
        PARSER_NOT_CREATED,
        PARSING_ERROR,
        ABORTED_BY_USER
    };

    /* Private constructor (singleton)
    */
    Application();

    /* This function initiates the data parsing process by creating parser and putting it to work
    */
    void _parseInput();

    /* This function initializes the application no matter which option is chosen (define connections, process connection input from user)
    */
    void _init();

    /* This function retrieves the paths of the required application files and directories from settings
       Based on the retrieved data it creates the application data directories and (if required) an empty connection definitions file
    */
    bool _setApplicationEnvironment();

    /* This function retrieves the paths of the application files from settings
    */
    void _retrieveFilePaths();

    /* This function retrieves the paths of the application directories from settings
    */
    void _retrieveDirPaths();

    /* This function creates the required directories or confirms their existence if they are already available
    */
    bool _setDirectories();

    /* This function creates a directory or confirms its existence
    */
    static bool _setDirectory(const Core::Path_t& dirPath);

    /* This function copies the example files directory from application folder into application data directory
    */
    static void _copyExamplesDir();

    /* This function opens the input and output files for each of the options
    */
    void _enableFileInputOutput();

    /* This function moves the output file to corresponding backup dir
       Any data csv file can be an output file depending on operation chosen by user
    */
    void _moveOutputFileToBackupDir();

    /* This function takes over the input from user and sets the parser type
    */
    bool _handleUserInput();

    /* This function displays a message for each resulting status code.
       It also determines the return code of the application.
       The goal of this method is to avoid spreading output messages among functions and to consolidate them instead.
    */
    int _handleStatusCode();

    /* This function deletes files that are no longer needed due to the application flow (e.g. error.txt file should be erased if no errors occurred)
    */
    void _removeUnnecessaryFiles();

    /* Output methods */
    static void _displayInvalidSettingsMessage();
    static void _displayParserNotCreatedMessage();
    static void _displayMenu();
    static void _displayGreetingAndVersion();
    static void _displayInvalidInputMessage();
    static void _displayAbortMessage();
    void _displayDirectoryNotSetupMessage() const;
    void _displayFileOpeningErrorMessage() const;
    void _displayParsingErrorMessage() const;
    void _displaySuccessMessage(bool additionalOutputRequired) const;

    /* Getters */
    Core::Path_t _getInputFile() const;
    Core::Path_t _getOutputFile() const;

    /* Attributes */
    static std::shared_ptr<Application> s_pApplication;

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
static constexpr std::string_view scClearScreenCommand{"clear"};
#else
static constexpr std::string_view scClearScreenCommand{"cls"};
#endif

    ParserCreator::ParserTypes mParserType;

    Core::Path_t mAppDataDir;
    Core::Path_t mInputBackupDir;
    Core::Path_t mOutputBackupDir;

    Core::Path_t mConnectionDefinitionsFile;
    Core::Path_t mConnectionInputFile;
    Core::Path_t mLabellingOutputFile;
    Core::Path_t mParsingErrorsFile;

    const Core::InputStreamPtr mpInputStream;
    const Core::OutputStreamPtr mpOutputStream;
    const Core::ErrorStreamPtr mpErrorStream;

    bool mIsInitialized;
    bool mIsFileIOEnabled;
    bool mIsCSVParsingRequired;

    StatusCode mStatusCode;
};

#endif // APPLICATION_H
