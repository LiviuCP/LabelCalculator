#include "coreutils.h"
#include "parserutils.h"

Data::DeviceTypeID Utilities::getDeviceTypeID(const std::string& deviceType)
{
    Data::DeviceTypeID deviceTypeID{Data::DeviceTypeID::UNKNOWN_DEVICE};

    std::string deviceTypeLowerCase{deviceType};
    convertStringCase(deviceTypeLowerCase, false);

    std::map<std::string, Data::DeviceTypeID>::const_iterator it{Data::c_DeviceTypeTextToIDMapping.find(deviceTypeLowerCase)};

    if (it != Data::c_DeviceTypeTextToIDMapping.cend())
    {
        deviceTypeID = it->second;
    }

    return deviceTypeID;
}

std::string Utilities::getDeviceTypeAsString(Data::DeviceTypeID deviceTypeID)
{
    std::string deviceType;

    for (std::map<std::string, Data::DeviceTypeID>::const_iterator it{Data::c_DeviceTypeTextToIDMapping.cbegin()}; it != Data::c_DeviceTypeTextToIDMapping.cend(); ++it)
    {
        if (deviceTypeID == it->second)
        {
            deviceType = it->first;
            break;
        }
    }

    return deviceType;
}
