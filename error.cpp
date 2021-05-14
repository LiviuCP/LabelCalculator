#include <cassert>

#include "error.h"

Error::Error(ErrorCode errorCode, std::ofstream& errorStream)
    : mErrorStream{errorStream}
    , mErrorCode{errorCode}
{
    const int c_ErrorCode{static_cast<int>(mErrorCode)};

    assert(c_ErrorCode > 0 &&
           c_ErrorCode < static_cast<int>(ErrorCode::ErrorCodesCount));

    assert(mErrorStream.is_open());
}

Error::~Error()
{
}

void Error::execute()
{
    mErrorStream << "Error code: " << static_cast<int>(mErrorCode) << std::endl << std::endl;
}

void Error::setCSVRowNumber(size_t rowNumber)
{
    mCSVRowNumber = rowNumber;
}

void Error::setCSVColumnNumber(size_t columnNumber)
{
    mCSVColumnNumber = columnNumber;
}

size_t Error::getCSVRowNumber() const
{
    return mCSVRowNumber;
}

size_t Error::getCSVColumnNumber() const
{
    return mCSVColumnNumber;
}
