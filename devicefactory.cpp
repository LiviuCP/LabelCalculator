#include "device.h"
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

Device* DeviceFactory::createDevice(const std::string& deviceType, bool isSourceDevice)
{
    Device* device{nullptr};

    // naming convention: for power connections the underscore is used at the beginning of the device type
    if ("_pdu" == deviceType)
    {
        device = new PDU{isSourceDevice};
    }
    else if ("_ext" == deviceType)
    {
        device = new ExtensionBar{isSourceDevice};
    }
    else if ("_ups" == deviceType)
    {
        device = new UPS{isSourceDevice};
    }
    else if ("_ps" == deviceType)
    {
        device = new PowerSupply{isSourceDevice};
    }
    else if ("lan" == deviceType)
    {
        device = new LANSwitch{isSourceDevice};
    }
    else if ("san"== deviceType)
    {
        device = new SANSwitch{isSourceDevice};
    }
    else if ("ib" == deviceType)
    {
        device = new InfinibandSwitch{isSourceDevice};
    }
    else if ("kvm" == deviceType)
    {
        device = new KVMSwitch{isSourceDevice};
    }
    else if ("svr" == deviceType)
    {
        device = new Server{isSourceDevice};
    }
    else if ("sto" == deviceType)
    {
        device = new Storage{isSourceDevice};
    }
    else if ("bld" == deviceType)
    {
        device = new BladeServer{isSourceDevice};
    }
    else
    {
        // no action, defensive programming
    }

    if (nullptr != device)
    {
        ++mCreatedDevicesCount;
    }

    return device;
}

int DeviceFactory::getCreatedDevicesCount() const
{
    return mCreatedDevicesCount;
}
