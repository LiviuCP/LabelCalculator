#include <cassert>

#include "coreutils.h"
#include "isubparser.h"
#include "parser.h"

namespace Core = Utilities::Core;

Parser::Parser(const InputStreamPtr pInputStream, const OutputStreamPtr pOutputStream, const ErrorStreamPtr pErrorStream, const std::string_view header)
    : mpInputStream{pInputStream}
    , mpOutputStream{pOutputStream}
    , mpErrorStream{pErrorStream}
    , mOutputHeader{header}
    , mIsResetRequired{false}
    , mpErrorHandler{nullptr}
{
    if (mpInputStream  && mpInputStream->is_open()  &&
        mpOutputStream && mpOutputStream->is_open() &&
        mpErrorStream  && mpErrorStream->is_open())
    {
        mpErrorHandler = std::make_shared<ErrorHandler>(mpErrorStream);
    }
    else
    {
        assert(false);
    }
}

Parser::~Parser()
{
}

bool Parser::parse()
{
    if (mIsResetRequired)
    {
        _reset(); // cleanup required after first usage
    }
    else
    {
        mIsResetRequired = true;
    }

    _readInput();

    const bool c_ParsingErrorsOccurred{_parseInput()};

    if(!c_ParsingErrorsOccurred)
    {
        _buildOutput();
        _writeOutput();
    }

    return c_ParsingErrorsOccurred;
}

void Parser::subParserFinished(ISubParser* const pISubParser)
{
    if (pISubParser)
    {
        _retrieveFileColumnNumberFromSubParser(pISubParser);
        _retrieveCurrentPositionFromSubParser(pISubParser);
    }
}

void Parser::_readInput()
{
    _readHeader();
    _readPayload();
}

void Parser::_readHeader()
{
    if (mpInputStream && mpInputStream->is_open())
    {
        mpInputStream->seekg(0);
        std::string header;
        getline(*mpInputStream, header);
    }
}

void Parser::_writeOutput()
{
    if (mpOutputStream && mpOutputStream->is_open())
    {
        *mpOutputStream << mOutputHeader << "\n";

        for (const auto& payloadRow : mOutputData)
        {
            *mpOutputStream << payloadRow << "\n";
        }
    }
}

void Parser::_reset()
{
    mInputData.clear();
    mOutputData.clear();
}

ErrorPtr Parser::_logError(const Error_t errorCode, const size_t fileRowNumber)
{
    ErrorPtr result{nullptr};

    // file row numbering starts at 1 and the first row is reserved for the header (so payload rows start at 2)
    if (mpErrorHandler && fileRowNumber > 1u)
    {
        if (const size_t c_RowIndex{fileRowNumber - 2}; c_RowIndex < mInputData.size())
        {
            result = mpErrorHandler->logError(errorCode, fileRowNumber, mFileColumnNumbers[c_RowIndex]);
        }
    }

    return result;
}

void Parser::_storeParsingError(ErrorPtr pError)
{
    if (pError)
    {
        mParsingErrors.push_back(pError);
    }
}

bool Parser::_logParsingErrorsToFile()
{
    for(auto& pError : mParsingErrors)
    {
        if (pError)
        {
            pError->execute();
        }
    }

    const bool c_ParsingErrorsOccurred{mParsingErrors.size() > 0};

    return c_ParsingErrorsOccurred;
}

bool Parser::_isInputStreamConsumed() const
{
    bool inputConsumed{true};

    if (mpInputStream && mpInputStream->is_open() && !mpInputStream->eof())
    {
        inputConsumed = false;
    }

    return inputConsumed;
}

bool Parser::_readLineAndAppendToInput()
{
    bool success{false};

    if (mpInputStream && mpInputStream->is_open() && !mpInputStream->eof())
    {
        std::string input;
        getline(*mpInputStream, input);

        if (Core::areParseableCharactersContained(input))
        {
            mInputData.push_back(input);
            mCurrentPositions.push_back(Index_t{});
            mFileColumnNumbers.push_back(1u); // initialize each column number with 1 (this is where column numbering always starts at)
            success = true;
        }
    }

    return success;
}

bool Parser::_readFirstCell(const size_t rowIndex, std::string& firstCell)
{
    bool success{false};

    if (rowIndex < mInputData.size())
    {
        mCurrentPositions[rowIndex] = Core::readDataField(mInputData[rowIndex], firstCell, 0);
        success = true;
    }

    return success;
}

bool Parser::_readCurrentCell(const size_t rowIndex, std::string& currentCell)
{
    bool success{false};

    if (_isValidCurrentPosition(rowIndex))
    {
        mCurrentPositions[rowIndex] = Core::readDataField(mInputData[rowIndex], currentCell, mCurrentPositions[rowIndex]);
        success = true;
    }

    return success;
}

void Parser::_moveToInputRowStart(const size_t rowIndex)
{
    if (rowIndex < mInputData.size())
    {
        mCurrentPositions[rowIndex] = 0u;
        mFileColumnNumbers[rowIndex] = 1u;
    }
}

