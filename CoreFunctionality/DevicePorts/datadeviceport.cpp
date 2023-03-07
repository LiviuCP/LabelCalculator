#include <cassert>

#include "deviceportdata.h"
#include "deviceportutils.h"
#include "datadeviceport.h"

namespace Core = Utilities::Core;
namespace Ports = Utilities::DevicePorts;

SwitchPort::SwitchPort(const std::string_view deviceUPosition, const DevicePortTypesInfo_t& switchPortTypesInfo, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
    , mAllowedDataPortTypes{switchPortTypesInfo.mAllowedDataPortTypes}
    , mIsManagementPortAllowed{switchPortTypesInfo.mIsManagementPortAllowed}
{
    // required parameters to be registered in the derived classes to make their ordering in the input csv file more flexible
}

void SwitchPort::_registerRequiredParameters()
{
    _registerRequiredParameter(&mPortType);
    _registerPortNumber();
}

std::string SwitchPort::_getPortType() const
{
    return mPortType;
}

void SwitchPort::updateDescriptionAndLabel()
{
    Core::convertStringCase(mPortType, true);

    if (mAllowedDataPortTypes.cend() != mAllowedDataPortTypes.find(mPortType) || Ports::isPowerPortType(mPortType))
    {
        _handleNumberedPortType();
    }
    else if ("-" == mPortType)
    {
        if (Ports::isManagementPortNumber(_getPortNumber()) && mIsManagementPortAllowed) // management port
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

void SwitchPort::_handleNumberedPortType()
{
    if (const std::string c_PortNumber{_getPortNumber()}; Core::isDigitString(c_PortNumber)) // power or data port
    {
        if (auto dataPortTypeIt{mAllowedDataPortTypes.find(mPortType)}; mAllowedDataPortTypes.cend() != dataPortTypeIt)
        {
            const std::string c_DataPortTypeDescription{dataPortTypeIt->second.first};
            const std::string c_DataPortTypeLabel{dataPortTypeIt->second.second};
            const std::string c_TrailingSpace{c_DataPortTypeDescription.size() > 0 ? " " : ""};
            const std::string c_PrecedingUnderscore{c_DataPortTypeLabel.size() > 0 ? "_" : ""};

            _appendDataToDescription(" - " + c_DataPortTypeDescription + c_TrailingSpace + "port " + c_PortNumber);
            _appendDataToLabel(c_PrecedingUnderscore + c_DataPortTypeLabel + "_P" + c_PortNumber);
        }
        else if (Ports::isPowerPortType(mPortType))
        {
            _appendDataToDescription(" - power supply " + c_PortNumber);
            _appendDataToLabel("_PS" + c_PortNumber);
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
            assert(false);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
    }
}

DirectorPort::DirectorPort(const std::string_view deviceUPosition, const AllowedDataPortTypes_t& directorDataPortTypesInfo, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : SwitchPort{deviceUPosition,
                 {directorDataPortTypesInfo, true},
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

void DirectorPort::updateDescriptionAndLabel()
{
    const std::string c_PortType{_getPortType()};
    const bool c_IsDataPortType{!Ports::isPowerPortType(c_PortType) && "-" != c_PortType};

    if (Core::isDigitString(mBladeNumber))
    {
        if (c_IsDataPortType)
        {
            _appendDataToDescription(" - blade " + mBladeNumber);
            _appendDataToLabel("_B" + mBladeNumber);
            SwitchPort::updateDescriptionAndLabel();
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
        }
    }
    else if ("-" == mBladeNumber)
    {
        if (!c_IsDataPortType)
        {
            SwitchPort::updateDescriptionAndLabel();
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidBladeNumberErrorText);
    }
}

void DirectorPort::_registerRequiredParameters()
{
    _registerRequiredParameter(&mBladeNumber);
    SwitchPort::_registerRequiredParameters();
}

LANSwitchPort::LANSwitchPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : SwitchPort{deviceUPosition,
                 Data::c_SwitchPortTypesInfoMap.at(Data::DeviceTypeID::LAN_SWITCH),
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

size_t LANSwitchPort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::LAN_SWITCH);
}

std::pair<std::string, std::string> LANSwitchPort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::LAN_SWITCH);
}

LANDirectorPort::LANDirectorPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DirectorPort{deviceUPosition,
                   Data::c_DirectorDataPortTypesInfoMap.at(Data::DeviceTypeID::LAN_DIRECTOR),
                   fileRowNumber,
                   fileColumnNumber,
                   isSourceDevice}
{
}

size_t LANDirectorPort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::LAN_DIRECTOR);
}

std::pair<std::string, std::string> LANDirectorPort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::LAN_DIRECTOR);
}

