#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "errorhandler.h"
#include "isubparserobserver.h"

#ifdef _WIN32
#include "auxdata.h"
#endif

class ISubParser;

/* This is a generic parser class for .csv files.
   The content of an input .csv file is read, parsed and the resulting output written to an output .csv file.
   If errors occur they are being logged to an error file. In this case the output file stays empty.
*/
class Parser : public ISubParserObserver
{
public:
    Parser(const Core::InputStreamPtr pInputStream, const Core::OutputStreamPtr pOutputStream, const Core::ErrorStreamPtr pErrorStream, const std::string_view header);
    virtual ~Parser();

    bool parse();
    virtual void subParserFinished(ISubParser* const pISubParser) override;

protected:
    /* This function reads all rows from input file starting with the second one (a.k.a. payload) and prepares them for parsing. */
    virtual void _readPayload() = 0;

    /* This function parses all rows read by previous function and generates intermediary data to be used for generating the output file. */
    virtual bool _parseInput() = 0;

    /* This function builds the rows to be written into the output file. */
    virtual void _buildOutput() = 0;

    /* This function resets the internal parser state after each parsing session. */
    virtual void _reset();

    /* Used for requesting error logging from error handler (force: used for non-localized error, e.g. empty file) */
    ErrorPtr _logError(const Core::Error_t errorCode, const size_t fileRowNumber, bool force = false);

    /* Used for storing locally generated errors
       Error location is setup at error storing point
    */
    void _storeParsingError(ErrorPtr pError);

    /* Used for logging all parsing errors to file */
    bool _logParsingErrorsToFile();

    /* Checks that input stream is at the end of file */
    bool _isInputStreamConsumed() const;

    /* Reads a payload line from input stream and appends it to input data */
    bool _readLineAndAppendToInput();

    /* Reads the first column from given row */
    bool _readFirstCell(const size_t rowIndex, std::string& firstCell);

    /* Reads current column from given row */
    bool _readCurrentCell(const size_t rowIndex, std::string& currentCell);

    /* Sets position and file column number to beginning of row */
    void _moveToInputRowStart(const size_t rowIndex);

    /* Increments file column number (position updated separately) */
    void _moveToNextInputColumn(const size_t rowIndex);

    /* Returns substring consisting of columns not yet parsed (from given row) */
    std::string _getUnparsedCellsContent(const size_t rowIndex) const;

    /* Checks if current position index is valid for the given row */
    bool _isValidCurrentPosition(const size_t rowIndex) const;

    /* Provides the number of input rows (payload) */
    size_t _getInputRowsCount() const;

    /* Appends the row content to output once or multiple times (no operation if timesToAppend is 0) */
    void _appendRowToOutput(const std::string& rowContent, const size_t timesToAppend = 1);

    /* Provides access to error handling mechanism to sub-parser */
    void _registerSubParser(ISubParser* const pISubParser);

    /* Activates the subparser so the it can start doing its part */
    bool _activateSubParser(const size_t rowIndex, const size_t subParserIndex);

    /* Launches sub-parsing for given sub-parser */
    void _doSubParsing(const size_t rowIndex, const size_t subParserIndex, std::vector<ErrorPtr>& parsingErrors);

    /* Used by derived classes to access the requested subparser for specific jobs */
    ISubParser* _getSubParser(const size_t rowIndex, const size_t subParserIndex) const;

    /* Checks if any parsing errors occured (either from parser or from sub-parser) */
    bool _parsingErrorsExist() const;

private:
    /* This function reads all rows (header and payload) from input file. */
    void _readInput();

    /* This function reads the first (header) row from input file and discards it. */
    void _readHeader();

    /* This function writes the resulting output to file. */
    void _writeOutput();

    /* Retrieves relevant data resulted from subparser work: current position, file column number, etc */
    void _retrieveRequiredDataFromSubParser(const ISubParser* const pISubParser);

    /* Checks if the subparser is registered within parser and has valid row number (otherwise it cannot be used) */
    bool _isValidSubParser(const ISubParser* const pISubParser) const;

    /* Deallocates all registered subparsers once Parser gets destroyed (once registered their ownership is assumed by Parser) */
    void _destroySubParsers();

    /* Information required for parsing a csv row */
    struct ParsedRowInfo
    {
        ParsedRowInfo() = delete;
        ParsedRowInfo(const std::string_view rowData);

        std::string mRowData; // input row (payload) data
        Core::Index_t mCurrentPosition; // current character index in the input string
        size_t mFileColumnNumber; // csv column number
        std::vector<ISubParser*> mRegisteredSubParsers; // subparsers used on the row
        bool mIsSubParserActive; // checks if a subparser is active on the row (only one subparser can be active on each row)
    };

    using ParserInput = std::vector<ParsedRowInfo>;

    /* Output created by parser (data rows + header) */
    struct ParserOutput
    {
        ParserOutput() = delete;
        ParserOutput(const std::string_view header);

        std::vector<std::string> mData; // stores final data (.csv rows) to be written to output file
        const std::string mHeader; // header (.csv row) to be written to output file
    };

    /* file streams used by parser, each one should correspond to a file that had been previously correctly opened */
    const Core::InputStreamPtr mpInputStream;
    const Core::OutputStreamPtr mpOutputStream;
    const Core::ErrorStreamPtr mpErrorStream;

    /* required info for parsing all (payload) input rows */
    ParserInput mParserInput;

    /* parsing output (header and data) */
    ParserOutput mParserOutput;

    /* gather all parsing errors here and write them to output file once parsing is complete (if any errors) */
    std::vector<ErrorPtr> mParsingErrors;

    /* error handler used for creating the objects that are responsible for logging the parsing errors to file */
    ErrorHandlerPtr mpErrorHandler;

    /* determines if the parser state needs to be reset before executing a parsing session */
    bool mIsResetRequired;
};

using ParserPtr = std::unique_ptr<Parser>;

#endif // PARSER_H
