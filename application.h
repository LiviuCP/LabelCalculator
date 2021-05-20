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
        FILE_NOT_OPENED,
        PARSING_ERROR,
        PARSER_NOT_CREATED,
        MISSING_USERNAME,
        ABORTED_BY_USER
    };

    // file types (used for error message creation)
    enum class FileType
    {
        INPUT,
        OUTPUT,
        ERROR
    };

    /* Private constructor (singleton)
    */
    Application();

    /* This function initializes the application no matter which option is chosen (define connections, process connection input from user)
    */
    void _init();

    /* This function opens the input and output files for each of the options
    */
    void _enableCSVParsing();

    /* This function takes over the input from user and sets the parser type
    */
    bool _handleUserInput();

    /* Output methods */
    void _displaySuccessMessage(bool additionalOutputRequired) const;
    void _displayParsingErrorMessage() const;
    void _displayFileOpeningErrorMessage(FileType fileType) const;
    static void _displayAbortMessage();
    static void _displayInvalidInputMessage();
    static void _displayMissingUsernameMessage();
    static void _displayParserNotCreatedMessage();
    static void _displayMenu();
    static void _displayGreetingAndVersion();

    /* Getters */
    std::string _getInputFile() const;
    std::string _getOutputFile() const;



    static std::shared_ptr<Application> s_pApplication;

    ParserCreator::ParserTypes mParserType;

    std::string mConnectionDefinitionsFile;
    std::string mConnectionInputFile;
    std::string mLabellingOutputFile;
    std::string mParsingErrorsFile;

    std::ifstream mInputStream;
    std::ofstream mOutputStream;
    std::ofstream mErrorStream;

    bool mIsInitialized;
    bool mIsCSVParsingEnabled;

    StatusCode mStatusCode;
};

#endif // APPLICATION_H
