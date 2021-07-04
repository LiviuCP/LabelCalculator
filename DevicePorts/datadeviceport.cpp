#include <cassert>
#include <set>

#include "applicationdata.h"
#include "applicationutils.h"
#include "deviceportutils.h"
#include "datadeviceport.h"

LANSwitchPort::LANSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::LAN_SWITCH),
                 isSourceDevice}
{
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void LANSwitchPort::computeDescriptionAndLabel()
{
    Utilities::convertStringCase(mPortType, true);

    bool isValidPortType{true};

    if ("N" == mPortType)
    {
        mDescription = "LAN switch placed at U" + mDeviceUPosition + " - Ethernet port ";
        mLabel = "U" + mDeviceUPosition + "_P";
    }
    else if ("P" == mPortType)
    {
        mDescription = "LAN switch placed at U" + mDeviceUPosition + " - power supply ";
        mLabel = "U" + mDeviceUPosition + "_PS";
    }
    else
    {
        mDescription = Utilities::c_InvalidPortTypeErrorText;
        mLabel = Utilities::c_LabelErrorText;
        isValidPortType = false;
    }

    if (isValidPortType)
    {
        if (Utilities::isDigitString(mPortNumber))
        {
            mDescription += mPortNumber;
            mLabel += mPortNumber;
        }
        else
        {
            mDescription = Utilities::c_InvalidPortNumberErrorText;
            mLabel = Utilities::c_LabelErrorText;
        }
    }

    _checkLabel();
}

SANSwitchPort::SANSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::SAN_SWITCH),
                 isSourceDevice}
{
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void SANSwitchPort::computeDescriptionAndLabel()
{
    Utilities::convertStringCase(mPortType, true);

    if ("F" == mPortType || "P" == mPortType)
    {
        _handleNumericPortType();
    }
    else if ("-" == mPortType)
    {
        if ("m" == mPortNumber || "M" == mPortNumber) // management port
        {
            mDescription += "SAN switch placed at U" + mDeviceUPosition + " - management port";
            mLabel = "U" + mDeviceUPosition + "_MGMT";
        }
        else
        {
            mDescription = Utilities::c_InvalidPortNumberErrorText;
            mLabel = Utilities::c_LabelErrorText;
        }
    }
    else
    {
        mDescription = Utilities::c_InvalidPortTypeErrorText;
        mLabel = Utilities::c_LabelErrorText;
    }

    _checkLabel();
}

void SANSwitchPort::_handleNumericPortType()
{
    if (Utilities::isDigitString(mPortNumber)) // power or data port
    {
        if ("F" == mPortType)
        {
            mDescription = "SAN switch placed at U" + mDeviceUPosition + " - FC port " + mPortNumber;
            mLabel = "U" + mDeviceUPosition + "_P" + mPortNumber;
        }
        else if ("P" == mPortType)
        {
            mDescription = "SAN switch placed at U" + mDeviceUPosition + " - power supply " + mPortNumber;
            mLabel = "U" + mDeviceUPosition + "_PS" + mPortNumber;
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        mDescription = Utilities::c_InvalidPortNumberErrorText;
        mLabel = Utilities::c_LabelErrorText;
    }
}

InfinibandSwitchPort::InfinibandSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::INFINIBAND_SWITCH),
                 isSourceDevice}
{
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void InfinibandSwitchPort::computeDescriptionAndLabel()
{
    Utilities::convertStringCase(mPortType, true);

    if ("I" == mPortType || "P" == mPortType)
    {
        _handleNumberedPortType();
    }
    else if ("-" == mPortType)
    {
        if ("m" == mPortNumber || "M" == mPortNumber) // management port
        {
            mDescription += "Infiniband switch placed at U" + mDeviceUPosition + " - management port";
            mLabel = "U" + mDeviceUPosition + "_MGMT";
        }
        else
        {
            mDescription = Utilities::c_InvalidPortNumberErrorText;
            mLabel = Utilities::c_LabelErrorText;
        }
    }
    else
    {
        mDescription = Utilities::c_InvalidPortTypeErrorText;
        mLabel = Utilities::c_LabelErrorText;
    }

    _checkLabel();
}

void InfinibandSwitchPort::_handleNumberedPortType()
{
    if (Utilities::isDigitString(mPortNumber)) // power or data port
    {
        if ("I" == mPortType)
        {
            mDescription = "Infiniband switch placed at U" + mDeviceUPosition + " - port " + mPortNumber;
            mLabel = "U" + mDeviceUPosition + "_P" + mPortNumber;
        }
        else if ("P" == mPortType)
        {
            mDescription = "Infiniband switch placed at U" + mDeviceUPosition + " - power supply " + mPortNumber;
            mLabel = "U" + mDeviceUPosition + "_PS" + mPortNumber;
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        mDescription = Utilities::c_InvalidPortNumberErrorText;
        mLabel = Utilities::c_LabelErrorText;
    }
}

KVMSwitchPort::KVMSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::KVM_SWITCH),
                 isSourceDevice}
{
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void KVMSwitchPort::computeDescriptionAndLabel()
{
    Utilities::convertStringCase(mPortType, true);

    bool isValidPortType{true};

    if ("K" == mPortType)
    {
        mDescription = "KVM switch placed at U" + mDeviceUPosition + " - port ";
        mLabel = "U" + mDeviceUPosition + "_P";
    }
    else if ("P" == mPortType)
    {
        mDescription = "KVM switch placed at U" + mDeviceUPosition + " - power supply ";
        mLabel = "U" + mDeviceUPosition + "_PS";
    }
    else
    {
        mDescription = Utilities::c_InvalidPortTypeErrorText;
        mLabel = Utilities::c_LabelErrorText;
        isValidPortType = false;
    }

    if (isValidPortType)
    {
        if (Utilities::isDigitString(mPortNumber))
        {
            mDescription += mPortNumber;
            mLabel += mPortNumber;
        }
        else
        {
            mDescription = Utilities::c_InvalidPortNumberErrorText;
            mLabel = Utilities::c_LabelErrorText;
        }
    }

    _checkLabel();
}

ServerPort::ServerPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::RACK_SERVER),
                 isSourceDevice}
{
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void ServerPort::computeDescriptionAndLabel()
{
    Utilities::convertStringCase(mPortType, true);

    std::set<std::string> c_NumberedPortTypes{"F", "N", "E", "I", "S", "P"};

    if (c_NumberedPortTypes.find(mPortType) != c_NumberedPortTypes.cend())
    {
        _handleNumberedPortType();
    }
    else if ("K" == mPortType) // KVM port
    {
        mDescription = "Server placed at U" + mDeviceUPosition + " - KVM port";
        mLabel = "U" + mDeviceUPosition + "_KVM";
    }
    else if ("-" == mPortType)
    {
        if("m" == mPortNumber || "M" == mPortNumber) // management port
        {
            mDescription = "Server placed at U" + mDeviceUPosition + " - management port";
            mLabel = "U" + mDeviceUPosition + "_MGMT";
        }
        else
        {
            mDescription = Utilities::c_InvalidPortNumberErrorText;
            mLabel = Utilities::c_LabelErrorText;
        }
    }
    else
    {
        mDescription = Utilities::c_InvalidPortTypeErrorText;
        mLabel = Utilities::c_LabelErrorText;
    }

    _checkLabel();
}

void ServerPort::_handleNumberedPortType()
{
    if (Utilities::isDigitString(mPortNumber))
    {
        if("F" == mPortType) // FC port
        {
            mDescription = "Server placed at U" + mDeviceUPosition + " - FC port " + mPortNumber;
            mLabel = "U" + mDeviceUPosition + "_FC_P" + mPortNumber;
        }
        else if("N" == mPortType) // Ethernet port (but not embedded but on PCIe public slot)
        {
            mDescription = "Server placed at U" + mDeviceUPosition + " - Ethernet port " + mPortNumber;
            mLabel = "U" + mDeviceUPosition + "_ETH_P" + mPortNumber;
        }
        else if ("E" == mPortType) // embedded port (Ethernet/Infiniband)
        {
            mDescription = "Server placed at U" + mDeviceUPosition + " - embedded port " + mPortNumber;
            mLabel = "U" + mDeviceUPosition + "_EMB_P" + mPortNumber;
        }
        else if ("I" == mPortType) // Infiniband port (but not embedded but on PCIe public slot)
        {
            mDescription = "Server placed at U" + mDeviceUPosition + " - Infiniband port " + mPortNumber;
            mLabel = "U" + mDeviceUPosition + "_IB_P" + mPortNumber;
        }
        else if ("S" == mPortType) // SAS port
        {
            mDescription = "Server placed at U" + mDeviceUPosition + " - SAS port " + mPortNumber;
            mLabel = "U" + mDeviceUPosition + "_SAS_P" + mPortNumber;
        }
        else if ("P" == mPortType) // power supply
        {
            mDescription = "Server placed at U" + mDeviceUPosition + " - power supply " + mPortNumber;
            mLabel = "U" + mDeviceUPosition + "_PS" + mPortNumber;
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        mDescription = Utilities::c_InvalidPortNumberErrorText;
        mLabel = Utilities::c_LabelErrorText;
    }
}

StoragePort::StoragePort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::STORAGE),
                 isSourceDevice}
{
    _registerRequiredParameter(&mControllerNr);
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void StoragePort::computeDescriptionAndLabel()
{
    mDescription = "Storage device placed at U" + mDeviceUPosition;
    mLabel = "U" + mDeviceUPosition;

    Utilities::convertStringCase(mPortType, true);

    if ("m" == mPortNumber || "M" == mPortNumber)
    {
        if ("m" == mControllerNr || "M" == mControllerNr)
        {
            mDescription += " - management port"; // single management port
            mLabel += "_MGMT";
        }
        else if (Utilities::isDigitString(mControllerNr))
        {
            mDescription += " - controller " + mControllerNr + " - management port"; // one management port per controller
            mLabel += "_C" + mControllerNr + "_MGMT";
        }
        else
        {
            mDescription = Utilities::c_InvalidControllerNumberErrorText;
            mLabel = Utilities::c_LabelErrorText;
        }
    }
    else if (Utilities::isDigitString(mPortNumber))
    {
        if ("D" == mPortType) // data port (FC, SAS, etc)
        {
            if (Utilities::isDigitString(mControllerNr))
            {
                mDescription = "Storage device placed at U" + mDeviceUPosition + " - controller " + mControllerNr + " - port " + mPortNumber;
                mLabel = "U" + mDeviceUPosition + "_C" + mControllerNr + "_P" + mPortNumber;
            }
            else
            {
                mDescription = Utilities::c_InvalidControllerNumberErrorText;
                mLabel = Utilities::c_LabelErrorText;
            }
        }
        else if ("P" == mPortType) // power supply
        {
            mDescription += " - power supply " + mPortNumber;
            mLabel += "_PS" + mPortNumber;
        }
        else
        {
            mDescription = Utilities::c_InvalidPortTypeErrorText;
            mLabel = Utilities::c_LabelErrorText;
        }
    }
    else
    {
        mDescription = Utilities::c_InvalidPortNumberErrorText;
        mLabel = Utilities::c_LabelErrorText;
    }

    _checkLabel();
}

/* In order to avoid registering an additional parameter that might cause some confusion (and potential issues),
   for blades the power supply is considered a module, not a port:
    - instead of mPortType: mModuleType is used for power supply
    - instead of mPortNumber: mModuleNumber is used for power supply number
*/
BladeServerPort::BladeServerPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::BLADE_SERVER),
                 isSourceDevice}
{
    _registerRequiredParameter(&mModuleType);
    _registerRequiredParameter(&mModuleNumber);
    _registerRequiredParameter(&mPortNumber);
}

