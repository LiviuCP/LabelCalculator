#include <cassert>

#include "labelutils.h"
#include "parser.h"

Parser::Parser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream, const std::string& header)
    : mpInputStream{pInputStream}
    , mpOutputStream{pOutputStream}
    , mpErrorStream{pErrorStream}
    , mOutputHeader{header}
    , mIsResetRequired{false}
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
        writeOutputToFile(*mpOutputStream, mOutputData, mOutputHeader);
    }

    return c_ParsingErrorsOccurred;
}

void Parser::_reset()
{
    mInputData.clear();
    mOutputData.clear();
}

void Parser::_storeParsingErrorAndLocation(ErrorPtr pError, const int rowNumber, const int columnNumber)
{
    if (nullptr != pError)
    {
        pError->setRow(rowNumber);
        pError->setColumn(columnNumber);
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
