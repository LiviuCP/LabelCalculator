#include "datadeviceport.h"
#include "powerdeviceport.h"
#include "deviceportsfactory.h"

DevicePortsFactory::DevicePortsFactory()
    : mCreatedDevicePortsCount{0}
{
}

DevicePortPtr DevicePortsFactory::createDevicePort(DeviceTypeID deviceTypeID, bool isSourceDevice)
{
    DevicePortPtr pDevicePort{nullptr};

    switch(deviceTypeID)
    {
    case DeviceTypeID::PDU:
        pDevicePort = std::make_shared<PDUPort>(isSourceDevice);
        break;
    case DeviceTypeID::EXTENSION_BAR:
        pDevicePort = std::make_shared<ExtensionBarPort>(isSourceDevice);
        break;
    case DeviceTypeID::UPS:
        pDevicePort = std::make_shared<UPSPort>(isSourceDevice);
        break;
    case DeviceTypeID::LAN_SWITCH:
        pDevicePort = std::make_shared<LANSwitchPort>(isSourceDevice);
        break;
    case DeviceTypeID::SAN_SWITCH:
        pDevicePort = std::make_shared<SANSwitchPort>(isSourceDevice);
        break;
    case DeviceTypeID::INFINIBAND_SWITCH:
        pDevicePort = std::make_shared<InfinibandSwitchPort>(isSourceDevice);
        break;
    case DeviceTypeID::KVM_SWITCH:
        pDevicePort = std::make_shared<KVMSwitchPort>(isSourceDevice);
        break;
    case DeviceTypeID::RACK_SERVER:
        pDevicePort = std::make_shared<ServerPort>(isSourceDevice);
        break;
    case DeviceTypeID::STORAGE:
        pDevicePort = std::make_shared<StoragePort>(isSourceDevice);
        break;
    case DeviceTypeID::BLADE_SERVER:
        pDevicePort = std::make_shared<BladeServerPort>(isSourceDevice);
        break;
    default:
        break;
    }

    if (nullptr != pDevicePort)
    {
        ++mCreatedDevicePortsCount;
    }

    return pDevicePort;
}

int DevicePortsFactory::getCreatedDevicePortsCount() const
{
    return mCreatedDevicePortsCount;
}