SANSwitchPort::SANSwitchPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : SwitchPort{deviceUPosition,
                 Data::c_SwitchPortTypesInfoMap.at(Data::DeviceTypeID::SAN_SWITCH),
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

size_t SANSwitchPort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::SAN_SWITCH);
}

std::pair<std::string, std::string> SANSwitchPort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::SAN_SWITCH);
}

SANDirectorPort::SANDirectorPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DirectorPort{deviceUPosition,
                 Data::c_DirectorDataPortTypesInfoMap.at(Data::DeviceTypeID::SAN_DIRECTOR),
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

size_t SANDirectorPort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::SAN_DIRECTOR);
}

std::pair<std::string, std::string> SANDirectorPort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::SAN_DIRECTOR);
}

InfinibandSwitchPort::InfinibandSwitchPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : SwitchPort{deviceUPosition,
                 Data::c_SwitchPortTypesInfoMap.at(Data::DeviceTypeID::INFINIBAND_SWITCH),
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

size_t InfinibandSwitchPort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::INFINIBAND_SWITCH);
}

std::pair<std::string, std::string> InfinibandSwitchPort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::INFINIBAND_SWITCH);
}

KVMSwitchPort::KVMSwitchPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : SwitchPort{deviceUPosition,
                 Data::c_SwitchPortTypesInfoMap.at(Data::DeviceTypeID::KVM_SWITCH),
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

size_t KVMSwitchPort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::KVM_SWITCH);
}

std::pair<std::string, std::string> KVMSwitchPort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::KVM_SWITCH);
}

ServerPort::ServerPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

void ServerPort::updateDescriptionAndLabel()
{
    Core::convertStringCase(mPortType, true);

    if (Data::c_ServerDataPortTypes.find(mPortType) != Data::c_ServerDataPortTypes.cend() || Ports::isPowerPortType(mPortType))
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
        if(Ports::isManagementPortNumber(_getPortNumber())) // management port
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

void ServerPort::_registerRequiredParameters()
{
    _registerRequiredParameter(&mPortType);
    _registerPortNumber();
}

size_t ServerPort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::RACK_SERVER);
}

std::pair<std::string, std::string> ServerPort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::RACK_SERVER);
}

void ServerPort::_handleNumberedPortType()
{
    if (const std::string c_PortNumber{_getPortNumber()}; Core::isDigitString(c_PortNumber))
    {
        if (auto dataPortTypeIt{Data::c_ServerDataPortTypes.find(mPortType)}; Data::c_ServerDataPortTypes.cend() != dataPortTypeIt)
        {
            const std::string c_DataPortTypeDescription{dataPortTypeIt->second.first};
            const std::string c_DataPortTypeLabel{dataPortTypeIt->second.second};

            if (c_DataPortTypeDescription.size() > 0 && c_DataPortTypeLabel.size() > 0)
            {
                _appendDataToDescription(" - " + c_DataPortTypeDescription + " port " + c_PortNumber);
                _appendDataToLabel("_" + c_DataPortTypeLabel + "_P" + c_PortNumber);
            }
            else
            {
                _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
                assert(false);
            }
        }
        else if (Ports::isPowerPortType(mPortType))
        {
            _appendDataToDescription(" - power supply " + c_PortNumber);
            _appendDataToLabel("_PS" + c_PortNumber);
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
            assert(false);
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
    }
}

ScalableServerPort::ScalableServerPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : ServerPort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

void ScalableServerPort::updateDescriptionAndLabel()
{
    if (Core::isDigitString(mSlotNumber))
    {
        _appendDataToDescription(" - slot " + mSlotNumber);
        _appendDataToLabel("_S" + mSlotNumber);

        ServerPort::updateDescriptionAndLabel();
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidSlotNumberErrorText);
    }
}

