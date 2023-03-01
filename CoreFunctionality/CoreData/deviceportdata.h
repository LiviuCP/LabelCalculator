#ifndef DEVICEPORTDATA_H
#define DEVICEPORTDATA_H

#include <map>
#include <string>
#include <cstdlib>

#include "deviceporttypes.h"
#include "applicationdata.h"

namespace Data
{
    // number of fields that should be filled in connectioninput.csv for EACH device (unused fields can be filled in with '-')
    inline constexpr size_t c_MaxPortInputParametersCount{3};

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
        {   DeviceTypeID::RACK_SERVER,             {"Server"           , ""   }   },
        {   DeviceTypeID::SCALABLE_SERVER,         {"Scalable server"  , ""   }   },
        {   DeviceTypeID::BLADE_SERVER,            {"Blade system"     , ""   }   },
        {   DeviceTypeID::STORAGE,                 {"Storage device"   , ""   }   },
        {   DeviceTypeID::SAN_SWITCH,              {"SAN switch"       , ""   }   },
        {   DeviceTypeID::LAN_SWITCH,              {"LAN switch"       , ""   }   },
        {   DeviceTypeID::INFINIBAND_SWITCH,       {"Infiniband switch", ""   }   },
        {   DeviceTypeID::KVM_SWITCH,              {"KVM switch"       , ""   }   }
    };

    inline const std::map<DeviceTypeID, DevicePortTypesInfo_t> c_SwitchPortTypesInfoMap
    {
        {   DeviceTypeID::LAN_SWITCH,          {  { {"N", {"Ethernet", ""}} }, false  }   },
        {   DeviceTypeID::SAN_SWITCH,          {  { {"F", {"FC"      , ""}} }, true   }   },
        {   DeviceTypeID::INFINIBAND_SWITCH,   {  { {"I", {""        , ""}} }, true   }   },
        {   DeviceTypeID::KVM_SWITCH,          {  { {"K", {""        , ""}} }, false  }   }
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
