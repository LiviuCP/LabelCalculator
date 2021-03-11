#include <map>

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

void LANSwitch::buildDescriptionText()
{
    mDescription = "LAN switch placed at U" + mDeviceName + " - Ethernet port " + mPortNumber;
}

void LANSwitch::buildLabelText()
{
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

void SANSwitch::buildDescriptionText()
{
    mDescription="SAN Switch placed at U" + mDeviceName;

    // management port vs. data port
    if ("m" == mPortNumber || "M" == mPortNumber)
    {
        mDescription = mDescription + " - management port";
    }
    else
    {
        mDescription = mDescription + " - FC port " + mPortNumber;
    }
}

void SANSwitch::buildLabelText()
{
    mLabel = "U" + mDeviceName;

    // management port vs. data port
    if ("m" == mPortNumber  || "M" == mPortNumber)
    {
        mLabel = mLabel + "_MGMT";
    }
    else
    {
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

void InfinibandSwitch::buildDescriptionText()
{
    mDescription = "Infiniband switch placed at U" + mDeviceName;

     // management port vs. data port
    if ("m" == mPortNumber  || "M" == mPortNumber )
    {
        mDescription = mDescription + " - management port";
    }
    else
    {
        mDescription = mDescription + " - port " + mPortNumber;
    }
}

void InfinibandSwitch::buildLabelText()
{
    mLabel = "U" + mDeviceName;

    // management port vs. data port
    if ("m" == mPortNumber || "M" == mPortNumber)
    {
        mLabel = mLabel + "_MGMT";
    }
    else
    {
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

void KVMSwitch::buildDescriptionText()
{
    mDescription = "KVM switch placed at U" + mDeviceName + " - port " + mPortNumber;
}

void KVMSwitch::buildLabelText()
{
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

void Server::buildDescriptionText()
{
    mPortType[0] = toupper(mPortType[0]);
    mDescription = "Server placed at U" + mDeviceName;

    if("m" == mPortNumber || "M" == mPortNumber) // management port
    {
        mDescription += " - management port";
    }
    else if("F" == mPortType) // FC port
    {
        mDescription += " - FC port " + mPortNumber;
    }
    else if("N" == mPortType) // Ethernet port (but not embedded but on PCIe public slot)
    {
        mDescription += " - Ethernet port " + mPortNumber;
    }
    else if ("E" == mPortType) // embedded port (Ethernet/Infiniband)
    {
        mDescription += " - embedded port " + mPortNumber;
    }
    else if ("I" == mPortType) // Infiniband port (but not embedded but on PCIe public slot)
    {
        mDescription+=" - Infiniband port " + mPortNumber;
    }
    else if ("S" == mPortType) // SAS port
    {
        mDescription += " - SAS port " + mPortNumber;
    }
    else if ("K" == mPortType) // KVM port
    {
        mDescription += " - KVM port";
    }
    else
    {
        mDescription += " - UNKNOWN PORT TYPE (PLEASE CHECK INPUT FILE connectioninput.csv)";
    }
}

void Server::buildLabelText()
{
    mLabel = "U" + mDeviceName;

    if("m" == mPortNumber || "M" == mPortNumber) // management port
    {
        mLabel += "_MGMT";
    }
    else if("F" == mPortType) // FC port
    {
        mLabel += "_FC_P" + mPortNumber;
    }
    else if("N" == mPortType) // Ethernet port (but not embedded but on PCIe public slot)
    {
        mLabel += "_ETH_P" + mPortNumber;
    }
    else if("E" == mPortType) // embedded port (Ethernet sau IB)
    {
        mLabel += "_EMB_P" + mPortNumber;
    }
    else if("I" == mPortType) // Infiniband port (but not embedded but on PCIe public slot)
    {
        mLabel += "_IB_P" + mPortNumber;
    }
    else if("S" == mPortType) // SAS port
    {
        mLabel += "_SAS_P" + mPortNumber;
    }
    else if("K" == mPortType) // KVM port
    {
        mLabel += "_KVM";
    }
    else
    {
        mLabel += "ERROR!!!";
    }
}

Storage::Storage(bool isSourceDevice)
    : Device{"sto", c_MaxAllowedNrOfChars.at("sto"), isSourceDevice}
{
    mInputData.resize(Device::scRequiredNrOfInputDataFields);

    mInputData[0] = &mDeviceName;
    mInputData[1] = &mControllerNr;
    mInputData[2] = &mPortNr;
    mInputData[3] = &mPlaceholder;
}

void Storage::buildDescriptionText()
{
    const bool c_IsManagementPort{"m" == mPortNr || "M" == mPortNr};
    const bool c_IsManagementController{"m" == mControllerNr || "M" == mControllerNr};

    if (c_IsManagementController && c_IsManagementPort)
    {
        mDescription = "Storage device placed at U" + mDeviceName + " - management port"; // special case: a single management port
    }
    else if (c_IsManagementPort) // special case: a management port per controller
    {
        mDescription = "Storage device placed at U" + mDeviceName + " - controller " + mControllerNr + " - management port";
    }
    else // general case: data port
    {
        mDescription = "Storage device placed at U" + mDeviceName + " - controller " + mControllerNr + " - port " + mPortNr;
    }
}

void Storage::buildLabelText()
{
    const bool c_IsManagementPort{"m" == mPortNr || "M" == mPortNr};
    const bool c_IsManagementController{"m" == mControllerNr || "M" == mControllerNr};

    if (c_IsManagementController && c_IsManagementPort)
    {
        mLabel = "U" + mDeviceName + "_MGMT"; // special case: a single management port
    }
    else if (c_IsManagementPort) // special case: a management port per controller
    {
        mLabel = "U" + mDeviceName + "_C" + mControllerNr + "_MGMT";
    }
    else
    {
        mLabel = "U" + mDeviceName + "_C" + mControllerNr + "_P" + mPortNr;
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

void BladeServer::buildDescriptionText()
{
    mDescription = "Blade system placed at U" + mDeviceName;
    mModuleType[0] = toupper(mModuleType[0]);
    mModuleType[1] = toupper(mModuleType[1]);

    if ("DM" == mModuleType) // data module
    {
        mDescription += " - data module " + mModuleNumber + " - port " + mPortNumber;
    }
    else if ("MG" == mModuleType) // management module
    {
        mDescription += " - management module " + mModuleNumber;
    }
    else if ("UP" == mModuleType) // management uplink port (for daisy chaining multiple blade systems)
    {
        mDescription += " - management uplink port";
    }
    else if ("DO" == mModuleType) // management downlink port (for daisy chaining multiple blade systems)
    {
        mDescription += " - management downlink port";
    }
    else
    {
        mDescription += " - UNKNOWN DEVICE! PLEASE CHECK INPUT FILE (connectioninput.csv)";
    }
}

void BladeServer::buildLabelText()
{
    mLabel = "U" + mDeviceName;

    if ("DM" == mModuleType) // data module
    {
        mLabel += "_DMO" + mModuleNumber + "_P" + mPortNumber;
    }
    else if ("MG" == mModuleType) // management module
    {
        mLabel += "_MGMT" + mModuleNumber;
    }
    else if ("UP" == mModuleType) // management module uplink port
    {
        mLabel += "_MG_UP";
    }
    else if ("DO" == mModuleType) // management module downlink port
    {
        mLabel += "_MG_DO";
    }
    else
    {
        mLabel += " - ERROR!";
    }
}
