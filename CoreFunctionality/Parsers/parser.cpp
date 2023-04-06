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
    , mParserOutput{header}
    , mpErrorHandler{nullptr}
    , mIsResetRequired{false}
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

        // file row numbering starts at 1 and the first row is reserved for the header (so payload rows start at 2)
        const size_t c_RowIndex{pISubParser->getFileRowNumber() - 2};
        mParserInput[c_RowIndex].mIsSubParserActive = false;
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
        *mpOutputStream << mParserOutput.mHeader << "\n";

        for (const auto& payloadRow : mParserOutput.mData)
        {
            *mpOutputStream << payloadRow << "\n";
        }
    }
}

void Parser::_reset()
{
    mParserInput.clear();
    mParserOutput.mData.clear();
    mParsingErrors.clear();
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
            if (const size_t c_RowIndex{fileRowNumber - 2}; c_RowIndex < mParserInput.size())
            {
                result = mpErrorHandler->logError(errorCode, fileRowNumber, mParserInput[c_RowIndex].mFileColumnNumber);
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
            mParserInput.emplace_back(input);
            success = true;
        }
    }

    return success;
}

bool Parser::_readFirstCell(const size_t rowIndex, std::string& firstCell)
{
    bool success{false};

    if (rowIndex < mParserInput.size())
    {
        // from Parser point of view position (index) 0 is considered a valid index even when the string is empty
        mParserInput[rowIndex].mCurrentPosition = Core::readDataField(mParserInput[rowIndex].mRowData, firstCell, 0);
        success = true;
    }

    return success;
}

bool Parser::_readCurrentCell(const size_t rowIndex, std::string& currentCell)
{
    bool success{false};

    if (_isValidCurrentPosition(rowIndex))
    {
        mParserInput[rowIndex].mCurrentPosition = Core::readDataField(mParserInput[rowIndex].mRowData, currentCell, mParserInput[rowIndex].mCurrentPosition);
        success = true;
    }

    return success;
}

void Parser::_moveToInputRowStart(const size_t rowIndex)
{
    if (rowIndex < mParserInput.size())
    {
        mParserInput[rowIndex].mCurrentPosition = 0u;
        mParserInput[rowIndex].mFileColumnNumber = 1u;
    }
}

void Parser::_moveToNextInputColumn(const size_t rowIndex)
{
    if (rowIndex < mParserInput.size())
    {
        ++mParserInput[rowIndex].mFileColumnNumber;
    }
}

std::string Parser::_getUnparsedCellsContent(const size_t rowIndex) const
{
    std::string result;

    if (_isValidCurrentPosition(rowIndex))
    {
        result.append(mParserInput[rowIndex].mRowData.substr(mParserInput[rowIndex].mCurrentPosition.value()));
    }

    return result;
}

bool Parser::_isEndOfInputDataRow(const size_t rowIndex) const
{
    bool result{false};

    if (_isValidCurrentPosition(rowIndex))
    {
        result = (mParserInput[rowIndex].mRowData.size() == mParserInput[rowIndex].mCurrentPosition);
    }

    return result;
}

bool Parser::_isValidCurrentPosition(const size_t rowIndex) const
{
    // from Parser point of view the index equal to string length is considered valid (it's similar to the end() iterator)
    return (rowIndex < mParserInput.size() &&
            mParserInput[rowIndex].mCurrentPosition.has_value() &&
            mParserInput[rowIndex].mCurrentPosition <= mParserInput[rowIndex].mRowData.size());
}

size_t Parser::_getInputRowsCount() const
{
    return mParserInput.size();
}

void Parser::_appendRowToOutput(const std::string& rowContent)
{
    mParserOutput.mData.push_back(rowContent);
}

