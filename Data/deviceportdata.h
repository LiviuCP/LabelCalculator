#ifndef DEVICEPORTDATA_H
#define DEVICEPORTDATA_H

#include <map>
#include <string>
#include <cstdlib>

namespace Data
{
    // number of fields that should be filled in connectioninput.csv for EACH device (unused fields can be filled in with '-')
    static constexpr size_t c_MaxPortInputParametersCount{3};

    // placement types for specific power devices (PDUs, extension bars)
    const std::map<std::string, std::string> c_DevicePlacementIdentifiers
    {
        {"H", "Horizontal"},    // horizontal PDU
        {"L", "Left"},          // vertically left placed PDU or extension bar
        {"R", "Right"}          // vertically right placed PDU or extension bar
    };


}

#endif // DEVICEPORTDATA_H
