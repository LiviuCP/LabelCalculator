#ifndef LABELUTILS_H
#define LABELUTILS_H

#include <string>
#include <map>

/* *CONSTANTS* */

static constexpr int c_MaxNrOfRackUnits{50};

static const std::string c_NoDevice{"NoDevice"}; // no device placed in the rack U position
static const std::string c_CablePartNumberPlaceholder{"CBL_PART_NR"};
static const std::string c_InputHeader{"__Cable part number__,__Source device type__,__Source U number__,__Parameter1__,__Parameter2__,__Parameter3__,__Destination device type__,__Destination U number__,__Parameter1__,__Parameter2__,__Parameter3__"};
static const std::string c_OutputHeader{"__Item number__,__Cable part number__,__Source device description__,__Source label__,__Destination device description__,__Destination label__"};
static const std::string c_ConfigurationFilename{"/tmp/configuration.txt"};

enum class ErrorTypes
{
    EMPTY_CELL = 1,
    MAX_CHARS_EXCEEDED,
    UNKNOWN_DEVICE,
    FEWER_CELLS,
    WRONG_CONNECTION_FORMAT,
    PLACEMENT_OUT_OF_RANGE,
    NO_DEVICE_PLACED_IN_POSITION,
    DEVICE_CONNECTED_TO_ITSELF,
    NULL_NR_OF_CONNECTIONS
};

static const std::map<std::string, int> c_MaxAllowedNrOfChars
{
    {"_pdu", 6},
    {"_ext", 8},
    {"_ups", 11},
    {"_ps", 12},
    {"lan", 13},
    {"san", 10},
    {"ib",  10},
    {"kvm", 13},
    {"svr", 8},
    {"sto", 7},
    {"bld", 7}
};

/* *FUNCTIONS* */

/* This function reads a substring starting with index until reaching a comma character (end of .csv cell) or the string has no more characters
   If the index points to ',' then reading starts with next character
*/
int readDataField(const std::string& src, std::string& dest, const int index);

/* This function checks the format of the connection of the first device from each row of the connectiondefinitions.csv file to another device from the row
   If the format is correct (true returned) it will also fill in the two arguments with the U placement of the other device and the number of connections between the two
*/
bool parseConnectionFormatting(const std::string& source, int& secondDevice, int& connectionsCount);

/* This function fills in the placeholder information that will be written subsequently in connectioninput.csv along with the name and type of the device
*/
void createPlaceholders(const std::string& deviceType, std::string& dest);

#endif // LABELUTILS_H
