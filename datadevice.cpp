#include "datadevice.h"

LANSwitch::LANSwitch(bool isSourceDevice)
    : Device{"lan", c_MaxAllowedNrOfChars.at("lan"), isSourceDevice}
{
    // device parameters filled in connectioninput.csv
    mInputData.resize(Device::scRequiredNrOfInputDataFields);
    mInputData[0] = &mDeviceName;
    mInputData[1] = &mPortNumber;
    mInputData[2] = &mPlaceholder1;
    mInputData[3] = &mPlaceholder2;
}

void LANSwitch::computeDescriptionAndLabel()
{
    mDescription = "LAN switch placed at U" + mDeviceName + " - Ethernet port " + mPortNumber;
    mLabel = "U" + mDeviceName + "_P" + mPortNumber;
}

SANSwitch::SANSwitch(bool isSourceDevice)
    : Device{"san", c_MaxAllowedNrOfChars.at("san"), isSourceDevice}
{
    mInputData.resize(Device::scRequiredNrOfInputDataFields);

    mInputData[0] = &mDeviceName;
    mInputData[1] = &mPortNumber;
    mInputData[2] = &mPlaceholder1;
    mInputData[3] = &mPlaceholder2;
}

void SANSwitch::computeDescriptionAndLabel()
{
    mDescription = "SAN Switch placed at U" + mDeviceName;
    mLabel = "U" + mDeviceName;

    // management port vs. data port
    if ("m" == mPortNumber || "M" == mPortNumber)
    {
        mDescription = mDescription + " - management port";
        mLabel = mLabel + "_MGMT";
    }
    else
    {
        mDescription = mDescription + " - FC port " + mPortNumber;
        mLabel = mLabel + "_P" + mPortNumber;
    }
}

InfinibandSwitch::InfinibandSwitch(bool isSourceDevice)
    : Device{"ib", c_MaxAllowedNrOfChars.at("ib"), isSourceDevice}
{
    mInputData.resize(Device::scRequiredNrOfInputDataFields);

    mInputData[0] = &mDeviceName;
    mInputData[1] = &mPortNumber;
    mInputData[2] = &mPlaceholder1;
    mInputData[3] = &mPlaceholder2;
}

void InfinibandSwitch::computeDescriptionAndLabel()
{
    mDescription = "Infiniband switch placed at U" + mDeviceName;
    mLabel = "U" + mDeviceName;

     // management port vs. data port
    if ("m" == mPortNumber  || "M" == mPortNumber )
    {
        mDescription = mDescription + " - management port";
        mLabel = mLabel + "_MGMT";
    }
    else
    {
        mDescription = mDescription + " - port " + mPortNumber;
        mLabel = mLabel + "_P" + mPortNumber;
    }
}

KVMSwitch::KVMSwitch(bool isSourceDevice)
    : Device{"kvm", c_MaxAllowedNrOfChars.at("kvm"), isSourceDevice}
{
    mInputData.resize(Device::scRequiredNrOfInputDataFields);

    mInputData[0] = &mDeviceName;
    mInputData[1] = &mPortNumber;
    mInputData[2] = &mPlaceholder1;
    mInputData[3] = &mPlaceholder2;
}

void KVMSwitch::computeDescriptionAndLabel()
{
    mDescription = "KVM switch placed at U" + mDeviceName + " - port " + mPortNumber;
    mLabel = "U" + mDeviceName + "_P" + mPortNumber;
}

Server::Server(bool isSourceDevice)
    : Device{"svr", 8, isSourceDevice}
{
    mInputData.resize(Device::scRequiredNrOfInputDataFields);

    mInputData[0] = &mDeviceName;
    mInputData[1] = &mPortType;
    mInputData[2] = &mPortNumber;
    mInputData[3] = &mPlaceholder;
}

void Server::computeDescriptionAndLabel()
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
    else
    {
        mDescription += " - UNKNOWN PORT TYPE (PLEASE CHECK INPUT FILE connectioninput.csv)";
        mLabel += "ERROR!!!";
    }
}

Storage::Storage(bool isSourceDevice)
    : Device{"sto", c_MaxAllowedNrOfChars.at("sto"), isSourceDevice}
{
    mInputData.resize(Device::scRequiredNrOfInputDataFields);

    mInputData[0] = &mDeviceName;
    mInputData[1] = &mControllerNr;
    mInputData[2] = &mPortNumber;
    mInputData[3] = &mPlaceholder;
}

void Storage::computeDescriptionAndLabel()
{
    const bool c_IsManagementPort{"m" == mPortNumber || "M" == mPortNumber};
    const bool c_IsManagementController{"m" == mControllerNr || "M" == mControllerNr};

    if (c_IsManagementController && c_IsManagementPort)
    {
        mDescription = "Storage device placed at U" + mDeviceName + " - management port"; // special case: a single management port
        mLabel = "U" + mDeviceName + "_MGMT"; // special case: a single management port
    }
    else if (c_IsManagementPort) // special case: a management port per controller
    {
        mDescription = "Storage device placed at U" + mDeviceName + " - controller " + mControllerNr + " - management port";
        mLabel = "U" + mDeviceName + "_C" + mControllerNr + "_MGMT";
    }
    else // general case: data port
    {
        mDescription = "Storage device placed at U" + mDeviceName + " - controller " + mControllerNr + " - port " + mPortNumber;
        mLabel = "U" + mDeviceName + "_C" + mControllerNr + "_P" + mPortNumber;
    }
}

BladeServer::BladeServer(bool isSourceDevice)
    : Device{"bld", c_MaxAllowedNrOfChars.at("bld"), isSourceDevice}
{
    mInputData.resize(Device::scRequiredNrOfInputDataFields);

    mInputData[0] = &mDeviceName;
    mInputData[1] = &mModuleType;
    mInputData[2] = &mModuleNumber;
    mInputData[3] = &mPortNumber;
}

void BladeServer::computeDescriptionAndLabel()
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
    else
    {
        mDescription += " - UNKNOWN DEVICE! PLEASE CHECK INPUT FILE (connectioninput.csv)";
        mLabel += " - ERROR!";
    }
}
