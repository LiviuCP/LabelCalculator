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

    void setCSVRowNumber(int rowNumber);
    void setCSVColumnNumber(int columnNumber);

    int getCSVRowNumber() const;
    int getCSVColumnNumber() const;

protected:
    std::ofstream& mErrorStream;
    ErrorCode mErrorCode;
    int mCSVRowNumber;              // row from parsed CSV file where the error is located
    int mCSVColumnNumber;           // column from parsed CSV file where the error is located
};

using ErrorPtr = std::shared_ptr<Error>;

#endif // ERROR_H