void BladeServerPort::computeDescriptionAndLabel()
{
    Utilities::convertStringCase(mModuleType, true);

    if ("DM" == mModuleType || "MG" == mModuleType || "P" == mModuleType)
    {
        _handleNumberedModuleType();
    }
    else if ("UP" == mModuleType) // management uplink port (for daisy chaining multiple blade systems)
    {
        mDescription = "Blade system placed at U" + mDeviceUPosition + " - management uplink port";
        mLabel = "U" + mDeviceUPosition + "_MG_UP";
    }
    else if ("DO" == mModuleType) // management downlink port (for daisy chaining multiple blade systems)
    {
        mDescription = "Blade system placed at U" + mDeviceUPosition + " - management downlink port";
        mLabel += "U" + mDeviceUPosition + "_MG_DO";
    }
    else
    {
        mDescription = Utilities::c_InvalidModuleTypeErrorText;
        mLabel = Utilities::c_LabelErrorText;
    }

    _checkLabel();
}

void BladeServerPort::_handleNumberedModuleType()
{
    if (Utilities::isDigitString(mModuleNumber))
    {
        mDescription = "Blade system placed at U" + mDeviceUPosition;
        mLabel = "U" + mDeviceUPosition;

        if ("DM" == mModuleType) // data module
        {
            if (Utilities::isDigitString(mPortNumber))
            {
                mDescription += " - data module " + mModuleNumber + " - port " + mPortNumber;
                mLabel += "_DMO" + mModuleNumber + "_P" + mPortNumber;
            }
            else
            {
                mDescription = Utilities::c_InvalidPortNumberErrorText;
                mLabel = Utilities::c_LabelErrorText;
            }
        }
        else if ("MG" == mModuleType) // management module
        {
            mDescription += " - management module " + mModuleNumber;
            mLabel += "_MGMT" + mModuleNumber;
        }
        else if ("P" == mModuleType) // power supply
        {
            mDescription += " - power supply " + mModuleNumber;
            mLabel += "_PS" + mModuleNumber;
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        mDescription = Utilities::c_InvalidModuleNumberErrorText;
        mLabel = Utilities::c_LabelErrorText;
    }
}
