#include <cassert>

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
    mDescription = "LAN switch placed at U" + mDeviceUPosition;
    mLabel = "U" + mDeviceUPosition;

    Utilities::convertStringCase(mPortType, true);

    if (Utilities::isDigitString(mPortNumber)) // no management port
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
    mDescription = "SAN switch placed at U" + mDeviceUPosition;
    mLabel = "U" + mDeviceUPosition;

    Utilities::convertStringCase(mPortType, true);

    if ("m" == mPortNumber || "M" == mPortNumber) // management port
    {
        mDescription += " - management port";
        mLabel = mLabel + "_MGMT";
    }
    else if (Utilities::isDigitString(mPortNumber)) // power or data port
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
    mDescription = "Infiniband switch placed at U" + mDeviceUPosition;
    mLabel = "U" + mDeviceUPosition;

    Utilities::convertStringCase(mPortType, true);

    if ("m" == mPortNumber || "M" == mPortNumber ) // management port
    {
        mDescription += " - management port";
        mLabel += "_MGMT";
    }
    else if (Utilities::isDigitString(mPortNumber)) // power or data port
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
    mDescription = "KVM switch placed at U" + mDeviceUPosition;
    mLabel = "U" + mDeviceUPosition;

    Utilities::convertStringCase(mPortType, true);

    if (Utilities::isDigitString(mPortNumber))
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
    mDescription = "Server placed at U" + mDeviceUPosition;
    mLabel = "U" + mDeviceUPosition;

    Utilities::convertStringCase(mPortType, true);

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
    else if (Utilities::isDigitString(mPortNumber))
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
        _handleMultipleInstanceModule();
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

void BladeServerPort::_handleMultipleInstanceModule()
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
