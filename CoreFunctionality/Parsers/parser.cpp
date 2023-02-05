#include <cassert>

#include "parser.h"

Parser::Parser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream, const std::string& header)
    : mpInputStream{pInputStream}
    , mpOutputStream{pOutputStream}
    , mpErrorStream{pErrorStream}
    , mOutputHeader{header}
    , mIsResetRequired{false}
    , mFileColumnNumber{1}
    , mpErrorHandler{nullptr}
{
    assert(nullptr != mpInputStream   &&
           mpInputStream->is_open());

    assert(nullptr != mpOutputStream  &&
           mpOutputStream->is_open());

    assert(nullptr != mpErrorStream   &&
           mpErrorStream->is_open());
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

void Parser::_readInput()
{
    _readHeader();
    _readPayload();
}

void Parser::_readHeader()
{
    mpInputStream->seekg(0);
    std::string header;
    getline(*mpInputStream, header);
}

void Parser::_writeOutput()
{
    assert(mpOutputStream->is_open());

    *mpOutputStream << mOutputHeader << "\n";

    for (const auto& payloadRow : mOutputData)
    {
        *mpOutputStream << payloadRow << "\n";
    }
}

void Parser::_reset()
{
    mInputData.clear();
    mOutputData.clear();
}

void Parser::_initializeErrorHandler()
{
    if (nullptr == mpErrorHandler.get())
    {
        mpErrorHandler = std::make_unique<ErrorHandler>();
    }
    else
    {
        mpErrorHandler->reset();
    }
}

void Parser::_storeParsingError(ErrorPtr pError)
{
    if (nullptr != pError)
    {
        mParsingErrors.push_back(pError);
    }
}

bool Parser::_logParsingErrorsToFile()
{
    for(auto& pError : mParsingErrors)
    {
        if (nullptr != pError.get())
        {
            pError->execute();
        }
    }

    const bool c_ParsingErrorsOccurred{mParsingErrors.size() > 0};

    return c_ParsingErrorsOccurred;
}
