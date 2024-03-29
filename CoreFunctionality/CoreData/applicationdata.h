#ifndef APPLICATIONDATA_H
#define APPLICATIONDATA_H

#include <map>
#include <string>
#include <string_view>

namespace Data
{
    using UNumber_t = size_t;

    inline constexpr char c_CSVSeparator{','};
    inline constexpr unsigned short c_FirstRackUPositionNumber{1u};
    inline constexpr unsigned short c_MaxRackUnitsCount{50u};

    enum class DeviceTypeID : int
    {
        UNKNOWN_DEVICE = -1, // invalid device type resulting from string conversion
        NO_DEVICE,           // no device placed STARTING WITH the given U position
        PDU,
        EXTENSION_BAR,
        UPS,
        LAN_SWITCH,
        LAN_DIRECTOR,
        SAN_SWITCH,
        SAN_DIRECTOR,
        INFINIBAND_SWITCH,
        KVM_SWITCH,
        RACK_SERVER,
        SCALABLE_SERVER,
        STORAGE,
        BLADE_SERVER
    };

    inline const std::map<std::string, DeviceTypeID> c_DeviceTypeTextToIDMapping
    {
        {    "pdu",     DeviceTypeID::PDU                   },
        {    "ext",     DeviceTypeID::EXTENSION_BAR         },
        {    "ups",     DeviceTypeID::UPS                   },
        {    "lan",     DeviceTypeID::LAN_SWITCH            },
        {    "lnd",     DeviceTypeID::LAN_DIRECTOR          },
        {    "san",     DeviceTypeID::SAN_SWITCH            },
        {    "snd",     DeviceTypeID::SAN_DIRECTOR          },
        {    "ib",      DeviceTypeID::INFINIBAND_SWITCH     },
        {    "kvm",     DeviceTypeID::KVM_SWITCH            },
        {    "svr",     DeviceTypeID::RACK_SERVER           },
        {    "sls",     DeviceTypeID::SCALABLE_SERVER       },
        {    "sto",     DeviceTypeID::STORAGE               },
        {    "bld",     DeviceTypeID::BLADE_SERVER          }
    };

    inline constexpr std::string_view c_ConnectionDefinitionsHeader {"U,Device,Connection1,Connection2,Connection3,Connection4,Connection5,Connection6,Connection7,Connection8"};

    inline constexpr std::string_view c_ConnectionInputHeader{"__Cable part number__,__Source device type__,__Source U number__,__Parameter1__,__Parameter2__,"
                                                              "__Parameter3__,__Destination device type__,__Destination U number__,__Parameter1__,__Parameter2__,__Parameter3__"};

    inline constexpr std::string_view c_LabellingTableHeader{"__Item number__,__Cable part number__,__Source device description__,"
                                                             "__Source label__,__Destination device description__,__Destination label__"};
}

#endif // APPLICATIONDATA_H
