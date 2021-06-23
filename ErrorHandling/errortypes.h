#ifndef ERRORTYPES_H
#define ERRORTYPES_H

#include "error.h"

class EmptyCellError final : public Error
{
public:
    EmptyCellError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream);

    virtual void execute() override;
};

class ExceedingCharsCountError final : public Error
{
public:
    ExceedingCharsCountError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream, size_t maxAllowedCharsCount, ssize_t deltaCharsCount, bool isSourceDevice);

    virtual void execute() override;

private:
    size_t mMaxAllowedCharsCount;
    ssize_t mDeltaCharsCount;
    bool mIsSourceDevice;
};

// unknown device entered in connection definitions/input file
class UnknownDeviceError final : public Error
{
public:
    UnknownDeviceError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream);

    virtual void execute() override;
};

// fewer input cells than required (some are empty) in connection input file
class FewerCellsError final : public Error
{
public:
    FewerCellsError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream);

    virtual void execute() override;
};

// wrong connection format in connection definitions file
class WrongConnectionFormatError final : public Error
{
public:
    WrongConnectionFormatError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream);

    virtual void execute() override;
};

// invalid device placement U number in connection definitions file
class DeviceUPositionOutOfRangeError final : public Error
{
public:
    DeviceUPositionOutOfRangeError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream);

    virtual void execute() override;
};

// device mentioned in connection definitions file not contained in the mapping table (connection to nothing)
class TargetDeviceNotFoundError final : public Error
{
public:
    TargetDeviceNotFoundError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream);

    virtual void execute() override;
};

// device mentioned in connection definitions file is connected to itself
class DeviceConnectedToItselfError final : public Error
{
public:
    DeviceConnectedToItselfError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream);

    virtual void execute() override;
};

// device marked as connected, number of connections mentioned in connection definitions file is 0
class NoConnectionsError final : public Error
{
public:
    NoConnectionsError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream);

    virtual void execute() override;
};

// invalid characters contained in CSV cell
class InvalidCharactersError final : public Error
{
public:
    InvalidCharactersError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream);

    virtual void execute() override;
};

// invalid value of the device U position (either invalid characters or not in the required range)
class InvalidUPositionValueError final : public Error
{
public:
    InvalidUPositionValueError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream);

    virtual void execute() override;
};

class EmptyConnectionsInputFileError final : public Error
{
public:
    EmptyConnectionsInputFileError(std::ofstream& errorStream);

    virtual void execute() override;
};

class NoConnectionsDefinedError final : public Error
{
public:
    NoConnectionsDefinedError(std::ofstream& errorStream);

    virtual void execute() override;
};

#endif // ERRORTYPES_H
