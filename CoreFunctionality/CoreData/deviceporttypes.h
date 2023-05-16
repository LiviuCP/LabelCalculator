#ifndef DEVICEPORTTYPES_H
#define DEVICEPORTTYPES_H

#include <map>
#include <string>

namespace Data
{
    /* Key is: port type input by user (e.g. F for fiber channel),
       Value is port type description + label pair to be included in the output file (e.g. fiber channel / FC)
    */
    using AllowedDataPortTypes_t = std::map<std::string, std::pair<std::string, std::string>>;

    struct DevicePortTypesInfo_t
    {
        // A std::map is required as there might be more data port types for a specific device (e.g. both FC and Ethernet)
        AllowedDataPortTypes_t mAllowedDataPortTypes;

        bool mIsManagementPortAllowed;
    };
}

#endif // DEVICEPORTTYPES_H
