#ifndef DEVICEPORTDATA_H
#define DEVICEPORTDATA_H

#include <cstdlib>

#include "deviceporttypes.h"
#include "applicationdata.h"

namespace Data
{
    // maximum number of characters allowed on a device port label
    inline constexpr unsigned short c_MaxLabelCharsCount{16u};

    // number of fields that should be filled in connectioninput.csv for EACH device (unused fields can be filled in with '-')
    inline constexpr size_t c_MaxPortInputParametersCount{3};

    // padding data that is prepended to raw input data in case device port doesn't start parsing from beginning of row
    inline constexpr std::string_view c_Padding{"--PREVIOUS_CELLS--"};

    inline const std::map<DeviceTypeID, size_t> c_RequiredInputParamsCount
    {
        {    DeviceTypeID::PDU,                 3    },
        {    DeviceTypeID::EXTENSION_BAR,       2    },
        {    DeviceTypeID::UPS,                 2    },
        {    DeviceTypeID::LAN_SWITCH,          2    },
        {    DeviceTypeID::LAN_DIRECTOR,        3    },
        {    DeviceTypeID::SAN_SWITCH,          2    },
        {    DeviceTypeID::SAN_DIRECTOR,        3    },
        {    DeviceTypeID::INFINIBAND_SWITCH,   2    },
        {    DeviceTypeID::KVM_SWITCH,          2    },
        {    DeviceTypeID::RACK_SERVER,         2    },
        {    DeviceTypeID::SCALABLE_SERVER,     3    },
        {    DeviceTypeID::STORAGE,             3    },
        {    DeviceTypeID::BLADE_SERVER,        3    }
    };

    // placement types for specific power devices (PDUs, extension bars)
    inline const std::map<std::string, std::string> c_DevicePlacementIdentifiers
    {
        {"H", "horizontal"},    // horizontal PDU
        {"L", "left"},          // vertically left placed PDU or extension bar
        {"R", "right"}          // vertically right placed PDU or extension bar
    };

    inline const std::map<DeviceTypeID, std::pair<std::string, std::string>> c_DeviceTypeDescriptionsAndLabels
    {
        {   DeviceTypeID::PDU,                     {"PDU"              , "PDU"}   },
        {   DeviceTypeID::EXTENSION_BAR,           {"Extension bar"    , "EXT"}   },
        {   DeviceTypeID::UPS,                     {"UPS"              , ""   }   },
        {   DeviceTypeID::SAN_SWITCH,              {"SAN switch"       , ""   }   },
        {   DeviceTypeID::SAN_DIRECTOR,            {"SAN director"     , ""   }   },
        {   DeviceTypeID::LAN_SWITCH,              {"LAN switch"       , ""   }   },
        {   DeviceTypeID::LAN_DIRECTOR,            {"LAN director"     , ""   }   },
        {   DeviceTypeID::INFINIBAND_SWITCH,       {"Infiniband switch", ""   }   },
        {   DeviceTypeID::KVM_SWITCH,              {"KVM switch"       , ""   }   },
        {   DeviceTypeID::RACK_SERVER,             {"Server"           , ""   }   },
        {   DeviceTypeID::SCALABLE_SERVER,         {"Scalable server"  , ""   }   },
        {   DeviceTypeID::STORAGE,                 {"Storage device"   , ""   }   },
        {   DeviceTypeID::BLADE_SERVER,            {"Blade system"     , ""   }   }
    };

    inline const std::map<DeviceTypeID, DevicePortTypesInfo_t> c_SwitchPortTypesInfoMap
    {
        {   DeviceTypeID::LAN_SWITCH,          {  { {"N", {"Ethernet", ""}} }, false  }   },
        {   DeviceTypeID::SAN_SWITCH,          {  { {"F", {"FC"      , ""}} }, true   }   },
        {   DeviceTypeID::INFINIBAND_SWITCH,   {  { {"I", {""        , ""}} }, true   }   },
        {   DeviceTypeID::KVM_SWITCH,          {  { {"K", {""        , ""}} }, false  }   }
    };

    inline const std::map<DeviceTypeID, AllowedDataPortTypes_t> c_DirectorDataPortTypesInfoMap
    {
        {   DeviceTypeID::SAN_DIRECTOR,          {  { "F", {"FC"      ,  "FC"}},
                                                    {"IS", {"iSCSI"   ,  "IS"}},
                                                    {"IP", { "FCIP"    , "FI"}}
                                                 }
        },
        {
            DeviceTypeID::LAN_DIRECTOR,         {   { "N", {"Ethernet",  "ETH"}}
                                                }
        }
    };

    inline const AllowedDataPortTypes_t c_ServerDataPortTypes
    {
        {"F", {"FC"        , "FC"   }},
        {"N", {"Ethernet"  , "ETH"  }},
        {"E", {"embedded"  , "EMB"  }},
        {"I", {"Infiniband", "IB"   }},
        {"S", {"SAS"       , "SAS"  }}
    };
}

#endif // DEVICEPORTDATA_H
