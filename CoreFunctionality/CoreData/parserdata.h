#ifndef PARSERDATA_H
#define PARSERDATA_H

#include "applicationdata.h"

namespace Data
{

    /* Used by error handling mechanism
    */

    inline constexpr size_t c_NrOfCablesPerConnectionInputRow{1u};
    inline constexpr size_t c_NrOfPortsPerConnectionInputRow{2u};
    inline constexpr size_t c_NrOfDeviceTypeAndUPositionCellsPerPort{2u};

    /* placeholders used for creating the connection input template file (should be filled in by user in next step)
    */

    inline constexpr std::string_view c_CablePartNumberPlaceholder{"CBL_PART_NR"};

    inline const std::map<DeviceTypeID, std::string> c_ConnectionInputPlaceholders
    {
        {   DeviceTypeID::PDU,                     "PLACEMENT,LOAD SEGMENT NUMBER,PORT NUMBER"     },
        {   DeviceTypeID::EXTENSION_BAR,           "PLACEMENT,PORT NUMBER,-"                       },
        {   DeviceTypeID::UPS,                     "LOAD SEGMENT NUMBER,PORT NUMBER,-"             },
        {   DeviceTypeID::LAN_SWITCH,              "PORT TYPE,PORT NUMBER,-"                       },
        {   DeviceTypeID::LAN_DIRECTOR,            "BLADE NUMBER,PORT TYPE,PORT NUMBER"            },
        {   DeviceTypeID::SAN_SWITCH,              "PORT TYPE,PORT NUMBER,-"                       },
        {   DeviceTypeID::SAN_DIRECTOR,            "BLADE NUMBER,PORT TYPE,PORT NUMBER"            },
        {   DeviceTypeID::INFINIBAND_SWITCH,       "PORT TYPE,PORT NUMBER,-"                       },
        {   DeviceTypeID::KVM_SWITCH,              "PORT TYPE,PORT NUMBER,-"                       },
        {   DeviceTypeID::RACK_SERVER,             "PORT TYPE,PORT NUMBER,-"                       },
        {   DeviceTypeID::SCALABLE_SERVER,         "SLOT NUMBER,PORT TYPE,PORT NUMBER"             },
        {   DeviceTypeID::STORAGE,                 "CONTROLLER NUMBER,PORT TYPE,PORT NUMBER"       },
        {   DeviceTypeID::BLADE_SERVER,            "MODULE TYPE,MODULE NUMBER,PORT NUMBER"         }
    };
}

#endif // PARSERDATA_H
