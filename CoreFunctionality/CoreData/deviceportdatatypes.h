#ifndef DEVICEPORTDATATYPES_H
#define DEVICEPORTDATATYPES_H

#include <string>

struct SwitchPortData_t
{
    std::string mDataPortType;
    std::string mDataPortTypeDescription;
    bool mHasManagementPort;
};

#endif // DEVICEPORTDATATYPES_H