void Parser::_registerSubParser(ISubParser* const pISubParser)
{
    if (pISubParser)
    {
        // file row numbering starts at 1 and the first row is reserved for the header (so payload rows start at 2)
        if (const size_t c_FileRowNumber{pISubParser->getFileRowNumber()}; c_FileRowNumber > 1u)
        {
            if (const size_t c_RowIndex{c_FileRowNumber - 2}; c_RowIndex < mParserInput.size())
            {
                mParserInput[c_RowIndex].mRegisteredSubParsers.push_back(pISubParser);
                pISubParser->setSubParserObserver(this);
                pISubParser->setErrorHandler(mpErrorHandler);
            }
        }
    }
}

bool Parser::_activateSubParser(const size_t rowIndex, const size_t subParserIndex)
{
    bool success{false};

    if (rowIndex < mParserInput.size() && subParserIndex < mParserInput[rowIndex].mRegisteredSubParsers.size())
    {
        if (ISubParser* const pISubParser{mParserInput[rowIndex].mRegisteredSubParsers[subParserIndex]};
            pISubParser && !mParserInput[rowIndex].mIsSubParserActive)
        {
            std::string_view dataToPass{mParserInput[rowIndex].mRowData};
            dataToPass.remove_prefix(mParserInput[rowIndex].mCurrentPosition.value());

            pISubParser->init();
            pISubParser->setRawInputData(dataToPass);
            pISubParser->setFileColumnNumber(mParserInput[rowIndex].mFileColumnNumber);

            success = true;
        }
    }

    return success;
}

void Parser::_doSubParsing(const size_t rowIndex, const size_t subParserIndex, std::vector<ErrorPtr>& parsingErrors)
{
    if (const bool c_SubParserActivated{_activateSubParser(rowIndex, subParserIndex)}; c_SubParserActivated)
    {
        mParserInput[rowIndex].mRegisteredSubParsers[subParserIndex]->parseInputData(parsingErrors);
    }
}

ISubParser* Parser::_getSubParser(const size_t rowIndex, const size_t subParserIndex) const
{
    ISubParser* pISubParser{nullptr};

    if (rowIndex < mParserInput.size() && subParserIndex < mParserInput[rowIndex].mRegisteredSubParsers.size())
    {
        pISubParser = mParserInput[rowIndex].mRegisteredSubParsers[subParserIndex];
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
            if (const size_t c_FileColumnNumber{pISubParser->getFileColumnNumber()};
                c_FileColumnNumber >= mParserInput[c_RowIndex].mFileColumnNumber)
            {
                mParserInput[c_RowIndex].mFileColumnNumber = c_FileColumnNumber;
            }

            if (const Index_t c_CurrentPosition{pISubParser->getCurrentPosition()};
                _isValidCurrentPosition(c_RowIndex) && c_CurrentPosition.has_value())
            {
                // parsing goes from beginning to the end of the string so the resulting position should never be lower than the initial one
                if (const size_t c_NewCurrentPosition{mParserInput[c_RowIndex].mCurrentPosition.value() + c_CurrentPosition.value()};
                    c_NewCurrentPosition <= mParserInput[c_RowIndex].mRowData.size())
                {
                    mParserInput[c_RowIndex].mCurrentPosition = c_NewCurrentPosition;
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
            if (const size_t c_RowIndex{c_FileRowNumber - 2}; c_RowIndex < mParserInput.size())
            {
                isRegistered = (mParserInput[c_RowIndex].mRegisteredSubParsers.cend() != std::find(mParserInput[c_RowIndex].mRegisteredSubParsers.cbegin(), mParserInput[c_RowIndex].mRegisteredSubParsers.cend(), pISubParser));
            }
        }
    }

    return isRegistered;
}

void Parser::_destroySubParsers()
{
    for (auto& row : mParserInput)
    {
        for (auto& pSubParser : row.mRegisteredSubParsers)
        {
            if (pSubParser)
            {
                delete pSubParser;
                pSubParser = nullptr;
            }
        }
    }
}

Parser::ParsedRowInfo::ParsedRowInfo(const std::string_view rowData)
    : mRowData{rowData}
    , mCurrentPosition{std::nullopt}
    , mFileColumnNumber{1u}
    , mIsSubParserActive{false}
{
}

Parser::ParserOutput::ParserOutput(const std::string_view header)
    : mHeader{header}
{
}
