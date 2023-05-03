#include "datadeviceport.h"
#include "powerdeviceport.h"
#include "deviceportsfactory.h"

DevicePort* DevicePortsFactory::createDevicePort(const Data::DeviceTypeID deviceTypeID,
                                                   const std::string_view deviceUPosition,
                                                   const size_t fileRowNumber,
                                                   const bool isSourceDevice)
{
    DevicePort* pDevicePort{nullptr};

    switch(deviceTypeID)
    {
    case Data::DeviceTypeID::PDU:
        pDevicePort = new PDUPort(deviceUPosition, fileRowNumber, isSourceDevice);
        break;
    case Data::DeviceTypeID::EXTENSION_BAR:
        pDevicePort = new ExtensionBarPort(deviceUPosition, fileRowNumber, isSourceDevice);
        break;
    case Data::DeviceTypeID::UPS:
        pDevicePort = new UPSPort(deviceUPosition, fileRowNumber, isSourceDevice);
        break;
    case Data::DeviceTypeID::LAN_SWITCH:
        pDevicePort = new LANSwitchPort(deviceUPosition, fileRowNumber, isSourceDevice);
        break;
    case Data::DeviceTypeID::LAN_DIRECTOR:
        pDevicePort = new LANDirectorPort(deviceUPosition, fileRowNumber, isSourceDevice);
        break;
    case Data::DeviceTypeID::SAN_SWITCH:
        pDevicePort = new SANSwitchPort(deviceUPosition, fileRowNumber, isSourceDevice);
        break;
    case Data::DeviceTypeID::SAN_DIRECTOR:
        pDevicePort = new SANDirectorPort(deviceUPosition, fileRowNumber, isSourceDevice);
        break;
    case Data::DeviceTypeID::INFINIBAND_SWITCH:
        pDevicePort = new InfinibandSwitchPort(deviceUPosition, fileRowNumber, isSourceDevice);
        break;
    case Data::DeviceTypeID::KVM_SWITCH:
        pDevicePort = new KVMSwitchPort(deviceUPosition, fileRowNumber, isSourceDevice);
        break;
    case Data::DeviceTypeID::RACK_SERVER:
        pDevicePort = new ServerPort(deviceUPosition, fileRowNumber, isSourceDevice);
        break;
    case Data::DeviceTypeID::SCALABLE_SERVER:
        pDevicePort = new ScalableServerPort(deviceUPosition, fileRowNumber, isSourceDevice);
        break;
    case Data::DeviceTypeID::STORAGE:
        pDevicePort = new StoragePort(deviceUPosition, fileRowNumber, isSourceDevice);
        break;
    case Data::DeviceTypeID::BLADE_SERVER:
        pDevicePort = new BladeServerPort(deviceUPosition, fileRowNumber, isSourceDevice);
        break;
    default:
        ASSERT(false, "");
    }

    return pDevicePort;
}
