#include <cassert>
#include <set>

#include "applicationdata.h"
#include "deviceportdata.h"
#include "coreutils.h"
#include "deviceportutils.h"
#include "datadeviceport.h"

namespace Core = Utilities::Core;
namespace Ports = Utilities::DevicePorts;

SwitchPort::SwitchPort(const std::string& deviceUPosition, const DevicePortTypesInfo_t& switchPortTypesInfo, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
    , mAllowedDataPortTypes{switchPortTypesInfo.mAllowedDataPortTypes}
    , mHasManagementPort{switchPortTypesInfo.mHasManagementPort}
{
    // required parameters to be registered in the derived classes to make their ordering in the input csv file more flexible
}

void SwitchPort::_registerRequiredParameters()
{
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

void SwitchPort::updateDescriptionAndLabel()
{
    Core::convertStringCase(mPortType, true);

    if (mAllowedDataPortTypes.cend() != mAllowedDataPortTypes.find(mPortType) || "P" == mPortType)
    {
        _handleNumberedPortType();
    }
    else if ("-" == mPortType)
    {
        if (("m" == mPortNumber || "M" == mPortNumber) && mHasManagementPort) // management port
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
    if (Core::isDigitString(mPortNumber)) // power or data port
    {
        if (auto dataPortTypeIt{mAllowedDataPortTypes.find(mPortType)}; mAllowedDataPortTypes.cend() != dataPortTypeIt)
        {
            const std::string c_DataPortTypeDescription{dataPortTypeIt->second.first};
            const std::string c_DataPortTypeLabel{dataPortTypeIt->second.second};
            const std::string c_TrailingSpace{c_DataPortTypeDescription.size() > 0 ? " " : ""};
            const std::string c_PrecedingUnderscore{c_DataPortTypeLabel.size() > 0 ? "_" : ""};

            _appendDataToDescription(" - " + c_DataPortTypeDescription + c_TrailingSpace + "port " + mPortNumber);
            _appendDataToLabel(c_PrecedingUnderscore + c_DataPortTypeLabel + "_P" + mPortNumber);
        }
        else if ("P" == mPortType)
        {
            _appendDataToDescription(" - power supply " + mPortNumber);
            _appendDataToLabel("_PS" + mPortNumber);
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

LANSwitchPort::LANSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
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

SANSwitchPort::SANSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
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

InfinibandSwitchPort::InfinibandSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
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

KVMSwitchPort::KVMSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
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

ServerPort::ServerPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

void ServerPort::updateDescriptionAndLabel()
{
    Core::convertStringCase(mPortType, true);

    if (Data::c_ServerDataPortTypes.find(mPortType) != Data::c_ServerDataPortTypes.cend() || "P" == mPortType)
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

void ServerPort::_registerRequiredParameters()
{
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
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
    if (Core::isDigitString(mPortNumber))
    {
        if (auto dataPortTypeIt{Data::c_ServerDataPortTypes.find(mPortType)}; Data::c_ServerDataPortTypes.cend() != dataPortTypeIt)
        {
            const std::string c_DataPortTypeDescription{dataPortTypeIt->second.first};
            const std::string c_DataPortTypeLabel{dataPortTypeIt->second.second};

            if (c_DataPortTypeDescription.size() > 0 && c_DataPortTypeLabel.size() > 0)
            {
                _appendDataToDescription(" - " + c_DataPortTypeDescription + " port " + mPortNumber);
                _appendDataToLabel("_" + c_DataPortTypeLabel + "_P" + mPortNumber);
            }
            else
            {
                _setInvalidDescriptionAndLabel(Ports::c_InvalidPortTypeErrorText);
                assert(false);
            }
        }
        else if ("P" == mPortType)
        {
            _appendDataToDescription(" - power supply " + mPortNumber);
            _appendDataToLabel("_PS" + mPortNumber);
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

ScalableServerPort::ScalableServerPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
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

StoragePort::StoragePort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
    : DevicePort{deviceUPosition,
                 fileRowNumber,
                 fileColumnNumber,
                 isSourceDevice}
{
}

void StoragePort::updateDescriptionAndLabel()
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

void StoragePort::_registerRequiredParameters()
{
    _registerRequiredParameter(&mControllerNr);
    _registerRequiredParameter(&mPortType);
    _registerRequiredParameter(&mPortNumber);
}

size_t StoragePort::_getInputParametersCount() const
{
    return Data::c_RequiredInputParamsCount.at(Data::DeviceTypeID::STORAGE);
}

std::pair<std::string, std::string> StoragePort::_getDeviceTypeDescriptionAndLabel() const
{
    return Data::c_DeviceTypeDescriptionsAndLabels.at(Data::DeviceTypeID::STORAGE);
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
    _registerRequiredParameter(&mPortNumber);
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
