#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>

#include "coreutils.h"
#include "errorhandler.h"
#include "iparser.h"

#ifdef _WIN32
#include "auxdata.h"
#endif

class ISubParser;

/* This is a generic parser class for .csv files.
   The content of an input .csv file is read, parsed and the resulting output written to an output .csv file.
   If errors occur they are being logged to an error file. In this case the output file stays empty.
*/
class Parser : public IParser
{
public:
    Parser(const InputStreamPtr pInputStream, const OutputStreamPtr pOutputStream, const ErrorStreamPtr pErrorStream, const std::string_view header);
    virtual ~Parser();

    virtual bool parse();
    virtual void subParserFinished(ISubParser* const pISubParser) override;

protected:
    /* This function reads all rows (header and payload) from input file. */
    virtual void _readInput();

    /* This function reads the first (header) row from input file and discards it. */
    virtual void _readHeader();

    /* This function reads all rows from input file starting with the second one (a.k.a. payload) and prepares them for parsing. */
    virtual void _readPayload() = 0;

    /* This function parses all rows read by previous function and generates intermediary data to be used for generating the output file. */
    virtual bool _parseInput() = 0;

    /* This function builds the rows to be written into the output file. */
    virtual void _buildOutput() = 0;

    /* This function writes the resulting output to file. */
    virtual void _writeOutput();

    /* This function resets the internal parser state after each parsing session. */
    virtual void _reset();

    /* Used for requesting error logging from error handler */
    virtual ErrorPtr _logError(const Error_t errorCode, const size_t fileRowNumber);

    /* Used for storing locally generated errors
       Error location is setup at error storing point
    */
    virtual void _storeParsingError(ErrorPtr pError);

    /* Used for logging all parsing errors to file */
    virtual bool _logParsingErrorsToFile();

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

    /* Provides the whole input (payload) row content */
    std::string _getInputRowContent(const size_t rowIndex) const;

    /* Returns substring consisting of columns not yet parsed (from given row) */
    std::string _getUnparsedCellsContent(const size_t rowIndex) const;

    /* Checks if the input row has been entirely parsed */
    bool _isEndOfInputDataRow(const size_t rowIndex) const;

    /* Checks if current position index is valid for the given row */
    bool _isValidCurrentPosition(const size_t rowIndex) const;

    /* Provides the number of input rows (payload) */
    size_t _getInputRowsCount() const;

    /* Appends the row content to output */
    void _appendRowToOutput(const std::string& rowContent);

    /* Provides access to error handling mechanism to sub-parser */
    void _registerSubParser(ISubParser* pISubParser);

    /* Checks if any parsing errors occured (either from parser or from sub-parser) */
    bool _parsingErrorsExist() const;

private:
    /* Methods used for communication with sub-parser (pass - retrieve mechanism) */
    void _passFileColumnNumberToSubParser(ISubParser* const pISubParser);
    void _retrieveFileColumnNumberFromSubParser(const ISubParser* const pISubParser);
    void _passCurrentPositionToSubParser(ISubParser* const pISubParser);
    void _retrieveCurrentPositionFromSubParser(const ISubParser* const pISubParser);

    /* file streams used by parser, each one should correspond to a file that had been previously correctly opened */
    const InputStreamPtr mpInputStream;
    const OutputStreamPtr mpOutputStream;
    const ErrorStreamPtr mpErrorStream;

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
    std::vector<Index_t> mCurrentPositions;

    /* current CSV column being parsed (the numbering starts from 1 as when opening the CSV with a spreadsheet tool) */
    std::vector<size_t> mFileColumnNumbers;

    /* error handler used for creating the objects that are responsible for logging the parsing errors to file */
    std::shared_ptr<ErrorHandler> mpErrorHandler;
};

using ParserPtr = std::unique_ptr<Parser>;

#endif // PARSER_H
