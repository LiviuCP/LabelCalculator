#include <map>

#include "powerdevice.h"

PDU::PDU(bool isSourceDevice)
{
    mDeviceType = "_pdu";
    mErrorCode = 0;
    mRow = 1;
    mColumn = 1;
    mRequiredNrOfInputDataFields = 4; // number of fields that should be filled in connectioninput.csv for this device type
    mMaxAllowedNrOfChars = 6;  // maximum number of characters that can be filled in the connectioninput.csv file for this device type
    mDeltaNrOfChars = 0;

    // device parameters
    mInputData.resize(mRequiredNrOfInputDataFields);
    mInputData[0] = &mDeviceName;
    mInputData[1] = &mPlacementType;
    mInputData[2] = &mLoadSegmentNumber;
    mInputData[3] = &mPortNumber;

    mIsSourceDevice = isSourceDevice;
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

    for (auto iterator{mPlacementType.begin()}; iterator != mPlacementType.end(); ++iterator)
    {
        *iterator = toupper(*iterator);
    }

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
        if (mLoadSegmentNumber=="-")  // single load segment PDU
        {
            const std::string c_PortNumberSubstring{"IN" == mPortNumber ? "_IN" : "_P" + mPortNumber}; //TODO: convert IN to lowercase
            mLabel = "U" + mDeviceName + "_" + mPlacementType + "PDU" + c_PortNumberSubstring;
        }
        else // multiple load segments PDU
        {
            const std::string c_LoadSegmentPortNumberSubstring{"IN" == mPortNumber ? "_IN" : "_P" + mLoadSegmentNumber + "." + mPortNumber}; //TODO: convert IN to lowercase
            mLabel = "U" + mDeviceName + "_" + mPlacementType + "PDU" + c_LoadSegmentPortNumberSubstring;
        }
    }
    else
    {
        mLabel = "ERROR. INVALID PLACEMENT TYPE.";
    }
}

ExtensionBar::ExtensionBar(bool isSourceDevice)
{
    mDeviceType = "_ext";
    mErrorCode = 0;
    mRow = 1;
    mColumn = 1;
    mRequiredNrOfInputDataFields = 4; // number of fields that should be filled in connectioninput.csv for this device type
    mMaxAllowedNrOfChars = 8;  // maximum number of characters that can be filled in the connectioninput.csv file for this device type (including the placeholder where character '-' should be filled in)
    mDeltaNrOfChars = 0;

    // device parameters
    mInputData.resize(mRequiredNrOfInputDataFields);
    mInputData[0] = &mDeviceName;
    mInputData[1] = &mPlacementType;
    mInputData[2] = &mPortNumber;
    mInputData[3] = &mPlaceholder;

    mIsSourceDevice = isSourceDevice;
}

void ExtensionBar::buildDescriptionText()
{
    for (auto iterator{mPlacementType.begin()}; iterator != mPlacementType.end(); ++iterator)
    {
        *iterator = toupper(*iterator);
    }

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
        mDescription="ERROR. THE EXTENSION BAR PLACEMENT TYPE YOU HAVE CHOSEN DOES NOT EXIST. PLEASE REVIEW INPUT FILE(connectioninput.csv).";
    }
}

void ExtensionBar::buildLabelText()
{
    if ("L" == mPlacementType || "R" == mPlacementType)
    {
        const std::string c_PortNumberSubstring{"IN" == mPortNumber ? "_IN" : "_P" + mPortNumber}; //TODO: convert IN to lowercase
        mLabel = "U" + mDeviceName + "_" + mPlacementType + "EXT" + c_PortNumberSubstring;
    }
    else
    {
        mLabel = "ERROR. INVALID PLACEMENT TYPE.";
    }
}

UPS::UPS(bool isSourceDevice)
{
    mDeviceType = "_ups";
    mErrorCode = 0;
    mRow = 1;
    mColumn = 1;
    mRequiredNrOfInputDataFields = 4; // number of fields that should be filled in connectioninput.csv for this device type
    mMaxAllowedNrOfChars = 11;  // maximum number of characters that can be filled in the connectioninput.csv file for this device type (including the placeholder where character '-' should be filled in)
    mDeltaNrOfChars = 0;

    // device parameters
    mInputData.resize(mRequiredNrOfInputDataFields);
    mInputData[0] = &mDeviceName;
    mInputData[1] = &mLoadSegmentNumber;
    mInputData[2] = &mPortNumber;
    mInputData[3] = &mPlaceholder;

    mIsSourceDevice = isSourceDevice;
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
{
    mDeviceType = "_ps";
    mErrorCode = 0;
    mRow = 1;
    mColumn = 1;
    mRequiredNrOfInputDataFields = 4; // number of fields that should be filled in connectioninput.csv for this device type
    mMaxAllowedNrOfChars = 12;  // maximum number of characters that can be filled in the connectioninput.csv file for this device type (including the 2 placeholders where character '-' should be filled in)
    mDeltaNrOfChars = 0;

    // device parameters
    mInputData.resize(mRequiredNrOfInputDataFields);
    mInputData[0] = &mDeviceName;
    mInputData[1] = &mPowerSupplyNumber;
    mInputData[2] = &mPlaceholder1;
    mInputData[3] = &mPlaceholder2;

    mIsSourceDevice = isSourceDevice;
}

void PowerSupply::buildDescriptionText()
{
    mDescription = "Device placed at U" + mDeviceName + " - power supply " + mPowerSupplyNumber;
}

void PowerSupply::buildLabelText()
{
    mLabel = "U" + mDeviceName + "_PS" + mPowerSupplyNumber;
}
