#ifndef ERROR_H
#define ERROR_H

#include <fstream>
#include <memory>

#include "coreutils.h"

class Error
{
public:
    Error(Error_t errorCode, const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream);
    virtual ~Error();

    virtual void execute();

    Error_t getErrorCode() const;

protected:
    Error_t mErrorCode;
    size_t mFileRowNumber;              // row from parsed CSV file where the error is located
    size_t mFileColumnNumber;           // column from parsed CSV file where the error is located
    std::ofstream& mErrorStream;
};

using ErrorPtr = std::shared_ptr<Error>;

#endif // ERROR_H
