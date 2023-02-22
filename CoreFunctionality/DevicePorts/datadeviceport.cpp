#include <cassert>
#include <set>

#include "applicationdata.h"
#include "coreutils.h"
#include "deviceportutils.h"
#include "datadeviceport.h"

namespace Core = Utilities::Core;
namespace Ports = Utilities::DevicePorts;

LANSwitchPort::LANSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::LAN_SWITCH),
                 isSourceDevice}
{
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);

    _initializeDescriptionAndLabel("LAN switch");
}

void LANSwitchPort::computeDescriptionAndLabel()
{
    Core::convertStringCase(mPortType, true);

    bool isValidPortType{true};

    if ("N" == mPortType)
    {
        _appendDataToDescription(" - Ethernet port ");
        _appendDataToLabel("_P");
    }
    else if ("P" == mPortType)
    {
        _appendDataToDescription(" - power supply ");
        _appendDataToLabel("_PS");
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
        isValidPortType = false;
    }

    if (isValidPortType)
    {
        if (Core::isDigitString(mPortNumber))
        {
            _appendDataToDescription(mPortNumber);
            _appendDataToLabel(mPortNumber);
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
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

    _initializeDescriptionAndLabel("SAN switch");
}

void SANSwitchPort::computeDescriptionAndLabel()
{
    Core::convertStringCase(mPortType, true);

    if ("F" == mPortType || "P" == mPortType)
    {
        _handleNumberedPortType();
    }
    else if ("-" == mPortType)
    {
        if ("m" == mPortNumber || "M" == mPortNumber) // management port
        {
            _appendDataToDescription(" - management port");
            _appendDataToLabel("_MGMT");
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
    }

    _checkLabel();
}

void SANSwitchPort::_handleNumberedPortType()
{
    if (Core::isDigitString(mPortNumber)) // power or data port
    {
        if ("F" == mPortType)
        {
            _appendDataToDescription(" - FC port " + mPortNumber);
            _appendDataToLabel("_P" + mPortNumber);
        }
        else if ("P" == mPortType)
        {
            _appendDataToDescription(" - power supply " + mPortNumber);
            _appendDataToLabel("_PS" + mPortNumber);
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
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

    _initializeDescriptionAndLabel("Infiniband switch");
}

void InfinibandSwitchPort::computeDescriptionAndLabel()
{
    Core::convertStringCase(mPortType, true);

    if ("I" == mPortType || "P" == mPortType)
    {
        _handleNumberedPortType();
    }
    else if ("-" == mPortType)
    {
        if ("m" == mPortNumber || "M" == mPortNumber) // management port
        {
            _appendDataToDescription(" - management port");
            _appendDataToLabel("_MGMT");
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
    }

    _checkLabel();
}

void InfinibandSwitchPort::_handleNumberedPortType()
{
    if (Core::isDigitString(mPortNumber)) // power or data port
    {
        if ("I" == mPortType)
        {
            _appendDataToDescription(" - port " + mPortNumber);
            _appendDataToLabel("_P" + mPortNumber);
        }
        else if ("P" == mPortType)
        {
            _appendDataToDescription(" - power supply " + mPortNumber);
            _appendDataToLabel("_PS" + mPortNumber);
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
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

    _initializeDescriptionAndLabel("KVM switch");
}

void KVMSwitchPort::computeDescriptionAndLabel()
{
    Core::convertStringCase(mPortType, true);

    bool isValidPortType{true};

    if ("K" == mPortType)
    {
        _appendDataToDescription(" - port ");
        _appendDataToLabel("_P");
    }
    else if ("P" == mPortType)
    {
        _appendDataToDescription(" - power supply ");
        _appendDataToLabel("_PS");
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
        isValidPortType = false;
    }

    if (isValidPortType)
    {
        if (Core::isDigitString(mPortNumber))
        {
            _appendDataToDescription(mPortNumber);
            _appendDataToLabel(mPortNumber);
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
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

    _initializeDescriptionAndLabel("Server");
}

void ServerPort::computeDescriptionAndLabel()
{
    Core::convertStringCase(mPortType, true);

    std::set<std::string> c_NumberedPortTypes{"F", "N", "E", "I", "S", "P"};

    if (c_NumberedPortTypes.find(mPortType) != c_NumberedPortTypes.cend())
    {
        _handleNumberedPortType();
    }
    else if ("K" == mPortType) // KVM port
    {
        _appendDataToDescription(" - KVM port");
        _appendDataToLabel("_KVM");
    }
    else if ("-" == mPortType)
    {
        if("m" == mPortNumber || "M" == mPortNumber) // management port
        {
            _appendDataToDescription(" - management port");
            _appendDataToLabel("_MGMT");
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
    }

    _checkLabel();
}

void ServerPort::_handleNumberedPortType()
{
    if (Core::isDigitString(mPortNumber))
    {
        if("F" == mPortType) // FC port
        {
            _appendDataToDescription(" - FC port " + mPortNumber);
            _appendDataToLabel("_FC_P" + mPortNumber);
        }
        else if("N" == mPortType) // Ethernet port (but not embedded but on PCIe public slot)
        {
            _appendDataToDescription(" - Ethernet port " + mPortNumber);
            _appendDataToLabel("_ETH_P" + mPortNumber);
        }
        else if ("E" == mPortType) // embedded port (Ethernet/Infiniband)
        {
            _appendDataToDescription(" - embedded port " + mPortNumber);
            _appendDataToLabel("_EMB_P" + mPortNumber);
        }
        else if ("I" == mPortType) // Infiniband port (but not embedded but on PCIe public slot)
        {
            _appendDataToDescription(" - Infiniband port " + mPortNumber);
            _appendDataToLabel("_IB_P" + mPortNumber);
        }
        else if ("S" == mPortType) // SAS port
        {
            _appendDataToDescription(" - SAS port " + mPortNumber);
            _appendDataToLabel("_SAS_P" + mPortNumber);
        }
        else if ("P" == mPortType) // power supply
        {
            _appendDataToDescription(" - power supply " + mPortNumber);
            _appendDataToLabel("_PS" + mPortNumber);
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
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

    _initializeDescriptionAndLabel("Storage device");
}

void StoragePort::computeDescriptionAndLabel()
{
    bool shouldCheckNumberedPortType{false};
    bool shouldCheckManagementPort{false};

    Core::convertStringCase(mPortType, true);

    if (Core::isDigitString(mControllerNr))
    {
        if ("D" == mPortType) // data port (e.g. FC)
        {
            _appendDataToDescription(" - controller " + mControllerNr + " - port ");
            _appendDataToLabel("_C" + mControllerNr + "_P");

            shouldCheckNumberedPortType = true;
        }
        else if ("-" == mPortType)
        {
            _appendDataToDescription(" - controller " + mControllerNr);
            _appendDataToLabel("_C" + mControllerNr);

            shouldCheckManagementPort = true; // one management port per controller
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
        }
    }
    else if ("m" == mControllerNr || "M" == mControllerNr)
    {
        if ("-" == mPortType)
        {
            shouldCheckManagementPort = true; // unified management port
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
        }
    }
    else if ("-" == mControllerNr)
    {
        if ("P" == mPortType) // power supply
        {
            _appendDataToDescription(" - power supply ");
            _appendDataToLabel("_PS");

            shouldCheckNumberedPortType = true;
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidControllerNumberErrorText);
    }

    if (shouldCheckNumberedPortType)
    {
        if (Core::isDigitString(mPortNumber))
        {
            _appendDataToDescription(mPortNumber);
            _appendDataToLabel(mPortNumber);
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
        }
    }
    else if (shouldCheckManagementPort)
    {
        if ("m" == mPortNumber || "M" == mPortNumber)
        {
            _appendDataToDescription(" - management port");
            _appendDataToLabel("_MGMT");
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
        }
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

    _initializeDescriptionAndLabel("Blade system");
}

void BladeServerPort::computeDescriptionAndLabel()
{
    Core::convertStringCase(mModuleType, true);

    if ("DM" == mModuleType || "MG" == mModuleType || "P" == mModuleType)
    {
        _handleNumberedModuleType();
    }
    else if ("UP" == mModuleType) // management uplink port (for daisy chaining multiple blade systems)
    {
        _appendDataToDescription(" - management uplink port");
        _appendDataToLabel("_MG_UP");
    }
    else if ("DO" == mModuleType) // management downlink port (for daisy chaining multiple blade systems)
    {
        _appendDataToDescription(" - management downlink port");
        _appendDataToLabel("_MG_DO");
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidModuleTypeErrorText);
    }

    _checkLabel();
}

void BladeServerPort::_handleNumberedModuleType()
{
    if (Core::isDigitString(mModuleNumber))
    {
        if ("DM" == mModuleType) // data module
        {
            if (Core::isDigitString(mPortNumber))
            {
                _appendDataToDescription(" - data module " + mModuleNumber + " - port " + mPortNumber);
                _appendDataToLabel("_DMO" + mModuleNumber + "_P" + mPortNumber);
            }
            else
            {
                _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
            }
        }
        else if ("MG" == mModuleType) // management module
        {
            _appendDataToDescription(" - management module " + mModuleNumber);
            _appendDataToLabel("_MGMT" + mModuleNumber);
        }
        else if ("P" == mModuleType) // power supply
        {
            _appendDataToDescription(" - power supply " + mModuleNumber);
            _appendDataToLabel("_PS" + mModuleNumber);
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidModuleNumberErrorText);
    }
}
