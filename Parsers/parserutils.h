#ifndef PARSERUTILS_H
#define PARSERUTILS_H

#include "applicationdata.h"

static const std::string c_InvalidCablePNErrorText{"Error! The cable part number is missing or contains invalid characters."};

// offset used for calculating the row number based on payload index (header row is ignored and row numbering starts at 1)
static constexpr short c_RowNumberOffset{2};

// offset used for calculating the column number of the first device port parameters (preceding columns are device type and device U position)
static constexpr short c_DevicePortParamsColumnOffset{2};

static constexpr short c_DevicesPerConnectionInputRowCount{2};

/* This function converts the input string containing the device type into a numeric device type ID that is then used for further processing (Device object creation, etc)
*/
DeviceTypeID getDeviceTypeID(const std::string& deviceType);

/* This function is used for getting the device type string for writing back into connection input file
*/
std::string getDeviceTypeAsString(DeviceTypeID deviceTypeID);


#endif // PARSERUTILS_H
