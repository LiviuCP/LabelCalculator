#include <cassert>

#include "applicationdata.h"
#include "coreutils.h"
#include "deviceportdata.h"
#include "deviceportutils.h"
#include "powerdeviceport.h"

namespace Core = Utilities::Core;
namespace Ports = Utilities::DevicePorts;

PDUPort::PDUPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::PDU),
                 isSourceDevice}
{
    _registerRequiredParameter(&mDevicePlacementType);
    _registerRequiredParameter(&mLoadSegmentNumber);
    _registerRequiredParameter(&mPortNumber);
}

void PDUPort::computeDescriptionAndLabel()
{
    Core::convertStringCase(mDevicePlacementType, true);
    Core::convertStringCase(mPortNumber, true);

    if (Data::c_DevicePlacementIdentifiers.find(mDevicePlacementType) != Data::c_DevicePlacementIdentifiers.cend())
    {
        _initializeDescriptionAndLabel(Data::c_DevicePlacementIdentifiers.at(mDevicePlacementType) + " PDU");

        if (Core::isDigitString(mLoadSegmentNumber))
        {
            if (Core::isDigitString(mPortNumber))
            {
                _appendDataToDescription(" - load segment number " + mLoadSegmentNumber + " - port number " + mPortNumber);
                _appendDataToLabel("_" + mDevicePlacementType + "PDU_P" + mLoadSegmentNumber + "." + mPortNumber);
            }
            else
            {
                _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
            }
        }
        else if ("-" == mLoadSegmentNumber)
        {
            if (Core::isDigitString(mPortNumber))
            {
                _appendDataToDescription(" - port number " + mPortNumber);
                _appendDataToLabel("_" + mDevicePlacementType + "PDU_P" + mPortNumber);
            }
            else if ("M" == mPortNumber) // management port
            {
                _appendDataToDescription(" - management port");
                _appendDataToLabel("_" + mDevicePlacementType + "PDU_MGMT");
            }
            else if ("IN" == mPortNumber)
            {
                _appendDataToDescription(" - port number " + mPortNumber);
                _appendDataToLabel("_" + mDevicePlacementType + "PDU_" + mPortNumber);
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

ExtensionBarPort::ExtensionBarPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::EXTENSION_BAR),
                 isSourceDevice}
{
    _registerRequiredParameter(&mDevicePlacementType);
    _registerRequiredParameter(&mPortNumber);
}

void ExtensionBarPort::computeDescriptionAndLabel()
{
    Core::convertStringCase(mDevicePlacementType, true);
    Core::convertStringCase(mPortNumber, true);

    if ("L" == mDevicePlacementType || "R" == mDevicePlacementType)
    {
        if (Data::c_DevicePlacementIdentifiers.find(mDevicePlacementType) != Data::c_DevicePlacementIdentifiers.cend())
        {
            _initializeDescriptionAndLabel(Data::c_DevicePlacementIdentifiers.at(mDevicePlacementType) + " extension bar");

            if (Core::isDigitString(mPortNumber))
            {
                _appendDataToDescription(" - port number " + mPortNumber);
                _appendDataToLabel("_" + mDevicePlacementType + "EXT_P" + mPortNumber);
            }
            else if ("IN" == mPortNumber)
            {
                _appendDataToDescription(" - port number IN");
                _appendDataToLabel("_" + mDevicePlacementType + "EXT_IN");
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

UPSPort::UPSPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::UPS),
                 isSourceDevice}
{
    _registerRequiredParameter(&mLoadSegmentNumber);
    _registerRequiredParameter(&mPortNumber);

    _initializeDescriptionAndLabel("UPS");
}

void UPSPort::computeDescriptionAndLabel()
{
    if (Core::isDigitString(mLoadSegmentNumber))
    {
        if (Core::isDigitString(mPortNumber)) // power port
        {
            _appendDataToDescription(" - load segment " + mLoadSegmentNumber + " - port " + mPortNumber);
            _appendDataToLabel("_P" + mLoadSegmentNumber + "." + mPortNumber);
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
        }
    }
    else if ("-" == mLoadSegmentNumber)
    {
        if ("m" == mPortNumber || "M" == mPortNumber) // management port
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
