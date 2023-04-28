#ifndef ERRORTYPES_H
#define ERRORTYPES_H

#include "error.h"

class EmptyCellError final : public Error
{
public:
    EmptyCellError(const size_t fileRowNumber, const size_t fileColumnNumber, const ErrorStreamPtr pErrorStream);
    virtual void execute() override;
};

// unknown device entered in connection definitions/input file
class UnknownDeviceError final : public Error
{
public:
    UnknownDeviceError(const size_t fileRowNumber, const size_t fileColumnNumber, const ErrorStreamPtr pErrorStream);
    virtual void execute() override;
};

// fewer input cells than required (some are empty) in connection input file
class FewerCellsError final : public Error
{
public:
    FewerCellsError(const size_t fileRowNumber, const size_t fileColumnNumber, const ErrorStreamPtr pErrorStream);
    virtual void execute() override;
};

// wrong connection format in connection definitions file
class InvalidConnectionFormatError final : public Error
{
public:
    InvalidConnectionFormatError(const size_t fileRowNumber, const size_t fileColumnNumber, const ErrorStreamPtr pErrorStream);
    virtual void execute() override;
};

// invalid device placement U number in connection definitions file
class DeviceUPositionOutOfRangeError final : public Error
{
public:
    DeviceUPositionOutOfRangeError(const size_t fileRowNumber, const size_t fileColumnNumber, const ErrorStreamPtr pErrorStream);
    virtual void execute() override;
};

// device mentioned in connection definitions file not contained in the mapping table (connection to nothing)
class TargetDeviceNotFoundError final : public Error
{
public:
    TargetDeviceNotFoundError(const size_t fileRowNumber, const size_t fileColumnNumber, const ErrorStreamPtr pErrorStream);
    virtual void execute() override;
};

// device mentioned in connection definitions file is connected to itself
class DeviceConnectedToItselfError final : public Error
{
public:
    DeviceConnectedToItselfError(const size_t fileRowNumber, const size_t fileColumnNumber, const ErrorStreamPtr pErrorStream);
    virtual void execute() override;
};

// device marked as connected, number of connections mentioned in connection definitions file is 0
class NullNrOfConnectionsError final : public Error
{
public:
    NullNrOfConnectionsError(const size_t fileRowNumber, const size_t fileColumnNumber, const ErrorStreamPtr pErrorStream);
    virtual void execute() override;
};

// invalid characters contained in CSV cell
class InvalidCharactersError final : public Error
{
public:
    InvalidCharactersError(const size_t fileRowNumber, const size_t fileColumnNumber, const ErrorStreamPtr pErrorStream);
    virtual void execute() override;
};

// invalid value of the device U position (either invalid characters or not in the required range)
class InvalidUPositionValueError final : public Error
{
public:
    InvalidUPositionValueError(const size_t fileRowNumber, const size_t fileColumnNumber, const ErrorStreamPtr pErrorStream);
    virtual void execute() override;
};

class EmptyConnectionInputFileError final : public Error
{
public:
    EmptyConnectionInputFileError(const ErrorStreamPtr pErrorStream);
    virtual void execute() override;
};

class NoConnectionsDefinedError final : public Error
{
public:
    NoConnectionsDefinedError(const ErrorStreamPtr pErrorStream);
    virtual void execute() override;
};

#endif // ERRORTYPES_H
