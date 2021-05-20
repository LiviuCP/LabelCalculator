#ifndef LABELUTILS_H
#define LABELUTILS_H

#include <vector>
#include <map>
#include <fstream>
#include <string>

/* --CONSTANTS-- */

using UNumber_t = size_t;

// offset used for calculating the row number based on payload index (header row is ignored and row numbering starts at 1)
static constexpr short c_RowNumberOffset{2};

// offset used for calculating the column number of the first device port parameters (preceding columns are device type and device U position)
static constexpr short c_DevicePortParamsColumnOffset{2};

static constexpr unsigned short c_MaxNrOfRackUnits{50u};
static constexpr short c_DevicesPerConnectionInputRowCount{2};

static const std::string c_CablePartNumberPlaceholder{"CBL_PART_NR"};
static const std::string c_InputHeader{"__Cable part number__,__Source device type__,__Source U number__,__Parameter1__,__Parameter2__,"
                                       "__Parameter3__,__Destination device type__,__Destination U number__,__Parameter1__,__Parameter2__,__Parameter3__"};
static const std::string c_OutputHeader{"__Item number__,__Cable part number__,__Source device description__,"
                                        "__Source label__,__Destination device description__,__Destination label__"};

static const std::string c_UnknownPortTypeErrorText{"ERROR: UNKNOWN PORT TYPE (PLEASE CHECK INPUT FILE connectioninput.csv)"};
static const std::string c_UnknownModuleTypeErrorText{"ERROR: UNKNOWN MODULE TYPE (PLEASE CHECK INPUT FILE connectioninput.csv)"};
static const std::string c_InvalidPlacementErrorText{"ERROR: THE PLACEMENT TYPE YOU HAVE CHOSEN DOES NOT EXIST. PLEASE REVIEW INPUT FILE (connectioninput.csv)."};
static const std::string c_InvalidControllerNumberErrorText{"ERROR: THE CONTROLLER NUMBER IS INVALID. PLEASE REVIEW INPUT FILE (connectioninput.csv)."};
static const std::string c_InvalidModuleNumberErrorText{"ERROR: THE MODULE NUMBER IS INVALID. PLEASE REVIEW INPUT FILE (connectioninput.csv)."};
static const std::string c_InvalidLoadSegmentNumberErrorText{"ERROR. THE LOAD SEGMENT NUMBER IS INVALID. PLEASE REVIEW INPUT FILE (connectioninput.csv)."};
static const std::string c_InvalidPortNumberErrorText{"ERROR: THE PORT NUMBER IS INVALID. PLEASE REVIEW INPUT FILE (connectioninput.csv)."};
static const std::string c_LabelErrorText{"ERROR!!!"};
static const std::string c_InvalidCablePNErrorText{"Error! The cable part number is missing or contains invalid characters."};

#if defined (__APPLE__) && defined (__MACH__)
static const std::string c_HomeDirParent{"/Users"};
#elif defined (__unix__)
static const std::string c_HomeDirParent{"/home"};
#else
static const std::string c_HomeDirParent{"C:\\Users"};
#endif

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
static const std::string c_ClearScreenCommand{"clear"};
static constexpr char c_PathSeparator{'/'};
#else
static const std::string c_ClearScreenCommand{"cls"};
static constexpr char c_PathSeparator{'\\'};
#endif

static constexpr char c_CSVSeparator{','};

static const std::string c_DocumentsDirName{"Documents"};
static const std::string c_ConnectionDefinitionsFilename{"connectiondefinitions.csv"};
static const std::string c_ConnectionInputFilename{"connectioninput.csv"};
static const std::string c_LabellingTableFilename{"labellingtable.csv"};
static const std::string c_ErrorFilename{"error.txt"};

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
    {    "_pdu",    DeviceTypeID::PDU                   },
    {    "_ext",    DeviceTypeID::EXTENSION_BAR         },
    {    "_ups",    DeviceTypeID::UPS                   },
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

static const std::map<DeviceTypeID, size_t> c_MaxAllowedCharsCount
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

enum class ErrorCode : unsigned short
{
    EMPTY_CELL = 1u,
    MAX_CHARS_EXCEEDED,
    UNKNOWN_DEVICE,
    FEWER_CELLS,
    WRONG_CONNECTION_FORMAT,
    PLACEMENT_OUT_OF_RANGE,
    INVALID_TARGET_DEVICE_POSITION,
    DEVICE_CONNECTED_TO_ITSELF,
    NULL_NR_OF_CONNECTIONS,
    INVALID_CHARS,
    INVALID_DEVICE_U_POSITION,
    ErrorCodesUpperBound
};

/* --FUNCTIONS-- */

/* This function reads a substring starting with index until reaching a comma character (end of .csv cell) or the string has no more characters
   If the index points to CSV separator (',') then reading starts with next character
*/
ssize_t readDataField(const std::string& src, std::string& dest, const ssize_t index);

/* This function converts the input string containing the device type into a numeric device type ID that is then used for further processing (Device object creation, etc)
*/
DeviceTypeID getDeviceTypeID(const std::string& deviceType);

/* This function is used for getting the device type string for writing back into connection input file
*/
std::string getDeviceTypeAsString(DeviceTypeID deviceTypeID);

/* This function converts the string case to upper or lower
*/
void convertStringCase(std::string& str, bool upperCase);

/* This function checks if the string contains only digits
*/
bool isDigitString(const std::string& str);

/* This function checks if the string contains invalid characters
   Only alphabet, numeric characters and '-' are considered valid.
*/
bool areInvalidCharactersContained(const std::string& str);

/* This function retrieves the username
   It is used for determining the paths of the application files
*/
std::string getUsername();

#endif // LABELUTILS_H
