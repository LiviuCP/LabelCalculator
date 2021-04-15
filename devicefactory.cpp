#include "datadevice.h"
#include "powerdevice.h"
#include "devicefactory.h"

DeviceFactory::DeviceFactory()
    : mCreatedDevicesCount{0}
{
}

DevicePtr DeviceFactory::createDevice(DeviceTypeID deviceTypeID, bool isSourceDevice)
{
    DevicePtr pDevice{nullptr};

    switch(deviceTypeID)
    {
    case DeviceTypeID::PDU:
        pDevice = std::make_shared<PDU>(isSourceDevice);
        break;
    case DeviceTypeID::EXTENSION_BAR:
        pDevice = std::make_shared<ExtensionBar>(isSourceDevice);
        break;
    case DeviceTypeID::UPS:
        pDevice = std::make_shared<UPS>(isSourceDevice);
        break;
    case DeviceTypeID::POWER_SUPPLY:
        pDevice = std::make_shared<PowerSupply>(isSourceDevice);
        break;
    case DeviceTypeID::LAN_SWITCH:
        pDevice = std::make_shared<LANSwitch>(isSourceDevice);
        break;
    case DeviceTypeID::SAN_SWITCH:
        pDevice = std::make_shared<SANSwitch>(isSourceDevice);
        break;
    case DeviceTypeID::INFINIBAND_SWITCH:
        pDevice = std::make_shared<InfinibandSwitch>(isSourceDevice);
        break;
    case DeviceTypeID::KVM_SWITCH:
        pDevice = std::make_shared<KVMSwitch>(isSourceDevice);
        break;
    case DeviceTypeID::RACK_SERVER:
        pDevice = std::make_shared<Server>(isSourceDevice);
        break;
    case DeviceTypeID::STORAGE:
        pDevice = std::make_shared<Storage>(isSourceDevice);
        break;
    case DeviceTypeID::BLADE_SERVER:
        pDevice = std::make_shared<BladeServer>(isSourceDevice);
        break;
    default:
        break;
    }

    if (nullptr != pDevice)
    {
        ++mCreatedDevicesCount;
    }

    return pDevice;
}

int DeviceFactory::getCreatedDevicesCount() const
{
    return mCreatedDevicesCount;
}
