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

    void setRow(int row);
    void setColumn(int column);

    int getRow() const;
    int getColumn() const;

protected:
    std::ofstream& mErrorStream;
    ErrorCode mErrorCode;
    int mRow;
    int mColumn;
};

using ErrorPtr = std::shared_ptr<Error>;

#endif // ERROR_H
