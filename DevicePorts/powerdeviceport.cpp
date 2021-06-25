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
                mDescription = Utilities::c_InvalidLoadSegmentNumberErrorText;
                mLabel = Utilities::c_LabelErrorText;
            }
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

    if (Utilities::c_LabelErrorText != mLabel)
    {
        _checkLabelSize();
    }
}

ExtensionBarPort::ExtensionBarPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredNrOfInputParams.at(Data::DeviceTypeID::EXTENSION_BAR),
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
            mDescription = Utilities::c_InvalidPortNumberErrorText;
            mLabel = Utilities::c_LabelErrorText;
        }
    }
    else
    {
        mDescription = Utilities::c_InvalidPlacementErrorText;
        mLabel = Utilities::c_LabelErrorText;
    }

    if (Utilities::c_LabelErrorText != mLabel)
    {
        _checkLabelSize();
    }
}

UPSPort::UPSPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredNrOfInputParams.at(Data::DeviceTypeID::UPS),
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
            mDescription = Utilities::c_InvalidLoadSegmentNumberErrorText;
            mLabel = Utilities::c_LabelErrorText;
        }
    }
    else // power port
    {
        mDescription = Utilities::c_InvalidPortNumberErrorText;
        mLabel = Utilities::c_LabelErrorText;
    }

    if (Utilities::c_LabelErrorText != mLabel)
    {
        _checkLabelSize();
    }
}
