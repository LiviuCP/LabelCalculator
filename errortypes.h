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
    ExceedingCharsCountError(std::ofstream& errorStream, int maxAllowedNrOfChars, int deltaNrOfChars, bool isSourceDevice);

    void execute();

private:
    int mMaxAllowedNrOfChars;
    int mDeltaNrOfChars;
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
class NoDevicePresentError final : public Error
{
public:
    NoDevicePresentError(std::ofstream& errorStream);

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

#endif // ERRORTYPES_H
