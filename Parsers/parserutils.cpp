#include "applicationutils.h"
#include "parserutils.h"

DeviceTypeID getDeviceTypeID(const std::string& deviceType)
{
    DeviceTypeID deviceTypeID{DeviceTypeID::UNKNOWN_DEVICE};

    std::string deviceTypeLowerCase{deviceType};
    convertStringCase(deviceTypeLowerCase, false);

    std::map<std::string, DeviceTypeID>::const_iterator it{c_DeviceTypeTextToIDMapping.find(deviceTypeLowerCase)};

    if (it != c_DeviceTypeTextToIDMapping.cend())
    {
        deviceTypeID = it->second;
    }

    return deviceTypeID;
}

std::string getDeviceTypeAsString(DeviceTypeID deviceTypeID)
{
    std::string deviceType;

    for (std::map<std::string, DeviceTypeID>::const_iterator it{c_DeviceTypeTextToIDMapping.cbegin()}; it != c_DeviceTypeTextToIDMapping.cend(); ++it)
    {
        if (deviceTypeID == it->second)
        {
            deviceType = it->first;
            break;
        }
    }

    return deviceType;
}

