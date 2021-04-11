#include <map>

#include "powerdevice.h"

PDU::PDU(bool isSourceDevice)
    : Device{c_RequiredNrOfInputParams.at("_pdu"), c_MaxAllowedCharsCount.at("_pdu"), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mPlacementType);
    _registerRequiredParameter(&mLoadSegmentNumber);
    _registerRequiredParameter(&mPortNumber);
}

void PDU::computeDescriptionAndLabel()
{
    using namespace std;

    const map<string, string> c_PlacementTypesDescriptions
    {
        {"H", "Horizontal"},    // horizontal PDU
        {"L", "Vertical left"}, // vertically left placed PDU
        {"R", "Vertical right"} // vertically right placed PDU
    };

    convertStringCase(mPlacementType, mPlacementType, true);

    std::string portNumberUpperCase;
    convertStringCase(mPortNumber, portNumberUpperCase, true);

    if (c_PlacementTypesDescriptions.find(mPlacementType) != c_PlacementTypesDescriptions.cend())
    {
        const string c_ChosenPlacementDescription{c_PlacementTypesDescriptions.at(mPlacementType)};

        if ("-" == mLoadSegmentNumber) // pdu with a single load segment (user fills in "-")
        {
            const std::string c_PortNumberSubstring{"IN" == portNumberUpperCase ? "_IN" : "_P" + mPortNumber};

            mDescription = c_ChosenPlacementDescription + " PDU placed at U" + mDeviceName + " - port number " + mPortNumber;
            mLabel = "U" + mDeviceName + "_" + mPlacementType + "PDU" + c_PortNumberSubstring;
        }
        else // pdu with multiple load segments
        {
            const std::string c_LoadSegmentPortNumberSubstring{"IN" == portNumberUpperCase ? "_IN" : "_P" + mLoadSegmentNumber + "." + mPortNumber};

            mDescription = c_ChosenPlacementDescription + " PDU placed at U" + mDeviceName + " - load segment number " + mLoadSegmentNumber + " - port number " + mPortNumber;
            mLabel = "U" + mDeviceName + "_" + mPlacementType + "PDU" + c_LoadSegmentPortNumberSubstring;
        }
    }
    else
    {
        mDescription = "ERROR. THE PDU PLACEMENT TYPE YOU HAVE CHOSEN DOES NOT EXIST. PLEASE REVIEW INPUT FILE (connectioninput.csv).";
        mLabel = "ERROR. INVALID PLACEMENT TYPE.";
    }
}

ExtensionBar::ExtensionBar(bool isSourceDevice)
    : Device{c_RequiredNrOfInputParams.at("_ext"), c_MaxAllowedCharsCount.at("_ext"), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mPlacementType);
    _registerRequiredParameter(&mPortNumber);
}

void ExtensionBar::computeDescriptionAndLabel()
{
    convertStringCase(mPlacementType, mPlacementType, true);

    std::string portNumberUpperCase;
    convertStringCase(mPortNumber, portNumberUpperCase, true);

    if ("L" == mPlacementType || "R" == mPlacementType)
    {
        const std::string c_PlacementSide{"L" == mPlacementType ? "Left" : "Right"};
        const std::string c_PortNumberSubstring{"IN" == portNumberUpperCase ? "_IN" : "_P" + mPortNumber};

        mDescription = c_PlacementSide + " extension bar placed at U" + mDeviceName + " - port number " + mPortNumber;
        mLabel = "U" + mDeviceName + "_" + mPlacementType + "EXT" + c_PortNumberSubstring;
    }
    else
    {
        mDescription = "ERROR. THE EXTENSION BAR PLACEMENT TYPE YOU HAVE CHOSEN DOES NOT EXIST. PLEASE REVIEW INPUT FILE(connectioninput.csv).";
        mLabel = "ERROR. INVALID PLACEMENT TYPE.";
    }
}

UPS::UPS(bool isSourceDevice)
    : Device{c_RequiredNrOfInputParams.at("_ups"), c_MaxAllowedCharsCount.at("_ups"), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mLoadSegmentNumber);
    _registerRequiredParameter(&mPortNumber);
}

void UPS::computeDescriptionAndLabel()
{
    mDescription = "UPS placed at U" + mDeviceName + " - load segment " + mLoadSegmentNumber + " - port " + mPortNumber;
    mLabel = "U" + mDeviceName + "_P" + mLoadSegmentNumber + "." + mPortNumber;
}

PowerSupply::PowerSupply(bool isSourceDevice)
    : Device{c_RequiredNrOfInputParams.at("_ps"), c_MaxAllowedCharsCount.at("_ps"), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mPortNumber);
}

void PowerSupply::computeDescriptionAndLabel()
{
    mDescription = "Device placed at U" + mDeviceName + " - power supply " + mPortNumber;
    mLabel = "U" + mDeviceName + "_PS" + mPortNumber;
}
