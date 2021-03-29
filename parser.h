#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>
#include <vector>
#include <memory>

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

    /* This function resets the internal parser state after each parsing session.
    */
    virtual void _reset();

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
};

using ParserPtr = std::unique_ptr<Parser>;

#endif // PARSER_H
