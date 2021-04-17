#include "datadeviceport.h"

LANSwitchPort::LANSwitchPort(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::LAN_SWITCH), c_MaxAllowedCharsCount.at(DeviceTypeID::LAN_SWITCH), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void LANSwitchPort::computeDescriptionAndLabel()
{
    mDescription = "LAN switch placed at U" + mDeviceName;
    mLabel = "U" + mDeviceName;

    convertStringCase(mPortType, mPortType, true);

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
        mDescription += c_UnknownPortTypeErrorText;
        mLabel += c_LabelErrorText;
    }
}

SANSwitchPort::SANSwitchPort(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::SAN_SWITCH), c_MaxAllowedCharsCount.at(DeviceTypeID::SAN_SWITCH), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void SANSwitchPort::computeDescriptionAndLabel()
{
    mDescription = "SAN switch placed at U" + mDeviceName;
    mLabel = "U" + mDeviceName;

    convertStringCase(mPortType, mPortType, true);

    // management port vs. data port
    if ("m" == mPortNumber || "M" == mPortNumber)
    {
        mDescription += " - management port";
        mLabel = mLabel + "_MGMT";
    }
    else if ("F" == mPortType)
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
        mDescription += c_UnknownPortTypeErrorText;
        mLabel += c_LabelErrorText;
    }
}

InfinibandSwitchPort::InfinibandSwitchPort(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::INFINIBAND_SWITCH), c_MaxAllowedCharsCount.at(DeviceTypeID::INFINIBAND_SWITCH), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void InfinibandSwitchPort::computeDescriptionAndLabel()
{
    mDescription = "Infiniband switch placed at U" + mDeviceName;
    mLabel = "U" + mDeviceName;

    convertStringCase(mPortType, mPortType, true);

     // management port vs. data port
    if ("m" == mPortNumber  || "M" == mPortNumber )
    {
        mDescription += " - management port";
        mLabel += "_MGMT";
    }
    else if ("I" == mPortType)
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
        mDescription += c_UnknownPortTypeErrorText;
        mLabel += c_LabelErrorText;
    }
}

KVMSwitchPort::KVMSwitchPort(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::KVM_SWITCH), c_MaxAllowedCharsCount.at(DeviceTypeID::KVM_SWITCH), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void KVMSwitchPort::computeDescriptionAndLabel()
{
    mDescription = "KVM switch placed at U" + mDeviceName;
    mLabel = "U" + mDeviceName;

    convertStringCase(mPortType, mPortType, true);

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
        mDescription += c_UnknownPortTypeErrorText;
        mLabel += c_LabelErrorText;
    }
}

ServerPort::ServerPort(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::RACK_SERVER), c_MaxAllowedCharsCount.at(DeviceTypeID::RACK_SERVER), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void ServerPort::computeDescriptionAndLabel()
{
    mDescription = "Server placed at U" + mDeviceName;
    mLabel = "U" + mDeviceName;

    convertStringCase(mPortType, mPortType, true);

    if("m" == mPortNumber || "M" == mPortNumber) // management port
    {
        mDescription += " - management port";
        mLabel += "_MGMT";
    }
    else if("F" == mPortType) // FC port
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
    else if ("K" == mPortType) // KVM port
    {
        mDescription += " - KVM port";
        mLabel += "_KVM";
    }
    else if ("P" == mPortType) // power supply
    {
        mDescription += " - power supply " + mPortNumber;
        mLabel += "_PS" + mPortNumber;
    }
    else
    {
        mDescription += c_UnknownPortTypeErrorText;
        mLabel += c_LabelErrorText;
    }
}

StoragePort::StoragePort(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::STORAGE), c_MaxAllowedCharsCount.at(DeviceTypeID::STORAGE), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mControllerNr);
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void StoragePort::computeDescriptionAndLabel()
{
    mDescription = "Storage device placed at U" + mDeviceName;
    mLabel = "U" + mDeviceName;

    convertStringCase(mPortType, mPortType, true);

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
    else if ("D" == mPortType) // data port (FC, SAS, etc)
    {
        mDescription = "Storage device placed at U" + mDeviceName + " - controller " + mControllerNr + " - port " + mPortNumber;
        mLabel = "U" + mDeviceName + "_C" + mControllerNr + "_P" + mPortNumber;
    }
    else if ("P" == mPortType) // power supply
    {
        mDescription += " - power supply " + mPortNumber;
        mLabel += "_PS" + mPortNumber;
    }
    else
    {
        mDescription += c_UnknownPortTypeErrorText;
        mLabel += c_LabelErrorText;
    }
}

/* In order to avoid registering an additional parameter that might cause some confusion (and potential issues),
   for blades the power supply is considered a module, not a port:
    - instead of mPortType: mModuleType is used for power supply
    - instead of mPortNumber: mModuleNumber is used for power supply number
*/
BladeServerPort::BladeServerPort(bool isSourceDevice)
    : DevicePort{c_RequiredNrOfInputParams.at(DeviceTypeID::BLADE_SERVER), c_MaxAllowedCharsCount.at(DeviceTypeID::BLADE_SERVER), isSourceDevice}
{
    _registerRequiredParameter(&mDeviceName);
    _registerRequiredParameter(&mModuleType);
    _registerRequiredParameter(&mModuleNumber);
    _registerRequiredParameter(&mPortNumber);
}

void BladeServerPort::computeDescriptionAndLabel()
{
    mDescription = "Blade system placed at U" + mDeviceName;
    mLabel = "U" + mDeviceName;

    convertStringCase(mModuleType, mModuleType, true);

    if ("DM" == mModuleType) // data module
    {
        mDescription += " - data module " + mModuleNumber + " - port " + mPortNumber;
        mLabel += "_DMO" + mModuleNumber + "_P" + mPortNumber;
    }
    else if ("MG" == mModuleType) // management module
    {
        mDescription += " - management module " + mModuleNumber;
        mLabel += "_MGMT" + mModuleNumber;
    }
    else if ("UP" == mModuleType) // management uplink port (for daisy chaining multiple blade systems)
    {
        mDescription += " - management uplink port";
        mLabel += "_MG_UP";
    }
    else if ("DO" == mModuleType) // management downlink port (for daisy chaining multiple blade systems)
    {
        mDescription += " - management downlink port";
        mLabel += "_MG_DO";
    }
    else if ("P" == mModuleType)
    {
        mDescription += " - power supply " + mModuleNumber;
        mLabel += "_PS" + mModuleNumber;
    }
    else
    {
        mDescription += c_UnknownModuleTypeErrorText;
        mLabel += c_LabelErrorText;
    }
}
