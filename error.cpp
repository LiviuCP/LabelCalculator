#include "error.h"

UnknownDeviceError::UnknownDeviceError()
{
    mErrorCode = 3;
    mRow = 0;
    mColumn = 0;
}

void UnknownDeviceError::execute(std::ofstream& err)
{
    handleError(err);
}

void UnknownDeviceError::buildDescriptionText()
{
    // not used
}
void UnknownDeviceError::buildLabelText()
{
    // not used
}
void UnknownDeviceError::parseInputData(const std::string&, int&, bool&, std::ofstream&)
{
    // not used
}

FewerCellsError::FewerCellsError()
{
    mErrorCode = 4;
    mRow = 0;
    mColumn = 0;
}

void FewerCellsError::execute(std::ofstream &err)
{
    handleError(err);
}

void FewerCellsError::buildDescriptionText()
{
    // not used
}

void FewerCellsError::buildLabelText()
{
    // not used
}

void FewerCellsError::parseInputData(const std::string&, int&, bool&, std::ofstream&)
{
    // not used
}

WrongFormatError::WrongFormatError()
{
    mErrorCode = 5;
    mRow = 0;
    mColumn = 0;
}

void WrongFormatError::execute(std::ofstream &err)
{
    handleError(err);
}

void WrongFormatError::buildDescriptionText()
{
    // not used
}

void WrongFormatError::buildLabelText()
{
    // not used
}

void WrongFormatError::parseInputData(const std::string&, int&, bool&, std::ofstream&)
{
    // not used
}

WrongUNumberError::WrongUNumberError()
{
    mErrorCode = 6;
    mRow = 0;
    mColumn = 0;
}

void WrongUNumberError::execute(std::ofstream &err)
{
    handleError(err);
}

void WrongUNumberError::buildDescriptionText()
{
    // not used
}

void WrongUNumberError::buildLabelText()
{
    // not used
}

void WrongUNumberError::parseInputData(const std::string&, int&, bool&, std::ofstream&)
{
    // not used
}

NoDevicePresentError::NoDevicePresentError()
{
    mErrorCode = 7;
    mRow = 0;
    mColumn = 0;
}

void NoDevicePresentError::execute(std::ofstream &err)
{
    handleError(err);
}

void NoDevicePresentError::buildDescriptionText()
{
    // not used
}

void NoDevicePresentError::buildLabelText()
{
    // not used
}

void NoDevicePresentError::parseInputData(const std::string&, int&, bool&, std::ofstream&)
{
    // not used
}

DeviceConnectedToItselfError::DeviceConnectedToItselfError()
{
    mErrorCode = 8;
    mRow = 0;
    mColumn = 0;
}

void DeviceConnectedToItselfError::execute(std::ofstream &err)
{
    handleError(err);
}

void DeviceConnectedToItselfError::buildDescriptionText()
{
    // not used
}

void DeviceConnectedToItselfError::buildLabelText()
{
    // not used
}

void DeviceConnectedToItselfError::parseInputData(const std::string&, int&, bool&, std::ofstream&)
{
    // not used
}

NoConnectionsError::NoConnectionsError()
{
    mErrorCode = 9;
    mRow = 0;
    mColumn = 0;
}

void NoConnectionsError::execute(std::ofstream &err)
{
    handleError(err);
}

void NoConnectionsError::buildDescriptionText()
{
    // not used
}

void NoConnectionsError::buildLabelText()
{
    // not used
}

void NoConnectionsError::parseInputData(const std::string&, int&, bool&, std::ofstream&)
{
    // not used
}
