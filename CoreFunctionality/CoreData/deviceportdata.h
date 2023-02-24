#ifndef DEVICEPORTDATA_H
#define DEVICEPORTDATA_H

#include <map>
#include <string>
#include <cstdlib>

#include "deviceportdatatypes.h"
#include "applicationdata.h"

namespace Data
{
    // number of fields that should be filled in connectioninput.csv for EACH device (unused fields can be filled in with '-')
    inline constexpr size_t c_MaxPortInputParametersCount{3};

    // placement types for specific power devices (PDUs, extension bars)
    inline const std::map<std::string, std::string> c_DevicePlacementIdentifiers
    {
        {"H", "Horizontal"},    // horizontal PDU
        {"L", "Left"},          // vertically left placed PDU or extension bar
        {"R", "Right"}          // vertically right placed PDU or extension bar
    };

    inline const std::map<DeviceTypeID, SwitchPortData_t> c_SwitchPortData
    {
        {   DeviceTypeID::LAN_SWITCH,          {"N", "Ethernet", false}     },
        {   DeviceTypeID::SAN_SWITCH,          {"F", "FC",       true }     },
        {   DeviceTypeID::INFINIBAND_SWITCH,   {"I", "",         true }     },
        {   DeviceTypeID::KVM_SWITCH,          {"K", "",         false}     }
    };
}

#endif // DEVICEPORTDATA_H
