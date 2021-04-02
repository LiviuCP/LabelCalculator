#include "datadevice.h"
#include "powerdevice.h"
#include "devicefactory.h"

bool DeviceFactory::isDeviceTypeValid(const std::string& deviceType)
{
    const bool c_IsValid{DeviceFactory::scDeviceTypes.find(deviceType) != scDeviceTypes.cend()};
    return c_IsValid;
}
const std::set<std::string> DeviceFactory::scDeviceTypes
{
    "_pdu",
    "_ext",
    "_ups",
    "_ps" ,
    "lan" ,
    "san" ,
    "ib"  ,
    "kvm" ,
    "svr" ,
    "sto" ,
    "bld"
};

DeviceFactory::DeviceFactory()
    : mCreatedDevicesCount{0}
{
}

DevicePtr DeviceFactory::createDevice(const std::string& deviceType, bool isSourceDevice)
{
    DevicePtr pDevice{nullptr};

    // naming convention: for power connections the underscore is used at the beginning of the device type
    if ("_pdu" == deviceType)
    {
        pDevice = std::make_shared<PDU>(isSourceDevice);
    }
    else if ("_ext" == deviceType)
    {
        pDevice = std::make_shared<ExtensionBar>(isSourceDevice);
    }
    else if ("_ups" == deviceType)
    {
        pDevice = std::make_shared<UPS>(isSourceDevice);
    }
    else if ("_ps" == deviceType)
    {
        pDevice = std::make_shared<PowerSupply>(isSourceDevice);
    }
    else if ("lan" == deviceType)
    {
        pDevice = std::make_shared<LANSwitch>(isSourceDevice);
    }
    else if ("san"== deviceType)
    {
        pDevice = std::make_shared<SANSwitch>(isSourceDevice);
    }
    else if ("ib" == deviceType)
    {
        pDevice = std::make_shared<InfinibandSwitch>(isSourceDevice);
    }
    else if ("kvm" == deviceType)
    {
        pDevice = std::make_shared<KVMSwitch>(isSourceDevice);
    }
    else if ("svr" == deviceType)
    {
        pDevice = std::make_shared<Server>(isSourceDevice);
    }
    else if ("sto" == deviceType)
    {
        pDevice = std::make_shared<Storage>(isSourceDevice);
    }
    else if ("bld" == deviceType)
    {
        pDevice = std::make_shared<BladeServer>(isSourceDevice);
    }
    else
    {
        // no action, defensive programming
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
