#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <string>
#include <fstream>

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
        UNINITIALIZED = -1,
        SUCCESS,
        INVALID_SETTINGS,
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

    /* This function initializes the application no matter which option is chosen (define connections, process connection input from user)
    */
    void _init();

    /* This function opens the input and output files for each of the options
    */
    void _enableFileInputOutput();

    /* This function takes over the input from user and sets the parser type
    */
    bool _handleUserInput();

    /* This function displays a message for each resulting status code.
       It also determines the return code of the application.
       The goal of this method is to avoid spreading output messages among functions and to consolidate them instead.
    */
    int _handleStatusCode() const;

    /* Output methods */
    static void _displayInvalidSettingsMessage();
    static void _displayParserNotCreatedMessage();
    static void _displayMenu();
    static void _displayGreetingAndVersion();
    static void _displayInvalidInputMessage();
    static void _displayAbortMessage();
    void _displayFileOpeningErrorMessage() const;
    void _displayParsingErrorMessage() const;
    void _displaySuccessMessage(bool additionalOutputRequired) const;

    /* Getters */
    std::string _getInputFile() const;
    std::string _getOutputFile() const;

    /* Attributes */
    static std::shared_ptr<Application> s_pApplication;
    static const std::string scClearScreenCommand;

    ParserCreator::ParserTypes mParserType;

    std::string mConnectionDefinitionsFile;
    std::string mConnectionInputFile;
    std::string mLabellingOutputFile;
    std::string mParsingErrorsFile;

    std::ifstream mInputStream;
    std::ofstream mOutputStream;
    std::ofstream mErrorStream;

    bool mIsInitialized;
    bool mIsFileIOEnabled;
    bool mIsCSVParsingRequired;

    StatusCode mStatusCode;
};

#endif // APPLICATION_H
