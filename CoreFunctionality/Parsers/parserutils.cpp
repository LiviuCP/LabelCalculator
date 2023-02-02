#include "coreutils.h"
#include "parserutils.h"

Data::DeviceTypeID Utilities::getDeviceTypeID(const std::string& deviceType)
{
    Data::DeviceTypeID deviceTypeID{Data::DeviceTypeID::UNKNOWN_DEVICE};

    std::string deviceTypeLowerCase{deviceType};
    convertStringCase(deviceTypeLowerCase, false);

    const auto it{Data::c_DeviceTypeTextToIDMapping.find(deviceTypeLowerCase)};

    if (it != Data::c_DeviceTypeTextToIDMapping.cend())
    {
        deviceTypeID = it->second;
    }

    return deviceTypeID;
}

std::string Utilities::getDeviceTypeAsString(Data::DeviceTypeID deviceTypeID)
{
    std::string deviceType;

    const auto it{std::find_if(Data::c_DeviceTypeTextToIDMapping.cbegin(),
                               Data::c_DeviceTypeTextToIDMapping.cend(),
                               [deviceTypeID](const std::pair<std::string, Data::DeviceTypeID>& element) {return deviceTypeID == element.second;})};

    if (Data::c_DeviceTypeTextToIDMapping.cend() != it)
    {
        deviceType = it->first;
    }

    return deviceType;
}
