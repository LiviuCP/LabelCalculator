#include <algorithm>
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
    _destroySubParsers();
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
    if (pISubParser && _isSubParserRegistered(pISubParser))
    {
        _retrieveRequiredDataFromSubParser(pISubParser);
        mIsSubParserActiveOnRow[pISubParser->getFileRowNumber() - 2] = false;
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
    mParsingErrors.clear();
    mCurrentPositions.clear();
    mFileColumnNumbers.clear();
    mRegisteredSubParsers.clear();
    mIsSubParserActiveOnRow.clear();
}

ErrorPtr Parser::_logError(const Error_t errorCode, const size_t fileRowNumber, bool force)
{
    ErrorPtr result{nullptr};

    if (mpErrorHandler)
    {
        if (force)
        {
            result = mpErrorHandler->logError(errorCode, fileRowNumber, 1);
        }
        else if (fileRowNumber > 1u)
        {
            // file row numbering starts at 1 and the first row is reserved for the header (so payload rows start at 2)
            if (const size_t c_RowIndex{fileRowNumber - 2}; c_RowIndex < mInputData.size())
            {
                result = mpErrorHandler->logError(errorCode, fileRowNumber, mFileColumnNumbers[c_RowIndex]);
            }
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
            mRegisteredSubParsers.push_back({}); // the container should be updated even if there are no sub-parsers (Parser cannot know if the derived class sub-parsers)
            mIsSubParserActiveOnRow.push_back(false); // by default no subparser is active on any of the rows (once a subparser is activated the flag is set)
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
        // from Parser point of view position (index) 0 is considered a valid index even when the string is empty
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
    // from Parser point of view the index equal to string length is considered valid (it's similar to the end() iterator)
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

void Parser::_registerSubParser(ISubParser* const pISubParser)
{
    if (pISubParser)
    {
        // file row numbering starts at 1 and the first row is reserved for the header (so payload rows start at 2)
        if (const size_t c_FileRowNumber{pISubParser->getFileRowNumber()}; c_FileRowNumber > 1u)
        {
            if (const size_t c_RowIndex{c_FileRowNumber - 2}; c_RowIndex < mInputData.size())
            {
                mRegisteredSubParsers[c_RowIndex].push_back(pISubParser);
                pISubParser->setSubParserObserver(this);
                pISubParser->setErrorHandler(mpErrorHandler);
            }
        }
    }
}

bool Parser::_activateSubParser(const size_t rowIndex, const size_t subParserIndex)
{
    bool success{false};

    if (rowIndex < mRegisteredSubParsers.size() && subParserIndex < mRegisteredSubParsers[rowIndex].size())
    {
        if (ISubParser* const pISubParser{mRegisteredSubParsers[rowIndex][subParserIndex]}; pISubParser && !mIsSubParserActiveOnRow[rowIndex])
        {
            std::string_view dataToPass{mInputData[rowIndex]};
            dataToPass.remove_prefix(mCurrentPositions[rowIndex].value());

            pISubParser->init();
            pISubParser->setRawInputData(dataToPass);
            pISubParser->setFileColumnNumber(mFileColumnNumbers[rowIndex]);

            success = true;
        }
    }

    return success;
}

void Parser::_doSubParsing(const size_t rowIndex, const size_t subParserIndex, std::vector<ErrorPtr>& parsingErrors)
{
    if (const bool c_SubParserActivated{_activateSubParser(rowIndex, subParserIndex)}; c_SubParserActivated)
    {
        mRegisteredSubParsers[rowIndex][subParserIndex]->parseInputData(parsingErrors);
    }
}

ISubParser* Parser::_getSubParser(const size_t rowIndex, const size_t subParserIndex) const
{
    ISubParser* pISubParser{nullptr};

    if (rowIndex < mRegisteredSubParsers.size() && subParserIndex < mRegisteredSubParsers[rowIndex].size())
    {
        pISubParser = mRegisteredSubParsers[rowIndex][subParserIndex];
    }

    return pISubParser;
}

bool Parser::_parsingErrorsExist() const
{
    return !mParsingErrors.empty();
}

void Parser::_retrieveRequiredDataFromSubParser(const ISubParser* const pISubParser)
{
    if (pISubParser && _isSubParserRegistered(pISubParser))
    {
        // file row numbering starts at 1 and the first row is reserved for the header (so payload rows start at 2)
        if (const size_t c_FileRowNumber{pISubParser->getFileRowNumber()}; c_FileRowNumber > 1u)
        {
            const size_t c_RowIndex{c_FileRowNumber - 2};

            // parsing goes from beginning to the end of the string so the resulting column number should never be lower than the initial one
            if (const size_t c_FileColumnNumber{pISubParser->getFileColumnNumber()}; c_FileColumnNumber >= mFileColumnNumbers[c_RowIndex])
            {
                mFileColumnNumbers[c_RowIndex] = c_FileColumnNumber;
            }

            if (const Index_t c_CurrentPosition{pISubParser->getCurrentPosition()}; _isValidCurrentPosition(c_RowIndex) && c_CurrentPosition.has_value())
            {
                // parsing goes from beginning to the end of the string so the resulting position should never be lower than the initial one
                if (const size_t c_NewCurrentPosition{mCurrentPositions[c_RowIndex].value() + c_CurrentPosition.value()};
                    c_NewCurrentPosition <= mInputData[c_RowIndex].size())
                {
                    mCurrentPositions[c_RowIndex] = c_NewCurrentPosition;
                }
            }
        }
    }
}

bool Parser::_isSubParserRegistered(const ISubParser* const pISubParser) const
{
    bool isRegistered{false};

    if (pISubParser)
    {
        // file row numbering starts at 1 and the first row is reserved for the header (so payload rows start at 2)
        if (const size_t c_FileRowNumber{pISubParser->getFileRowNumber()}; c_FileRowNumber > 1u)
        {
            if (const size_t c_RowIndex{c_FileRowNumber - 2}; c_RowIndex < mInputData.size())
            {
                isRegistered = (mRegisteredSubParsers[c_RowIndex].cend() != std::find(mRegisteredSubParsers[c_RowIndex].cbegin(), mRegisteredSubParsers[c_RowIndex].cend(), pISubParser));
            }
        }
    }

    return isRegistered;
}

void Parser::_destroySubParsers()
{
    for (auto& row : mRegisteredSubParsers)
    {
        for (auto& pSubParser : row)
        {
            if (pSubParser)
            {
                delete pSubParser;
                pSubParser = nullptr;
            }
        }
    }
}
