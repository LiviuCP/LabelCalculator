#include <cassert>

#include "labelutils.h"
#include "parser.h"

Parser::Parser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream, const std::string& header)
    : mpInputStream{pInputStream}
    , mpOutputStream{pOutputStream}
    , mpErrorStream{pErrorStream}
    , mOutputHeader{header}
    , mIsResetRequired{false}
    , mCurrentPosition{-1}
    , mCurrentColumnNumber{1}
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

void Parser::_writeOutput()
{
    assert(mpOutputStream->is_open());

    *mpOutputStream << mOutputHeader << std::endl;

    for (const auto& payloadRow : mOutputData)
    {
        *mpOutputStream << payloadRow << std::endl;
    }
}

void Parser::_reset()
{
    mInputData.clear();
    mOutputData.clear();
}

void Parser::_storeParsingErrorAndLocation(ErrorPtr pError)
{
    if (nullptr != pError)
    {
        mParsingErrors.push_back(pError);
    }
}

bool Parser::_logParsingErrorsToFile()
{
    const bool c_ParsingErrorsOccurred{mParsingErrors.size() > 0};

    if (c_ParsingErrorsOccurred)
    {
        for(auto& pError : mParsingErrors)
        {
            pError->execute();
        }
    }

    return c_ParsingErrorsOccurred;
}
