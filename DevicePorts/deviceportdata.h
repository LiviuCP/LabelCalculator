#ifndef DEVICEPORTDATA_H
#define DEVICEPORTDATA_H

#include <cstdlib>

namespace Data
{
    // number of fields that should be filled in connectioninput.csv for EACH device (unused fields can be filled in with '-')
    static constexpr size_t c_MaxPortInputParametersCount{3};
}

#endif // DEVICEPORTDATA_H
