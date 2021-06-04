#include <map>

#include "applicationdata.h"
#include "applicationutils.h"
#include "deviceportutils.h"
#include "powerdeviceport.h"

PDUPort::PDUPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredNrOfInputParams.at(Data::DeviceTypeID::PDU),
                 Data::c_MaxAllowedInputCharsCount.at(Data::DeviceTypeID::PDU),
                 isSourceDevice}
{
    _registerRequiredParameter(&mDevicePlacementType);
    _registerRequiredParameter(&mLoadSegmentNumber);
    _registerRequiredParameter(&mPortNumber);
}

void PDUPort::computeDescriptionAndLabel()
{
    const std::map<std::string, std::string> c_PlacementTypesDescriptions
    {
        {"H", "Horizontal"},    // horizontal PDU
        {"L", "Vertical left"}, // vertically left placed PDU
        {"R", "Vertical right"} // vertically right placed PDU
    };

    Utilities::convertStringCase(mDevicePlacementType, true);
    Utilities::convertStringCase(mPortNumber, true);

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
        else if (Utilities::isDigitString(mPortNumber))
        {
            if ("-" == mLoadSegmentNumber)
            {
                mDescription += " - port number" + mPortNumber;
                mLabel += "_P" + mPortNumber;
            }
            else if (Utilities::isDigitString(mLoadSegmentNumber))
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

ExtensionBarPort::ExtensionBarPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredNrOfInputParams.at(Data::DeviceTypeID::EXTENSION_BAR),
                 Data::c_MaxAllowedInputCharsCount.at(Data::DeviceTypeID::EXTENSION_BAR),
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
        const std::string c_PlacementSide{"L" == mDevicePlacementType ? "Left" : "Right"};

        mDescription = c_PlacementSide + " extension bar placed at U" + mDeviceUPosition;
        mLabel = "U" + mDeviceUPosition + "_" + mDevicePlacementType + "EXT";

        if ("IN" == mPortNumber)
        {
            mDescription += " - port number IN";
            mLabel += "_IN";
        }
        else if (Utilities::isDigitString(mPortNumber))
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

UPSPort::UPSPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredNrOfInputParams.at(Data::DeviceTypeID::UPS),
                 Data::c_MaxAllowedInputCharsCount.at(Data::DeviceTypeID::UPS),
                 isSourceDevice}
{
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
    else if (Utilities::isDigitString(mPortNumber))
    {
        if (Utilities::isDigitString(mLoadSegmentNumber))
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
