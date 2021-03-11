#include "error.h"

UnknownDeviceError::UnknownDeviceError()
    : Device{static_cast<int>(ErrorTypes::UNKNOWN_DEVICE)}
{
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
    : Device{static_cast<int>(ErrorTypes::FEWER_CELLS)}
{
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
    : Device{static_cast<int>(ErrorTypes::WRONG_CONNECTION_FORMAT)}
{
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
    : Device{static_cast<int>(ErrorTypes::PLACEMENT_OUT_OF_RANGE)}
{
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
    : Device{static_cast<int>(ErrorTypes::NO_DEVICE_PLACED_IN_POSITION)}
{
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
    : Device{static_cast<int>(ErrorTypes::DEVICE_CONNECTED_TO_ITSELF)}
{
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
    : Device{static_cast<int>(ErrorTypes::NULL_NR_OF_CONNECTIONS)}
{
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