void Parser::_moveToNextInputColumn(const size_t rowIndex)
{
    if (rowIndex < mInputData.size())
    {
        ++(mFileColumnNumbers[rowIndex]);
    }
}

std::string Parser::_getInputRowContent(const size_t rowIndex) const
{
    std::string result;

    if (rowIndex < mInputData.size())
    {
        result.append(mInputData[rowIndex]);
    }

    return result;
}

std::string Parser::_getUnparsedCellsContent(const size_t rowIndex) const
{
    std::string result;

    if (_isValidCurrentPosition(rowIndex))
    {
        result.append(mInputData[rowIndex].substr(mCurrentPositions[rowIndex].value()));
    }

    return result;
}

bool Parser::_isEndOfInputDataRow(const size_t rowIndex) const
{
    bool result{false};

    if (_isValidCurrentPosition(rowIndex))
    {
        result = (mInputData[rowIndex].size() == mCurrentPositions[rowIndex]);
    }

    return result;
}

bool Parser::_isValidCurrentPosition(const size_t rowIndex) const
{
    return (rowIndex < mInputData.size() && mCurrentPositions[rowIndex].has_value() && mCurrentPositions[rowIndex] <= mInputData[rowIndex].size());
}

size_t Parser::_getInputRowsCount() const
{
    return mInputData.size();
}

void Parser::_appendRowToOutput(const std::string& rowContent)
{
    mOutputData.push_back(rowContent);
}

void Parser::_registerSubParser(ISubParser* pISubParser)
{
    if (pISubParser)
    {
        pISubParser->setParentParser(this);
        pISubParser->setErrorHandler(mpErrorHandler);
        _passFileColumnNumberToSubParser(pISubParser);
        _passCurrentPositionToSubParser(pISubParser);
    }
}

bool Parser::_parsingErrorsExist() const
{
    return !mParsingErrors.empty();
}

void Parser::_passFileColumnNumberToSubParser(ISubParser* const pISubParser)
{
    if (pISubParser)
    {
        // file row numbering starts at 1 and the first row is reserved for the header (so payload rows start at 2)
        if (const size_t c_FileRowNumber{pISubParser->getFileRowNumber()}; c_FileRowNumber > 1u)
        {
            if (const size_t c_RowIndex{c_FileRowNumber - 2}; c_RowIndex < mInputData.size())
            {
                pISubParser->setFileColumnNumber(mFileColumnNumbers[c_RowIndex]);
            }
        }
    }
}

void Parser::_retrieveFileColumnNumberFromSubParser(const ISubParser* const pISubParser)
{
    if (pISubParser)
    {
        // file row numbering starts at 1 and the first row is reserved for the header (so payload rows start at 2)
        if (const size_t c_FileRowNumber{pISubParser->getFileRowNumber()},
                         c_FileColumnNumber{pISubParser->getFileColumnNumber()};
            c_FileRowNumber > 1u)
        {
            // parsing goes from beginning to the end of the string so the resulting column number should never be lower than the initial one
            if (const size_t c_RowIndex{c_FileRowNumber - 2}; c_RowIndex < mInputData.size() && c_FileColumnNumber >= mFileColumnNumbers[c_RowIndex])
            {

                mFileColumnNumbers[c_RowIndex] = c_FileColumnNumber;
            }
        }
    }
}

void Parser::_passCurrentPositionToSubParser(ISubParser* const pISubParser)
{
    if (pISubParser)
    {
        // file row numbering starts at 1 and the first row is reserved for the header (so payload rows start at 2)
        if (const size_t c_FileRowNumber{pISubParser->getFileRowNumber()}; c_FileRowNumber > 1u)
        {
            if (const size_t c_RowIndex{c_FileRowNumber - 2}; _isValidCurrentPosition(c_RowIndex))
            {
                pISubParser->setCurrentPosition(mCurrentPositions[c_RowIndex]);
            }
        }
    }
}

void Parser::_retrieveCurrentPositionFromSubParser(const ISubParser* const pISubParser)
{
    if (pISubParser)
    {
        // file row numbering starts at 1 and the first row is reserved for the header (so payload rows start at 2)
        if (const size_t c_FileRowNumber{pISubParser->getFileRowNumber()}; c_FileRowNumber > 1u)
        {
            const Index_t c_CurrentPosition{pISubParser->getCurrentPosition()};

            // parsing goes from beginning to the end of the string so the resulting position should never be lower than the initial one
            if (const size_t c_RowIndex{c_FileRowNumber - 2};
                c_RowIndex < mInputData.size() &&
                c_CurrentPosition.has_value() &&
                mCurrentPositions[c_RowIndex].has_value() &&
                c_CurrentPosition >= mCurrentPositions[c_RowIndex] &&
                c_CurrentPosition <= mInputData[c_RowIndex].size())
            {
                mCurrentPositions[c_RowIndex] = c_CurrentPosition;
            }
        }
    }
}
