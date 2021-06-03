#include "applicationdata.h"
#include "applicationutils.h"
#include "deviceportutils.h"
#include "datadeviceport.h"

LANSwitchPort::LANSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition, fileRowNumber, fileColumnNumber, c_RequiredNrOfInputParams.at(DeviceTypeID::LAN_SWITCH), c_MaxAllowedInputCharsCount.at(DeviceTypeID::LAN_SWITCH), isSourceDevice}
{
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void LANSwitchPort::computeDescriptionAndLabel()
{
    mDescription = "LAN switch placed at U" + mDeviceUPosition;
    mLabel = "U" + mDeviceUPosition;

    convertStringCase(mPortType, true);

    if (isDigitString(mPortNumber)) // no management port
    {
        if ("N" == mPortType)
        {
            mDescription += " - Ethernet port " + mPortNumber;
            mLabel += "_P" + mPortNumber;
        }
        else if ("P" == mPortType)
        {
            mDescription += " - power supply " + mPortNumber;
            mLabel += "_PS" + mPortNumber;
        }
        else
        {
            mDescription = c_UnknownPortTypeErrorText;
            mLabel = c_LabelErrorText;
        }
    }
    else
    {
        mDescription = c_InvalidPortNumberErrorText;
        mLabel = c_LabelErrorText;
    }
}

SANSwitchPort::SANSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition, fileRowNumber, fileColumnNumber, c_RequiredNrOfInputParams.at(DeviceTypeID::SAN_SWITCH), c_MaxAllowedInputCharsCount.at(DeviceTypeID::SAN_SWITCH), isSourceDevice}
{
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void SANSwitchPort::computeDescriptionAndLabel()
{
    mDescription = "SAN switch placed at U" + mDeviceUPosition;
    mLabel = "U" + mDeviceUPosition;

    convertStringCase(mPortType, true);

    if ("m" == mPortNumber || "M" == mPortNumber) // management port
    {
        mDescription += " - management port";
        mLabel = mLabel + "_MGMT";
    }
    else if (isDigitString(mPortNumber)) // power or data port
    {
        if ("F" == mPortType)
        {
            mDescription += " - FC port " + mPortNumber;
            mLabel = mLabel + "_P" + mPortNumber;
        }
        else if ("P" == mPortType)
        {
            mDescription += " - power supply " + mPortNumber;
            mLabel += "_PS" + mPortNumber;
        }
        else
        {
            mDescription = c_UnknownPortTypeErrorText;
            mLabel = c_LabelErrorText;
        }
    }
    else
    {
        mDescription = c_InvalidPortNumberErrorText;
        mLabel = c_LabelErrorText;
    }
}

InfinibandSwitchPort::InfinibandSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition, fileRowNumber, fileColumnNumber, c_RequiredNrOfInputParams.at(DeviceTypeID::INFINIBAND_SWITCH), c_MaxAllowedInputCharsCount.at(DeviceTypeID::INFINIBAND_SWITCH), isSourceDevice}
{
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void InfinibandSwitchPort::computeDescriptionAndLabel()
{
    mDescription = "Infiniband switch placed at U" + mDeviceUPosition;
    mLabel = "U" + mDeviceUPosition;

    convertStringCase(mPortType, true);

    if ("m" == mPortNumber || "M" == mPortNumber ) // management port
    {
        mDescription += " - management port";
        mLabel += "_MGMT";
    }
    else if (isDigitString(mPortNumber)) // power or data port
    {
        if ("I" == mPortType)
        {
            mDescription += + " - port " + mPortNumber;
            mLabel += "_P" + mPortNumber;
        }
        else if ("P" == mPortType)
        {
            mDescription += " - power supply " + mPortNumber;
            mLabel += "_PS" + mPortNumber;
        }
        else
        {
            mDescription = c_UnknownPortTypeErrorText;
            mLabel = c_LabelErrorText;
        }
    }
    else
    {
        mDescription = c_InvalidPortNumberErrorText;
        mLabel = c_LabelErrorText;
    }
}

KVMSwitchPort::KVMSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition, fileRowNumber, fileColumnNumber, c_RequiredNrOfInputParams.at(DeviceTypeID::KVM_SWITCH), c_MaxAllowedInputCharsCount.at(DeviceTypeID::KVM_SWITCH), isSourceDevice}
{
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void KVMSwitchPort::computeDescriptionAndLabel()
{
    mDescription = "KVM switch placed at U" + mDeviceUPosition;
    mLabel = "U" + mDeviceUPosition;

    convertStringCase(mPortType, true);

    if (isDigitString(mPortNumber))
    {
        if ("K" == mPortType)
        {
            mDescription += " - port " + mPortNumber;
            mLabel += "_P" + mPortNumber;
        }
        else if ("P" == mPortType)
        {
            mDescription += " - power supply " + mPortNumber;
            mLabel += "_PS" + mPortNumber;
        }
        else
        {
            mDescription = c_UnknownPortTypeErrorText;
            mLabel = c_LabelErrorText;
        }
    }
    else
    {
        mDescription = c_InvalidPortNumberErrorText;
        mLabel = c_LabelErrorText;
    }
}

ServerPort::ServerPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition, fileRowNumber, fileColumnNumber, c_RequiredNrOfInputParams.at(DeviceTypeID::RACK_SERVER), c_MaxAllowedInputCharsCount.at(DeviceTypeID::RACK_SERVER), isSourceDevice}
{
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void ServerPort::computeDescriptionAndLabel()
{
    mDescription = "Server placed at U" + mDeviceUPosition;
    mLabel = "U" + mDeviceUPosition;

    convertStringCase(mPortType, true);

    if("m" == mPortNumber || "M" == mPortNumber) // management port
    {
        mDescription += " - management port";
        mLabel += "_MGMT";
    }
    else if ("K" == mPortType) // KVM port
    {
        mDescription += " - KVM port";
        mLabel += "_KVM";
    }
    else if (isDigitString(mPortNumber))
    {
        if("F" == mPortType) // FC port
        {
            mDescription += " - FC port " + mPortNumber;
            mLabel += "_FC_P" + mPortNumber;
        }
        else if("N" == mPortType) // Ethernet port (but not embedded but on PCIe public slot)
        {
            mDescription += " - Ethernet port " + mPortNumber;
            mLabel += "_ETH_P" + mPortNumber;
        }
        else if ("E" == mPortType) // embedded port (Ethernet/Infiniband)
        {
            mDescription += " - embedded port " + mPortNumber;
            mLabel += "_EMB_P" + mPortNumber;
        }
        else if ("I" == mPortType) // Infiniband port (but not embedded but on PCIe public slot)
        {
            mDescription+=" - Infiniband port " + mPortNumber;
            mLabel += "_IB_P" + mPortNumber;
        }
        else if ("S" == mPortType) // SAS port
        {
            mDescription += " - SAS port " + mPortNumber;
            mLabel += "_SAS_P" + mPortNumber;
        }
        else if ("P" == mPortType) // power supply
        {
            mDescription += " - power supply " + mPortNumber;
            mLabel += "_PS" + mPortNumber;
        }
        else
        {
            mDescription = c_UnknownPortTypeErrorText;
            mLabel = c_LabelErrorText;
        }
    }
    else
    {
        mDescription = c_InvalidPortNumberErrorText;
        mLabel = c_LabelErrorText;
    }
}

StoragePort::StoragePort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition, fileRowNumber, fileColumnNumber, c_RequiredNrOfInputParams.at(DeviceTypeID::STORAGE), c_MaxAllowedInputCharsCount.at(DeviceTypeID::STORAGE), isSourceDevice}
{
    _registerRequiredParameter(&mControllerNr);
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void StoragePort::computeDescriptionAndLabel()
{
    mDescription = "Storage device placed at U" + mDeviceUPosition;
    mLabel = "U" + mDeviceUPosition;

    convertStringCase(mPortType, true);

    if ("m" == mPortNumber || "M" == mPortNumber)
    {
        if ("m" == mControllerNr || "M" == mControllerNr)
        {
            mDescription += " - management port"; // single management port
            mLabel += "_MGMT";
        }
        else
        {
            mDescription += " - controller " + mControllerNr + " - management port"; // one management port per controller
            mLabel += "_C" + mControllerNr + "_MGMT";
        }
    }
    else if (isDigitString(mPortNumber))
    {
        if ("D" == mPortType) // data port (FC, SAS, etc)
        {
            if (isDigitString(mControllerNr))
            {
                mDescription = "Storage device placed at U" + mDeviceUPosition + " - controller " + mControllerNr + " - port " + mPortNumber;
                mLabel = "U" + mDeviceUPosition + "_C" + mControllerNr + "_P" + mPortNumber;
            }
            else
            {
                mDescription = c_InvalidControllerNumberErrorText;
                mLabel = c_LabelErrorText;
            }
        }
        else if ("P" == mPortType) // power supply
        {
            mDescription += " - power supply " + mPortNumber;
            mLabel += "_PS" + mPortNumber;
        }
        else
        {
            mDescription = c_UnknownPortTypeErrorText;
            mLabel = c_LabelErrorText;
        }
    }
    else
    {
        mDescription = c_InvalidPortNumberErrorText;
        mLabel = c_LabelErrorText;
    }
}

/* In order to avoid registering an additional parameter that might cause some confusion (and potential issues),
   for blades the power supply is considered a module, not a port:
    - instead of mPortType: mModuleType is used for power supply
    - instead of mPortNumber: mModuleNumber is used for power supply number
*/
BladeServerPort::BladeServerPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition, fileRowNumber, fileColumnNumber, c_RequiredNrOfInputParams.at(DeviceTypeID::BLADE_SERVER), c_MaxAllowedInputCharsCount.at(DeviceTypeID::BLADE_SERVER), isSourceDevice}
{
    _registerRequiredParameter(&mModuleType);
    _registerRequiredParameter(&mModuleNumber);
    _registerRequiredParameter(&mPortNumber);
}

