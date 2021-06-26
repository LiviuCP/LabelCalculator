#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "errorhandler.h"

/* This is a generic parser class for .csv files.
   The content of an input .csv file is read, parsed and the resulting output written to an output .csv file.
   If errors occur they are being logged to an error file. In this case the output file stays empty.
*/
class Parser
{
public:
    Parser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream, const std::string& header);
    virtual ~Parser();

    virtual bool parse();

protected:
    /* This function reads all rows from input file and caches them for further processing.
    */
    virtual void _readInput() = 0;

    /* This function parses all rows read by previous function and generates intermediary data to be used for generating the output file.
    */
    virtual bool _parseInput() = 0;

    /* This function builds the rows to be written into the output file.
    */
    virtual void _buildOutput() = 0;

    /* This function writes the resulting output to file.
    */
    virtual void _writeOutput();

    /* This function resets the internal parser state after each parsing session.
    */
    virtual void _reset();

    /* This function is used for setting the error handler for any derived parser
       The handler is not set in constructor but when the actual parsing begins (lazy initialization)
    */
    virtual void _initializeErrorHandler() final;

    /* Used for storing locally generated errors
       Error location is setup at error storing point
    */
    virtual void _storeParsingErrorAndLocation(ErrorPtr pError);

    /* Used for logging all parsing errors to file
    */
    virtual bool _logParsingErrorsToFile();

    /* file streams used by parser, each one should correspond to a file that had been previously correctly opened */
    std::ifstream* const mpInputStream;
    std::ofstream* const mpOutputStream;
    std::ofstream* const mpErrorStream;

    /* stores raw data (.csv rows) read from input file */
    std::vector<std::string> mInputData;

    /* stores final data (.csv rows) to be written to output file */
    std::vector<std::string> mOutputData;

    /* header (.csv row) to be written to output file */
    std::string mOutputHeader;

    /* determines if the parser state needs to be reset before executing a parsing session */
    bool mIsResetRequired;

    /* gather all parsing errors here and write them to output file once parsing is complete (if any errors) */
    std::vector<ErrorPtr> mParsingErrors;

    /* current character index in the currently parsed CSV row string */
    ssize_t mCurrentPosition;

    /* current CSV column being parsed (the numbering starts from 1 as when opening the CSV with a spreadsheet tool)
    */
    size_t mFileColumnNumber;

    /* error handler used for creating the objects that are responsible for logging the parsing errors to file */
    std::unique_ptr<ErrorHandler> mpErrorHandler;
};

using ParserPtr = std::unique_ptr<Parser>;

#endif // PARSER_H