void ScalableServerPort::_registerRequiredParameters()
{
    // slot number comes in addition to the input params of the Server class (and precedes them in the csv input file)
    _registerRequiredParameter(&mSlotNumber);

    ServerPort::_registerRequiredParameters();
}

size_t ScalableServerPort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::SCALABLE_SERVER);
}

std::pair<std::string, std::string> ScalableServerPort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::SCALABLE_SERVER);
}

StoragePort::StoragePort(const std::string_view deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

void StoragePort::updateDescriptionAndLabel()
{
    Core::convertStringCase(mPortType, true);

    if (Core::isDigitString(mControllerNr))
    {
        if ("D" == mPortType) // data port (e.g. FC)
        {
            _appendDataToDescription(" - controller " + mControllerNr + " - port ");
            _appendDataToLabel("_C" + mControllerNr + "_P");
            _handleNumberedPortType();
        }
        else if ("-" == mPortType)
        {
            _appendDataToDescription(" - controller " + mControllerNr);
            _appendDataToLabel("_C" + mControllerNr);
            _handleManagementPort(); // one management port per controller
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
            _handleManagementPort(); // unified management port
        }
        else
        {
            _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
        }
    }
    else if ("-" == mControllerNr)
    {
        if (Ports::isPowerPortType(mPortType)) // power supply
        {
            _appendDataToDescription(" - power supply ");
            _appendDataToLabel("_PS");
            _handleNumberedPortType();
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

    _checkLabel();
}

void StoragePort::_registerRequiredParameters()
{
    _registerRequiredParameter(&mControllerNr);
    _registerRequiredParameter(&mPortType);
    _registerPortNumber();
}

size_t StoragePort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::STORAGE);
}

std::pair<std::string, std::string> StoragePort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::STORAGE);
}

void StoragePort::_handleNumberedPortType()
{
    if (const std::string c_PortNumber{_getPortNumber()}; Core::isDigitString(c_PortNumber))
    {
        _appendDataToDescription(c_PortNumber);
        _appendDataToLabel(c_PortNumber);
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
    }
}

void StoragePort::_handleManagementPort()
{
    if (Ports::isManagementPortNumber(_getPortNumber()))
    {
        _appendDataToDescription(" - management port");
        _appendDataToLabel("_MGMT");
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_InvalidPortNumberErrorText);
    }
}

/* In order to avoid registering an additional parameter that might cause some confusion (and potential issues),
   for blades the power supply is considered a module, not a port:
    - instead of mPortType: mModuleType is used for power supply
    - instead of port number: mModuleNumber is used for power supply number
*/
BladeServerPort::BladeServerPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

void BladeServerPort::updateDescriptionAndLabel()
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

void BladeServerPort::_registerRequiredParameters()
{
    _registerRequiredParameter(&mModuleType);
    _registerRequiredParameter(&mModuleNumber);
    _registerPortNumber();
}

size_t BladeServerPort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::BLADE_SERVER);
}

std::pair<std::string, std::string> BladeServerPort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::BLADE_SERVER);
}

void BladeServerPort::_handleNumberedModuleType()
{
    if (Core::isDigitString(mModuleNumber))
    {
        if ("DM" == mModuleType) // data module
        {
            if (const std::string c_PortNumber{_getPortNumber()}; Core::isDigitString(c_PortNumber))
            {
                _appendDataToDescription(" - data module " + mModuleNumber + " - port " + c_PortNumber);
                _appendDataToLabel("_DMO" + mModuleNumber + "_P" + c_PortNumber);
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
