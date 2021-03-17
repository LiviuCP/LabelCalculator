#ifndef ERROR_H
#define ERROR_H

#include <fstream>

enum class ErrorCode
{
    EMPTY_CELL = 1,
    MAX_CHARS_EXCEEDED,
    UNKNOWN_DEVICE,
    FEWER_CELLS,
    WRONG_CONNECTION_FORMAT,
    PLACEMENT_OUT_OF_RANGE,
    NO_DEVICE_PLACED_IN_POSITION,
    DEVICE_CONNECTED_TO_ITSELF,
    NULL_NR_OF_CONNECTIONS,
    ErrorCodesCount
};

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

class EmptyCellError : public Error
{
public:
    EmptyCellError(std::ofstream& errorStream);

    void execute();
};

class ExceedingCharsCountError : public Error
{
public:
    ExceedingCharsCountError(std::ofstream& errorStream, int maxAllowedNrOfChars, int deltaNrOfChars, bool isSourceDevice);

    void execute();

protected:
    int mMaxAllowedNrOfChars;
    int mDeltaNrOfChars;
    bool mIsSourceDevice;
};

// unknown device entered in connection definitions/input file
class UnknownDeviceError : public Error
{
public:
    UnknownDeviceError(std::ofstream& errorStream);

    void execute();
};

// fewer input cells than required (some are empty) in connection input file
class FewerCellsError : public Error
{
public:
    FewerCellsError(std::ofstream& errorStream);

    void execute();
};

// wrong connection format in connection definitions file
class WrongFormatError : public Error
{
public:
    WrongFormatError(std::ofstream& errorStream);

    void execute();
};

// invalid device placement U number in connection definitions file
class WrongUNumberError : public Error
{
public:
    WrongUNumberError(std::ofstream& errorStream);

    void execute();
};

// device mentioned in connection definitions file not contained in the mapping table (connection to nothing)
class NoDevicePresentError : public Error
{
public:
    NoDevicePresentError(std::ofstream& errorStream);

    void execute();
};

// device mentioned in connection definitions file is connected to itself
class DeviceConnectedToItselfError : public Error
{
public:
    DeviceConnectedToItselfError(std::ofstream& errorStream);

    void execute();
};

// device marked as connected, number of connections mentioned in connection definitions file is 0
class NoConnectionsError : public Error
{
public:
    NoConnectionsError(std::ofstream& errorStream);

    void execute();
};

#endif // ERROR_H
