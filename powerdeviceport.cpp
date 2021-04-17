#include <map>

#include "powerdeviceport.h"

PDUPort::PDUPort(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::PDU), c_MaxAllowedCharsCount.at(DeviceTypeID::PDU), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
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

    convertStringCase(mDevicePlacementType, mDevicePlacementType, true);

    std::string portNumberUpperCase;
    convertStringCase(mPortNumber, portNumberUpperCase, true);

    if (c_PlacementTypesDescriptions.find(mDevicePlacementType) != c_PlacementTypesDescriptions.cend())
    {
        mDescription = c_PlacementTypesDescriptions.at(mDevicePlacementType) + " PDU placed at U" + mDeviceName;
        mLabel = "U" + mDeviceName + "_" + mDevicePlacementType + "PDU";

        if ("M" == portNumberUpperCase) // management port
        {
            mDescription += " - management port";
            mLabel += "_MGMT";
        }
        else if ("-" == mLoadSegmentNumber) // pdu with a single load segment (user fills in "-")
        {
            const std::string c_PortNumberSubstring{"IN" == portNumberUpperCase ? "_IN" : "_P" + mPortNumber};

            mDescription += " - port number " + mPortNumber;
            mLabel += c_PortNumberSubstring;
        }
        else // pdu with multiple load segments
        {
            const std::string c_LoadSegmentPortNumberSubstring{"IN" == portNumberUpperCase ? "_IN" : "_P" + mLoadSegmentNumber + "." + mPortNumber};

            mDescription += " - load segment number " + mLoadSegmentNumber + " - port number " + mPortNumber;
            mLabel += c_LoadSegmentPortNumberSubstring;
        }
    }
    else
    {
        mDescription = c_InvalidPDUPlacementErrorText;
        mLabel += c_LabelErrorText;
    }
}

ExtensionBarPort::ExtensionBarPort(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::EXTENSION_BAR), c_MaxAllowedCharsCount.at(DeviceTypeID::EXTENSION_BAR), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mDevicePlacementType);
    _registerRequiredParameter(&mPortNumber);
}

void ExtensionBarPort::computeDescriptionAndLabel()
{
    convertStringCase(mDevicePlacementType, mDevicePlacementType, true);

    std::string portNumberUpperCase;
    convertStringCase(mPortNumber, portNumberUpperCase, true);

    if ("L" == mDevicePlacementType || "R" == mDevicePlacementType)
    {
        const std::string c_PlacementSide{"L" == mDevicePlacementType ? "Left" : "Right"};
        const std::string c_PortNumberSubstring{"IN" == portNumberUpperCase ? "_IN" : "_P" + mPortNumber};

        mDescription = c_PlacementSide + " extension bar placed at U" + mDeviceName + " - port number " + mPortNumber;
        mLabel = "U" + mDeviceName + "_" + mDevicePlacementType + "EXT" + c_PortNumberSubstring;
    }
    else
    {
        mDescription = c_InvalidExtPlacementErrorText;
        mLabel += c_LabelErrorText;
    }
}

UPSPort::UPSPort(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::UPS), c_MaxAllowedCharsCount.at(DeviceTypeID::UPS), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mLoadSegmentNumber);
    _registerRequiredParameter(&mPortNumber);
}

void UPSPort::computeDescriptionAndLabel()
{
    mDescription = "UPS placed at U" + mDeviceName;
    mLabel = mLabel = "U" + mDeviceName;

    if ("m" == mPortNumber || "M" == mPortNumber) // management port
    {
        mDescription += " - management port";
        mLabel += "_MGMT";
    }
    else // power port
    {
        mDescription += " - load segment " + mLoadSegmentNumber + " - port " + mPortNumber;
        mLabel += "_P" + mLoadSegmentNumber + "." + mPortNumber;
    }
}
