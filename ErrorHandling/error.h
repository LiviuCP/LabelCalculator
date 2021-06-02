#ifndef ERROR_H
#define ERROR_H

#include <fstream>
#include <memory>

enum class ErrorCode : unsigned short
{
    EMPTY_CELL = 1u,
    MAX_CHARS_EXCEEDED,
    UNKNOWN_DEVICE,
    FEWER_CELLS,
    WRONG_CONNECTION_FORMAT,
    PLACEMENT_OUT_OF_RANGE,
    INVALID_TARGET_DEVICE_POSITION,
    DEVICE_CONNECTED_TO_ITSELF,
    NULL_NR_OF_CONNECTIONS,
    INVALID_CHARS,
    INVALID_DEVICE_U_POSITION,
    EMPTY_CONNECTIONS_INPUT_FILE,
    NO_CONNECTED_DEVICES,
    ErrorCodesUpperBound
};

class Error
{
public:
    Error(const ErrorCode errorCode, const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream);
    virtual ~Error();

    virtual void execute();

protected:
    ErrorCode mErrorCode;
    size_t mFileRowNumber;              // row from parsed CSV file where the error is located
    size_t mFileColumnNumber;           // column from parsed CSV file where the error is located
    std::ofstream& mErrorStream;
};

using ErrorPtr = std::shared_ptr<Error>;

#endif // ERROR_H
