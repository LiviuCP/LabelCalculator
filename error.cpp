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

void Error::setRow(int row)
{
    mRow = row;
}

void Error::setColumn(int column)
{
    mColumn = column;
}

int Error::getRow() const
{
    return mRow;
}

int Error::getColumn() const
{
    return mColumn;
}
