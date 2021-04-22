#include <map>

#include "powerdeviceport.h"

PDUPort::PDUPort(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::PDU), c_MaxAllowedCharsCount.at(DeviceTypeID::PDU), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceUPosition);
    _registerRequiredParameter(&mDevicePlacementType);
    _registerRequiredParameter(&mLoadSegmentNumber);
    _registerRequiredParameter(&mPortNumber);
}

void PDUPort::computeDescriptionAndLabel()
{
    using namespace std;

    const map<string, string> c_PlacementTypesDescriptions
    {
        {"H", "Horizontal"},    // horizontal PDU
        {"L", "Vertical left"}, // vertically left placed PDU
        {"R", "Vertical right"} // vertically right placed PDU
    };

    convertStringCase(mDevicePlacementType, true);
    convertStringCase(mPortNumber, true);

    if (c_PlacementTypesDescriptions.find(mDevicePlacementType) != c_PlacementTypesDescriptions.cend())
    {
        mDescription = c_PlacementTypesDescriptions.at(mDevicePlacementType) + " PDU placed at U" + mDeviceUPosition;
        mLabel = "U" + mDeviceUPosition + "_" + mDevicePlacementType + "PDU";

        if ("M" == mPortNumber) // management port
        {
            mDescription += " - management port";
            mLabel += "_MGMT";
        }
        else if ("IN" == mPortNumber)
        {
            mDescription += " - port number " + mPortNumber;
            mLabel += "_" + mPortNumber;
        }
        else if (isDigitString(mPortNumber))
        {
            if ("-" == mLoadSegmentNumber)
            {
                mDescription += " - port number" + mPortNumber;
                mLabel += "_P" + mPortNumber;
            }
            else if (isDigitString(mLoadSegmentNumber))
            {
                mDescription += " - load segment number " + mLoadSegmentNumber + " - port number " + mPortNumber;
                mLabel += "_P" + mLoadSegmentNumber + "." + mPortNumber;
            }
            else
            {
                mDescription = c_InvalidLoadSegmentNumberErrorText;
                mLabel = c_LabelErrorText;
            }
        }
        else
        {
            mDescription = c_InvalidPortNumberErrorText;
            mLabel = c_LabelErrorText;
        }
    }
    else
    {
        mDescription = c_InvalidPlacementErrorText;
        mLabel = c_LabelErrorText;
    }
}

ExtensionBarPort::ExtensionBarPort(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::EXTENSION_BAR), c_MaxAllowedCharsCount.at(DeviceTypeID::EXTENSION_BAR), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceUPosition);
    _registerRequiredParameter(&mDevicePlacementType);
    _registerRequiredParameter(&mPortNumber);
}

void ExtensionBarPort::computeDescriptionAndLabel()
{
    convertStringCase(mDevicePlacementType, true);
    convertStringCase(mPortNumber, true);

    if ("L" == mDevicePlacementType || "R" == mDevicePlacementType)
    {
        const std::string c_PlacementSide{"L" == mDevicePlacementType ? "Left" : "Right"};

        mDescription = c_PlacementSide + " extension bar placed at U" + mDeviceUPosition;
        mLabel = "U" + mDeviceUPosition + "_" + mDevicePlacementType + "EXT";

        if ("IN" == mPortNumber)
        {
            mDescription += " - port number IN";
            mLabel += "_IN";
        }
        else if (isDigitString(mPortNumber))
        {
            mDescription += " - port number " + mPortNumber;
            mLabel += "_P" + mPortNumber;
        }
        else
        {
            mDescription = c_InvalidPortNumberErrorText;
            mLabel = c_LabelErrorText;
        }
    }
    else
    {
        mDescription = c_InvalidPlacementErrorText;
        mLabel = c_LabelErrorText;
    }
}

UPSPort::UPSPort(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::UPS), c_MaxAllowedCharsCount.at(DeviceTypeID::UPS), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceUPosition);
    _registerRequiredParameter(&mLoadSegmentNumber);
    _registerRequiredParameter(&mPortNumber);
}

void UPSPort::computeDescriptionAndLabel()
{
    mDescription = "UPS placed at U" + mDeviceUPosition;
    mLabel = mLabel = "U" + mDeviceUPosition;

    if ("m" == mPortNumber || "M" == mPortNumber) // management port
    {
        mDescription += " - management port";
        mLabel += "_MGMT";
    }
    else if (isDigitString(mPortNumber))
    {
        if (isDigitString(mLoadSegmentNumber))
        {
            mDescription += " - load segment " + mLoadSegmentNumber + " - port " + mPortNumber;
            mLabel += "_P" + mLoadSegmentNumber + "." + mPortNumber;
        }
        else
        {
            mDescription = c_InvalidLoadSegmentNumberErrorText;
            mLabel = c_LabelErrorText;
        }
    }
    else // power port
    {
        mDescription = c_InvalidPortNumberErrorText;
        mLabel = c_LabelErrorText;
    }
}
