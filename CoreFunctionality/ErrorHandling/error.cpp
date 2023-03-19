#include <cassert>

#include "errorcodes.h"
#include "error.h"

Error::Error(Error_t errorCode, const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : mErrorCode{errorCode}
    , mFileRowNumber{fileRowNumber}
    , mFileColumnNumber{fileColumnNumber}
    , mErrorStream{errorStream}
{
    const Error_t c_ErrorCode{static_cast<Error_t>(mErrorCode)};

    assert(c_ErrorCode > 0u &&
           c_ErrorCode < static_cast<Error_t>(ErrorCode::ErrorCodesUpperBound));

    assert(mFileRowNumber > 0u &&
           mFileColumnNumber > 0u);

    assert(mErrorStream.is_open());
}

Error::~Error()
{
}

void Error::execute()
{
    mErrorStream << "Error code: " << static_cast<int>(mErrorCode) << "\n\n";
}

Error_t Error::getErrorCode() const
{
    return mErrorCode;
}

void Error::_logMessage(std::string_view message, bool shouldPrependErrorLabel)
{
    if (shouldPrependErrorLabel)
    {
        mErrorStream << "Error: ";
    }

    mErrorStream << message << "\n";
}

void Error::_logRowAndColumnNumber()
{
    mErrorStream << "Row number: " << mFileRowNumber << "    " << "Column number: " << mFileColumnNumber << "\n";
}

void Error::_logRowNumber()
{
    mErrorStream << "Row number: " << mFileRowNumber << "\n";
}
