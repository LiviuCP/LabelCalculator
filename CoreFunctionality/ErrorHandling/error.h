#ifndef ERROR_H
#define ERROR_H

#include "coreutils.h"

class Error
{
public:
    Error(Error_t errorCode, const size_t fileRowNumber, const size_t fileColumnNumber, const ErrorStreamPtr pErrorStream);
    virtual ~Error();

    virtual void execute();

    Error_t getErrorCode() const;

protected:
    void _logMessage(std::string_view message, bool shouldPrependErrorLabel = false);
    void _logRowAndColumnNumber();
    void _logRowNumber();

private:
    Error_t mErrorCode;
    size_t mFileRowNumber;              // row from parsed CSV file where the error is located
    size_t mFileColumnNumber;           // column from parsed CSV file where the error is located
    const ErrorStreamPtr mpErrorStream;
};

using ErrorPtr = std::shared_ptr<Error>;

#endif // ERROR_H
