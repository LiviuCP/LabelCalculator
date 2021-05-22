#include "datadeviceport.h"
#include "powerdeviceport.h"
#include "deviceportsfactory.h"

DevicePortsFactory::DevicePortsFactory()
    : mCreatedDevicePortsCount{0u}
{
}

void DevicePortsFactory::reset()
{
    mCreatedDevicePortsCount = 0u;
}

DevicePortPtr DevicePortsFactory::createDevicePort(const DeviceTypeID deviceTypeID, const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice)
{
    DevicePortPtr pDevicePort{nullptr};

    switch(deviceTypeID)
    {
    case DeviceTypeID::PDU:
        pDevicePort = std::make_shared<PDUPort>(deviceUPosition, fileRowNumber, fileColumnNumber, isSourceDevice);
        break;
    case DeviceTypeID::EXTENSION_BAR:
        pDevicePort = std::make_shared<ExtensionBarPort>(deviceUPosition, fileRowNumber, fileColumnNumber, isSourceDevice);
        break;
    case DeviceTypeID::UPS:
        pDevicePort = std::make_shared<UPSPort>(deviceUPosition, fileRowNumber, fileColumnNumber, isSourceDevice);
        break;
    case DeviceTypeID::LAN_SWITCH:
        pDevicePort = std::make_shared<LANSwitchPort>(deviceUPosition, fileRowNumber, fileColumnNumber, isSourceDevice);
        break;
    case DeviceTypeID::SAN_SWITCH:
        pDevicePort = std::make_shared<SANSwitchPort>(deviceUPosition, fileRowNumber, fileColumnNumber, isSourceDevice);
        break;
    case DeviceTypeID::INFINIBAND_SWITCH:
        pDevicePort = std::make_shared<InfinibandSwitchPort>(deviceUPosition, fileRowNumber, fileColumnNumber, isSourceDevice);
        break;
    case DeviceTypeID::KVM_SWITCH:
        pDevicePort = std::make_shared<KVMSwitchPort>(deviceUPosition, fileRowNumber, fileColumnNumber, isSourceDevice);
        break;
    case DeviceTypeID::RACK_SERVER:
        pDevicePort = std::make_shared<ServerPort>(deviceUPosition, fileRowNumber, fileColumnNumber, isSourceDevice);
        break;
    case DeviceTypeID::STORAGE:
        pDevicePort = std::make_shared<StoragePort>(deviceUPosition, fileRowNumber, fileColumnNumber, isSourceDevice);
        break;
    case DeviceTypeID::BLADE_SERVER:
        pDevicePort = std::make_shared<BladeServerPort>(deviceUPosition, fileRowNumber, fileColumnNumber, isSourceDevice);
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

size_t DevicePortsFactory::getCreatedDevicePortsCount() const
{
    return mCreatedDevicePortsCount;
}
