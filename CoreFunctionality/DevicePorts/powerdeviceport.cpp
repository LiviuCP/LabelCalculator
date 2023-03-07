#include <cassert>

#include "deviceportdata.h"
#include "deviceportutils.h"
#include "powerdeviceport.h"

namespace Core = Utilities::Core;
namespace Ports = Utilities::DevicePorts;

PDUPort::PDUPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

void PDUPort::updateDescriptionAndLabel()
{
    Core::convertStringCase(mDevicePlacementType, true);

    if (auto devicePlacementTypeIt{Data::c_DevicePlacementIdentifiers.find(mDevicePlacementType)}; Data::c_DevicePlacementIdentifiers.cend() != devicePlacementTypeIt)
    {
        const std::string c_DevicePlacementTypeDescription{"H" == devicePlacementTypeIt->first ? " in horizontal position" : " on the " + devicePlacementTypeIt->second + " side"};

        std::string portNumber{_getPortNumber()};
        Core::convertStringCase(portNumber, true);

        if (Core::isDigitString(mLoadSegmentNumber))
        {
            if (Core::isDigitString(portNumber))
            {
                _appendDataToDescription(c_DevicePlacementTypeDescription + " - load segment number " + mLoadSegmentNumber + " - port number " + portNumber);
                _appendDataToLabel("_" + mDevicePlacementType + "_P" + mLoadSegmentNumber + "." + portNumber);
            }
            else
            {
                _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
            }
        }
        else if ("-" == mLoadSegmentNumber)
        {
            if (Core::isDigitString(portNumber))
            {
                _appendDataToDescription(c_DevicePlacementTypeDescription + " - port number " + portNumber);
                _appendDataToLabel("_" + mDevicePlacementType + "_P" + portNumber);
            }
            else if (Ports::isManagementPortNumber(_getPortNumber())) // management port
            {
                _appendDataToDescription(c_DevicePlacementTypeDescription + " - management port");
                _appendDataToLabel("_" + mDevicePlacementType + "_MGMT");
            }
            else if ("IN" == portNumber)
            {
                _appendDataToDescription(c_DevicePlacementTypeDescription + " - port number IN");
                _appendDataToLabel("_" + mDevicePlacementType + "_IN");
            }
            else
            {
                _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
            }
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidLoadSegmentNumberErrorText);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPlacementErrorText);
    }

    _checkLabel();
}

void PDUPort::_registerRequiredParameters()
{
    _registerRequiredParameter(&mDevicePlacementType);
    _registerRequiredParameter(&mLoadSegmentNumber);
    _registerPortNumber();
}

size_t PDUPort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::PDU);
}

std::pair<std::string, std::string> PDUPort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::PDU);
}

ExtensionBarPort::ExtensionBarPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

void ExtensionBarPort::updateDescriptionAndLabel()
{
    Core::convertStringCase(mDevicePlacementType, true);

    if ("L" == mDevicePlacementType || "R" == mDevicePlacementType)
    {
        if (auto devicePlacementTypeIt{Data::c_DevicePlacementIdentifiers.find(mDevicePlacementType)}; Data::c_DevicePlacementIdentifiers.cend() != devicePlacementTypeIt)
        {
            const std::string c_DevicePlacementTypeDescription{" on the " + devicePlacementTypeIt->second + " side"};

            std::string portNumber{_getPortNumber()};
            Core::convertStringCase(portNumber, true);

            if (Core::isDigitString(portNumber))
            {
                _appendDataToDescription(c_DevicePlacementTypeDescription + " - port number " + portNumber);
                _appendDataToLabel("_" + mDevicePlacementType + "_P" + portNumber);
            }
            else if ("IN" == portNumber)
            {
                _appendDataToDescription(c_DevicePlacementTypeDescription + " - port number IN");
                _appendDataToLabel("_" + mDevicePlacementType + "_IN");
            }
            else
            {
                _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
            }
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPlacementErrorText);
    }

    _checkLabel();
}

void ExtensionBarPort::_registerRequiredParameters()
{
    _registerRequiredParameter(&mDevicePlacementType);
    _registerPortNumber();
}

size_t ExtensionBarPort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::EXTENSION_BAR);
}

std::pair<std::string, std::string> ExtensionBarPort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::EXTENSION_BAR);
}

UPSPort::UPSPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

void UPSPort::updateDescriptionAndLabel()
{
    if (Core::isDigitString(mLoadSegmentNumber))
    {
        if (const std::string c_PortNumber{_getPortNumber()}; Core::isDigitString(c_PortNumber)) // power port
        {
            _appendDataToDescription(" - load segment " + mLoadSegmentNumber + " - port " + c_PortNumber);
            _appendDataToLabel("_P" + mLoadSegmentNumber + "." + c_PortNumber);
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
        }
    }
    else if ("-" == mLoadSegmentNumber)
    {
        if (Ports::isManagementPortNumber(_getPortNumber())) // management port
        {
            _appendDataToDescription(" - management port");
            _appendDataToLabel("_MGMT");
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidLoadSegmentNumberErrorText);
    }

    _checkLabel();
}

void UPSPort::_registerRequiredParameters()
{
    _registerRequiredParameter(&mLoadSegmentNumber);
    _registerPortNumber();
}

size_t UPSPort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::UPS);
}

std::pair<std::string, std::string> UPSPort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::UPS);
}
