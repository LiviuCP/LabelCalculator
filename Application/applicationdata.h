#ifndef APPLICATIONDATA_H
#define APPLICATIONDATA_H

#include <map>

using UNumber_t = size_t;

static constexpr char c_CSVSeparator{','};
static constexpr unsigned short c_MaxNrOfRackUnits{50u};

enum class DeviceTypeID : int
{
    UNKNOWN_DEVICE = -1, // invalid device type resulting from string conversion
    NO_DEVICE,           // no device placed STARTING WITH the given U position
    PDU,
    EXTENSION_BAR,
    UPS,
    LAN_SWITCH,
    SAN_SWITCH,
    INFINIBAND_SWITCH,
    KVM_SWITCH,
    RACK_SERVER,
    STORAGE,
    BLADE_SERVER
};

static const std::map<std::string, DeviceTypeID> c_DeviceTypeTextToIDMapping
{
    {    "pdu",     DeviceTypeID::PDU                   },
    {    "ext",     DeviceTypeID::EXTENSION_BAR         },
    {    "ups",     DeviceTypeID::UPS                   },
    {    "lan",     DeviceTypeID::LAN_SWITCH            },
    {    "san",     DeviceTypeID::SAN_SWITCH            },
    {    "ib",      DeviceTypeID::INFINIBAND_SWITCH     },
    {    "kvm",     DeviceTypeID::KVM_SWITCH            },
    {    "svr",     DeviceTypeID::RACK_SERVER           },
    {    "sto",     DeviceTypeID::STORAGE               },
    {    "bld",     DeviceTypeID::BLADE_SERVER          }
};

static const std::map<DeviceTypeID, size_t> c_RequiredNrOfInputParams
{
    {    DeviceTypeID::PDU,                 3    },
    {    DeviceTypeID::EXTENSION_BAR,       2    },
    {    DeviceTypeID::UPS,                 2    },
    {    DeviceTypeID::LAN_SWITCH,          2    },
    {    DeviceTypeID::SAN_SWITCH,          2    },
    {    DeviceTypeID::INFINIBAND_SWITCH,   2    },
    {    DeviceTypeID::KVM_SWITCH,          2    },
    {    DeviceTypeID::RACK_SERVER,         2    },
    {    DeviceTypeID::STORAGE,             3    },
    {    DeviceTypeID::BLADE_SERVER,        3    }
};

static const std::map<DeviceTypeID, size_t> c_MaxAllowedInputCharsCount
{
    {    DeviceTypeID::PDU,                 6    },
    {    DeviceTypeID::EXTENSION_BAR,       8    },
    {    DeviceTypeID::UPS,                11    },
    {    DeviceTypeID::LAN_SWITCH,         13    },
    {    DeviceTypeID::SAN_SWITCH,         10    },
    {    DeviceTypeID::INFINIBAND_SWITCH,  10    },
    {    DeviceTypeID::KVM_SWITCH,         13    },
    {    DeviceTypeID::RACK_SERVER,         8    },
    {    DeviceTypeID::STORAGE,             7    },
    {    DeviceTypeID::BLADE_SERVER,        7    }
};

// placeholders used for creating the connection input template file (should be filled in by user in next step)
const std::map<DeviceTypeID, std::string> scConnectionInputPlaceholders
{
    {   DeviceTypeID::PDU,                     "PLACEMENT,LOAD SEGMENT NUMBER,PORT NUMBER"     },
    {   DeviceTypeID::EXTENSION_BAR,           "PLACEMENT,PORT NUMBER,-"                       },
    {   DeviceTypeID::UPS,                     "LOAD SEGMENT NUMBER,PORT NUMBER,-"             },
    {   DeviceTypeID::RACK_SERVER,             "PORT TYPE,PORT NUMBER,-"                       },
    {   DeviceTypeID::BLADE_SERVER,            "MODULE TYPE,MODULE NUMBER,PORT NUMBER"         },
    {   DeviceTypeID::STORAGE,                 "CONTROLLER NUMBER,PORT TYPE,PORT NUMBER"       },
    {   DeviceTypeID::SAN_SWITCH,              "PORT TYPE,PORT NUMBER,-"                       },
    {   DeviceTypeID::LAN_SWITCH,              "PORT TYPE,PORT NUMBER,-"                       },
    {   DeviceTypeID::INFINIBAND_SWITCH,       "PORT TYPE,PORT NUMBER,-"                       },
    {   DeviceTypeID::KVM_SWITCH,              "PORT TYPE,PORT NUMBER,-"                       }
};
#endif // APPLICATIONDATA_H
