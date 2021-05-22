#ifndef ERROR_H
#define ERROR_H

#include <fstream>
#include <memory>

#include "labelutils.h"

class Error
{
public:
    Error(ErrorCode errorCode, std::ofstream& errorStream);
    virtual ~Error();

    virtual void execute();

    void setCSVRowNumber(size_t rowNumber);
    void setCSVColumnNumber(size_t columnNumber);

    size_t getCSVRowNumber() const;
    size_t getCSVColumnNumber() const;

protected:
    std::ofstream& mErrorStream;
    ErrorCode mErrorCode;
    size_t mCSVRowNumber;              // row from parsed CSV file where the error is located
    size_t mCSVColumnNumber;           // column from parsed CSV file where the error is located
};

using ErrorPtr = std::shared_ptr<Error>;

#endif // ERROR_H
