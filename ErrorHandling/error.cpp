#include <cassert>

#include "error.h"

Error::Error(ErrorCode errorCode, std::ofstream& errorStream)
    : mErrorStream{errorStream}
    , mErrorCode{errorCode}
{
    const unsigned short c_ErrorCode{static_cast<unsigned short>(mErrorCode)};

    assert(c_ErrorCode > 0u &&
           c_ErrorCode < static_cast<unsigned short>(ErrorCode::ErrorCodesUpperBound));

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
