#ifndef ERRORTYPES_H
#define ERRORTYPES_H

#include "error.h"

class EmptyCellError final : public Error
{
public:
    EmptyCellError(std::ofstream& errorStream);

    void execute();
};

class ExceedingCharsCountError final : public Error
{
public:
    ExceedingCharsCountError(std::ofstream& errorStream, size_t maxAllowedCharsCount, ssize_t deltaCharsCount, bool isSourceDevice);

    void execute();

private:
    size_t mMaxAllowedCharsCount;
    ssize_t mDeltaCharsCount;
    bool mIsSourceDevice;
};

// unknown device entered in connection definitions/input file
class UnknownDeviceError final : public Error
{
public:
    UnknownDeviceError(std::ofstream& errorStream);

    void execute();
};

// fewer input cells than required (some are empty) in connection input file
class FewerCellsError final : public Error
{
public:
    FewerCellsError(std::ofstream& errorStream);

    void execute();
};

// wrong connection format in connection definitions file
class WrongFormatError final : public Error
{
public:
    WrongFormatError(std::ofstream& errorStream);

    void execute();
};

// invalid device placement U number in connection definitions file
class WrongUNumberError final : public Error
{
public:
    WrongUNumberError(std::ofstream& errorStream);

    void execute();
};

// device mentioned in connection definitions file not contained in the mapping table (connection to nothing)
class InvalidTargetDevicePositionError final : public Error
{
public:
    InvalidTargetDevicePositionError(std::ofstream& errorStream);

    void execute();
};

// device mentioned in connection definitions file is connected to itself
class DeviceConnectedToItselfError final : public Error
{
public:
    DeviceConnectedToItselfError(std::ofstream& errorStream);

    void execute();
};

// device marked as connected, number of connections mentioned in connection definitions file is 0
class NoConnectionsError final : public Error
{
public:
    NoConnectionsError(std::ofstream& errorStream);

    void execute();
};

// invalid characters contained in CSV cell
class InvalidCharactersError final : public Error
{
public:
    InvalidCharactersError(std::ofstream& errorStream);

    void execute();
};

// invalid characters contained in CSV cell
class InvalidDeviceUPositionError final : public Error
{
public:
    InvalidDeviceUPositionError(std::ofstream& errorStream);

    void execute();
};

#endif // ERRORTYPES_H
