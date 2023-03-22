#include <cassert>

#include "errorcodes.h"
#include "error.h"

Error::Error(Error_t errorCode, const size_t fileRowNumber, const size_t fileColumnNumber, const ErrorStreamPtr pErrorStream)
    : mErrorCode{errorCode}
    , mFileRowNumber{fileRowNumber}
    , mFileColumnNumber{fileColumnNumber}
    , mpErrorStream{pErrorStream}
{
    const Error_t c_ErrorCode{static_cast<Error_t>(mErrorCode)};

    assert(c_ErrorCode > 0u &&
           c_ErrorCode < static_cast<Error_t>(ErrorCode::ErrorCodesUpperBound));

    assert(mFileRowNumber > 0u &&
           mFileColumnNumber > 0u);

    assert(mpErrorStream && mpErrorStream->is_open());
}

Error::~Error()
{
}

void Error::execute()
{
    if (mpErrorStream && mpErrorStream->is_open())
    {
        *mpErrorStream << "Error code: " << mErrorCode << "\n\n";
    }

}

Error_t Error::getErrorCode() const
{
    return mErrorCode;
}

void Error::_logMessage(std::string_view message, bool shouldPrependErrorLabel)
{
    if (mpErrorStream && mpErrorStream->is_open())
    {
        if (shouldPrependErrorLabel)
        {
            *mpErrorStream << "Error: ";
        }

        *mpErrorStream << message << "\n";
    }
}

void Error::_logRowAndColumnNumber()
{
    if (mpErrorStream && mpErrorStream->is_open())
    {
        *mpErrorStream << "Row number: " << mFileRowNumber << "    " << "Column number: " << mFileColumnNumber << "\n";
    }
}

void Error::_logRowNumber()
{
    if (mpErrorStream && mpErrorStream->is_open())
    {
        *mpErrorStream << "Row number: " << mFileRowNumber << "\n";
    }
}
