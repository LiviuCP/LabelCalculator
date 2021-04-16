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
        const string c_ChosenPlacementDescription{c_PlacementTypesDescriptions.at(mDevicePlacementType)};

        if ("-" == mLoadSegmentNumber) // pdu with a single load segment (user fills in "-")
        {
            const std::string c_PortNumberSubstring{"IN" == portNumberUpperCase ? "_IN" : "_P" + mPortNumber};

            mDescription = c_ChosenPlacementDescription + " PDU placed at U" + mDeviceName + " - port number " + mPortNumber;
            mLabel = "U" + mDeviceName + "_" + mDevicePlacementType + "PDU" + c_PortNumberSubstring;
        }
        else // pdu with multiple load segments
        {
            const std::string c_LoadSegmentPortNumberSubstring{"IN" == portNumberUpperCase ? "_IN" : "_P" + mLoadSegmentNumber + "." + mPortNumber};

            mDescription = c_ChosenPlacementDescription + " PDU placed at U" + mDeviceName + " - load segment number " + mLoadSegmentNumber + " - port number " + mPortNumber;
            mLabel = "U" + mDeviceName + "_" + mDevicePlacementType + "PDU" + c_LoadSegmentPortNumberSubstring;
        }
    }
    else
    {
        mDescription = "ERROR. THE PDU PLACEMENT TYPE YOU HAVE CHOSEN DOES NOT EXIST. PLEASE REVIEW INPUT FILE (connectioninput.csv).";
        mLabel = "ERROR. INVALID PLACEMENT TYPE.";
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
        mDescription = "ERROR. THE EXTENSION BAR PLACEMENT TYPE YOU HAVE CHOSEN DOES NOT EXIST. PLEASE REVIEW INPUT FILE(connectioninput.csv).";
        mLabel = "ERROR. INVALID PLACEMENT TYPE.";
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
    mDescription = "UPS placed at U" + mDeviceName + " - load segment " + mLoadSegmentNumber + " - port " + mPortNumber;
    mLabel = "U" + mDeviceName + "_P" + mLoadSegmentNumber + "." + mPortNumber;
}

PowerSupply::PowerSupply(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::POWER_SUPPLY), c_MaxAllowedCharsCount.at(DeviceTypeID::POWER_SUPPLY), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mPortNumber);
}

void PowerSupply::computeDescriptionAndLabel()
{
    mDescription = "Device placed at U" + mDeviceName + " - power supply " + mPortNumber;
    mLabel = "U" + mDeviceName + "_PS" + mPortNumber;
}
