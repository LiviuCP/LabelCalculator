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
    enum class ReturnCode : int
    {
        SUCCESS,
        FILE_NOT_OPENED,
        PARSING_ERROR,
        PARSER_NOT_CREATED
    };

    // file types (used for error message creation)
    enum class FileType
    {
        INPUT,
        OUTPUT,
        ERROR,
        CONFIGURATION
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
    void _handleUserInput();

    /* Output methods */
    void _displaySuccessMessage(bool additionalOutputRequired);
    void _displayParsingErrorMessage();
    void _displayFileOpeningErrorMessage(FileType fileType);
    static void _displayAbortMessage();
    static void _displayMenu();
    static void _displayVersion();

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
};

#endif // APPLICATION_H
