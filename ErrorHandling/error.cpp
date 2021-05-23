#include <cassert>

#include "error.h"

Error::Error(const ErrorCode errorCode, const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : mErrorCode{errorCode}
    , mFileRowNumber{fileRowNumber}
    , mFileColumnNumber{fileColumnNumber}
    , mErrorStream{errorStream}
{
    const unsigned short c_ErrorCode{static_cast<unsigned short>(mErrorCode)};

    assert(c_ErrorCode > 0u &&
           c_ErrorCode < static_cast<unsigned short>(ErrorCode::ErrorCodesUpperBound));

    assert(mFileRowNumber > 0u &&
           mFileColumnNumber > 0u);

    assert(mErrorStream.is_open());
}

Error::~Error()
{
}

void Error::execute()
{
    mErrorStream << "Error code: " << static_cast<int>(mErrorCode) << std::endl << std::endl;
}