void BladeServerPort::computeDescriptionAndLabel()
{
    mDescription = "Blade system placed at U" + mDeviceUPosition;
    mLabel = "U" + mDeviceUPosition;

    convertStringCase(mModuleType, true);

    if ("UP" == mModuleType) // management uplink port (for daisy chaining multiple blade systems)
    {
        mDescription += " - management uplink port";
        mLabel += "_MG_UP";
    }
    else if ("DO" == mModuleType) // management downlink port (for daisy chaining multiple blade systems)
    {
        mDescription += " - management downlink port";
        mLabel += "_MG_DO";
    }
    else if (isDigitString(mModuleNumber))
    {
        if ("DM" == mModuleType) // data module
        {
            if (isDigitString(mPortNumber))
            {
                mDescription += " - data module " + mModuleNumber + " - port " + mPortNumber;
                mLabel += "_DMO" + mModuleNumber + "_P" + mPortNumber;
            }
            else
            {
                mDescription = c_InvalidPortNumberErrorText;
                mLabel = c_LabelErrorText;
            }
        }
        else if ("MG" == mModuleType) // management module
        {
            mDescription += " - management module " + mModuleNumber;
            mLabel += "_MGMT" + mModuleNumber;
        }
        else if ("P" == mModuleType)
        {
            mDescription += " - power supply " + mModuleNumber;
            mLabel += "_PS" + mModuleNumber;
        }
        else
        {
            mDescription = c_UnknownModuleTypeErrorText;
            mLabel = c_LabelErrorText;
        }
    }
    else
    {
        mDescription = c_InvalidModuleNumberErrorText;
        mLabel = c_LabelErrorText;
    }
}