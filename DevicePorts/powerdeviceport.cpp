#include <cassert>

#include "applicationdata.h"
#include "applicationutils.h"
#include "deviceportdata.h"
#include "deviceportutils.h"
#include "powerdeviceport.h"

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
    Utilities::convertStringCase(mDevicePlacementType, true);
    Utilities::convertStringCase(mPortNumber, true);

    if (Data::c_DevicePlacementIdentifiers.find(mDevicePlacementType) != Data::c_DevicePlacementIdentifiers.cend())
    {
        if (Utilities::isDigitString(mLoadSegmentNumber))
        {
            if (Utilities::isDigitString(mPortNumber))
            {
                mDescription = Data::c_DevicePlacementIdentifiers.at(mDevicePlacementType) + " PDU placed at U" + mDeviceUPosition +
                               " - load segment number " + mLoadSegmentNumber + " - port number " + mPortNumber;
                mLabel = "U" + mDeviceUPosition + "_" + mDevicePlacementType + "PDU_P" + mLoadSegmentNumber + "." + mPortNumber;
            }
            else
            {
                mDescription = Utilities::c_InvalidPortNumberErrorText;
                mLabel = Utilities::c_LabelErrorText;
            }
        }
        else if ("-" == mLoadSegmentNumber)
        {
            if (Utilities::isDigitString(mPortNumber))
            {
                mDescription = Data::c_DevicePlacementIdentifiers.at(mDevicePlacementType) + " PDU placed at U" + mDeviceUPosition + " - port number r" + mPortNumber;
                mLabel = "U" + mDeviceUPosition + "_" + mDevicePlacementType + "PDU_P" + mPortNumber;
            }
            else if ("M" == mPortNumber) // management port
            {
                mDescription = Data::c_DevicePlacementIdentifiers.at(mDevicePlacementType) + " PDU placed at U" + mDeviceUPosition + " - management port";
                mLabel = "U" + mDeviceUPosition + "_" + mDevicePlacementType + "PDU_MGMT";
            }
            else if ("IN" == mPortNumber)
            {
                mDescription = Data::c_DevicePlacementIdentifiers.at(mDevicePlacementType) + " PDU placed at U" + mDeviceUPosition + " - port number " + mPortNumber;
                mLabel = "U" + mDeviceUPosition + "_" + mDevicePlacementType + "PDU_" + mPortNumber;
            }
            else
            {
                mDescription = Utilities::c_InvalidPortNumberErrorText;
                mLabel = Utilities::c_LabelErrorText;
            }
        }
        else
        {
            mDescription = Utilities::c_InvalidLoadSegmentNumberErrorText;
            mLabel = Utilities::c_LabelErrorText;
        }
    }
    else
    {
        mDescription = Utilities::c_InvalidPlacementErrorText;
        mLabel = Utilities::c_LabelErrorText;
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
    Utilities::convertStringCase(mDevicePlacementType, true);
    Utilities::convertStringCase(mPortNumber, true);

    if ("L" == mDevicePlacementType || "R" == mDevicePlacementType)
    {
        assert(Data::c_DevicePlacementIdentifiers.find(mDevicePlacementType) != Data::c_DevicePlacementIdentifiers.cend());

        if (Utilities::isDigitString(mPortNumber))
        {
            mDescription = Data::c_DevicePlacementIdentifiers.at(mDevicePlacementType) + " extension bar placed at U" + mDeviceUPosition + " - port number " + mPortNumber;
            mLabel = "U" + mDeviceUPosition + "_" + mDevicePlacementType + "EXT_P" + mPortNumber;
        }
        else if ("IN" == mPortNumber)
        {
            mDescription = Data::c_DevicePlacementIdentifiers.at(mDevicePlacementType) + " extension bar placed at U" + mDeviceUPosition + " - port number IN";
            mLabel = "U" + mDeviceUPosition + "_" + mDevicePlacementType + "EXT_IN";
        }
        else
        {
            mDescription = Utilities::c_InvalidPortNumberErrorText;
            mLabel = Utilities::c_LabelErrorText;
        }
    }
    else
    {
        mDescription = Utilities::c_InvalidPlacementErrorText;
        mLabel = Utilities::c_LabelErrorText;
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
}

void UPSPort::computeDescriptionAndLabel()
{
    if (Utilities::isDigitString(mLoadSegmentNumber))
    {
        if (Utilities::isDigitString(mPortNumber)) // power port
        {
            mDescription = "UPS placed at U" + mDeviceUPosition + " - load segment " + mLoadSegmentNumber + " - port " + mPortNumber;
            mLabel += "U" + mDeviceUPosition + "_P" + mLoadSegmentNumber + "." + mPortNumber;
        }
        else
        {
            mDescription = Utilities::c_InvalidPortNumberErrorText;
            mLabel = Utilities::c_LabelErrorText;
        }
    }
    else if ("-" == mLoadSegmentNumber)
    {
        if ("m" == mPortNumber || "M" == mPortNumber) // management port
        {
            mDescription = "UPS placed at U" + mDeviceUPosition + " - management port";
            mLabel = "U" + mDeviceUPosition + "_MGMT";
        }
        else
        {
            mDescription = Utilities::c_InvalidPortNumberErrorText;
            mLabel = Utilities::c_LabelErrorText;
        }
    }
    else
    {
        mDescription = Utilities::c_InvalidLoadSegmentNumberErrorText;
        mLabel = Utilities::c_LabelErrorText;
    }

    _checkLabel();
}
