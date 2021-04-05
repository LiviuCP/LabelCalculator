#include <map>

#include "powerdevice.h"

PDU::PDU(bool isSourceDevice)
    : Device{"_pdu", c_MaxAllowedNrOfChars.at("_pdu"), isSourceDevice}
{
    mInputData.resize(Device::scRequiredNrOfInputDataFields);

    mInputData[0] = &mDeviceName;
    mInputData[1] = &mPlacementType;
    mInputData[2] = &mLoadSegmentNumber;
    mInputData[3] = &mPortNumber;
}

void PDU::buildDescriptionText()
{
    using namespace std;

    const map<string, string> c_PlacementTypesDescriptions
    {
        {"H", "Horizontal"},    // horizontal PDU
        {"L", "Vertical left"}, // vertically left placed PDU
        {"R", "Vertical right"} // vertically right placed PDU
    };

    convertStringCase(mPlacementType, mPlacementType, true);

    if (c_PlacementTypesDescriptions.find(mPlacementType) != c_PlacementTypesDescriptions.cend())
    {
        const string c_ChosenPlacementDescription{c_PlacementTypesDescriptions.at(mPlacementType)};

        if (mLoadSegmentNumber=="-") // pdu with a single load segment (user fills in "-")
        {
            mDescription = c_ChosenPlacementDescription + " PDU placed at U" + mDeviceName + " - port number " + mPortNumber;
        }
        else // pdu with multiple load segments
        {
            mDescription = c_ChosenPlacementDescription + " PDU placed at U" + mDeviceName + " - load segment number " + mLoadSegmentNumber + " - port number " + mPortNumber;
        }
    }
    else
    {
        mDescription = "ERROR. THE PDU PLACEMENT TYPE YOU HAVE CHOSEN DOES NOT EXIST. PLEASE REVIEW INPUT FILE (connectioninput.csv).";
    }
}

void PDU::buildLabelText()
{
    if ("L" == mPlacementType || "R" == mPlacementType || "H" == mPlacementType)
    {
        std::string portNumberUpperCase;
        convertStringCase(mPortNumber, portNumberUpperCase, true);

        if (mLoadSegmentNumber=="-")  // single load segment PDU
        {
            const std::string c_PortNumberSubstring{"IN" == portNumberUpperCase ? "_IN" : "_P" + mPortNumber};
            mLabel = "U" + mDeviceName + "_" + mPlacementType + "PDU" + c_PortNumberSubstring;
        }
        else // multiple load segments PDU
        {
            const std::string c_LoadSegmentPortNumberSubstring{"IN" == portNumberUpperCase ? "_IN" : "_P" + mLoadSegmentNumber + "." + mPortNumber};
            mLabel = "U" + mDeviceName + "_" + mPlacementType + "PDU" + c_LoadSegmentPortNumberSubstring;
        }
    }
    else
    {
        mLabel = "ERROR. INVALID PLACEMENT TYPE.";
    }
}

ExtensionBar::ExtensionBar(bool isSourceDevice)
    : Device{"_ext", c_MaxAllowedNrOfChars.at("_ext"), isSourceDevice}
{
    mInputData.resize(Device::scRequiredNrOfInputDataFields);

    mInputData[0] = &mDeviceName;
    mInputData[1] = &mPlacementType;
    mInputData[2] = &mPortNumber;
    mInputData[3] = &mPlaceholder;
}

void ExtensionBar::buildDescriptionText()
{
    convertStringCase(mPlacementType, mPlacementType, true);

    if ("L" == mPlacementType)
    {
        mDescription = "Left extension bar placed at U" + mDeviceName + " - port number " + mPortNumber;
    }
    else if ("R" == mPlacementType)
    {
        mDescription = "Right extension bar placed at U" + mDeviceName + " - port number " + mPortNumber;
    }
    else
    {
        mDescription = "ERROR. THE EXTENSION BAR PLACEMENT TYPE YOU HAVE CHOSEN DOES NOT EXIST. PLEASE REVIEW INPUT FILE(connectioninput.csv).";
    }
}

void ExtensionBar::buildLabelText()
{
    std::string portNumberUpperCase;
    convertStringCase(mPortNumber, portNumberUpperCase, true);

    if ("L" == mPlacementType || "R" == mPlacementType)
    {
        const std::string c_PortNumberSubstring{"IN" == portNumberUpperCase ? "_IN" : "_P" + mPortNumber};
        mLabel = "U" + mDeviceName + "_" + mPlacementType + "EXT" + c_PortNumberSubstring;
    }
    else
    {
        mLabel = "ERROR. INVALID PLACEMENT TYPE.";
    }
}

UPS::UPS(bool isSourceDevice)
    : Device{"_ups", c_MaxAllowedNrOfChars.at("_ups"), isSourceDevice}
{
    mInputData.resize(Device::scRequiredNrOfInputDataFields);

    mInputData[0] = &mDeviceName;
    mInputData[1] = &mLoadSegmentNumber;
    mInputData[2] = &mPortNumber;
    mInputData[3] = &mPlaceholder;
}

void UPS::buildDescriptionText()
{
    mDescription = "UPS placed at U" + mDeviceName + " - load segment " + mLoadSegmentNumber + " - port " + mPortNumber;
}

void UPS::buildLabelText()
{
    mLabel = "U" + mDeviceName + "_P" + mLoadSegmentNumber + "." + mPortNumber;
}

PowerSupply::PowerSupply(bool isSourceDevice)
    : Device{"_ps", c_MaxAllowedNrOfChars.at("_ps"), isSourceDevice}
{
    mInputData.resize(Device::scRequiredNrOfInputDataFields);

    mInputData[0] = &mDeviceName;
    mInputData[1] = &mPortNumber; // port number is power supply number (a power supply normally has one port)
    mInputData[2] = &mPlaceholder1;
    mInputData[3] = &mPlaceholder2;
}

void PowerSupply::buildDescriptionText()
{
    mDescription = "Device placed at U" + mDeviceName + " - power supply " + mPortNumber;
}

void PowerSupply::buildLabelText()
{
    mLabel = "U" + mDeviceName + "_PS" + mPortNumber;
}
