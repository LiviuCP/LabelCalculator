#ifndef LABELUTILS_H
#define LABELUTILS_H

#include <vector>
#include <map>
#include <fstream>
#include <string>

/* *CONSTANTS* */

static constexpr int c_MaxNrOfRackUnits{50};
static constexpr int c_RowNumberOffset{2}; // offset used for calculating the row number based on payload index (header row is ignored and row numbering starts at 1)
static constexpr int c_DevicesPerConnectionInputRowCount{2};

static const std::string c_CablePartNumberPlaceholder{"CBL_PART_NR"};
static const std::string c_InputHeader{"__Cable part number__,__Source device type__,__Source U number__,__Parameter1__,__Parameter2__,__Parameter3__,__Destination device type__,__Destination U number__,__Parameter1__,__Parameter2__,__Parameter3__"};
static const std::string c_OutputHeader{"__Item number__,__Cable part number__,__Source device description__,__Source label__,__Destination device description__,__Destination label__"};

#if defined (__APPLE__) && defined (__MACH__)
static const std::string c_HomeDirParent{"/Users"};
#elif defined (__unix__)
static const std::string c_HomeDirParent{"/home"};
#else
static const std::string c_HomeDirParent{"C:\\Users"};
#endif

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
static const std::string c_ConfigurationFilePath{"/tmp/configuration.txt"};
static const std::string c_ClearScreenCommand{"clear"};
static constexpr char c_PathSeparator{'/'};
static constexpr char c_CSVSeparator{','};
#else
static const std::string c_ConfigurationFilename{"C:\\tmp\\configuration.txt"};
static const std::string c_ClearScreenCommand{"cls"};
static constexpr char c_PathSeparator{'\\'};
#endif

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
    POWER_SUPPLY,
    LAN_SWITCH,
    SAN_SWITCH,
    INFINIBAND_SWITCH,
    KVM_SWITCH,
    RACK_SERVER,
    STORAGE,
    BLADE_SERVER,
};

static const std::map<std::string, DeviceTypeID> c_DeviceTypeToIDMapping
{
    {    "_pdu",    DeviceTypeID::PDU                   },
    {    "_ext",    DeviceTypeID::EXTENSION_BAR         },
    {    "_ups",    DeviceTypeID::UPS                   },
    {    "_ps",     DeviceTypeID::POWER_SUPPLY          },
    {    "lan",     DeviceTypeID::LAN_SWITCH            },
    {    "san",     DeviceTypeID::SAN_SWITCH            },
    {    "ib",      DeviceTypeID::INFINIBAND_SWITCH     },
    {    "kvm",     DeviceTypeID::KVM_SWITCH            },
    {    "svr",     DeviceTypeID::RACK_SERVER           },
    {    "sto",     DeviceTypeID::STORAGE               },
    {    "bld",     DeviceTypeID::BLADE_SERVER          }
};

static const std::map<DeviceTypeID, int> c_RequiredNrOfInputParams
{
    {    DeviceTypeID::PDU,                 4    },
    {    DeviceTypeID::EXTENSION_BAR,       3    },
    {    DeviceTypeID::UPS,                 3    },
    {    DeviceTypeID::POWER_SUPPLY,        2    },
    {    DeviceTypeID::LAN_SWITCH,          2    },
    {    DeviceTypeID::SAN_SWITCH,          2    },
    {    DeviceTypeID::INFINIBAND_SWITCH,   2    },
    {    DeviceTypeID::KVM_SWITCH,          2    },
    {    DeviceTypeID::RACK_SERVER,         3    },
    {    DeviceTypeID::STORAGE,             3    },
    {    DeviceTypeID::BLADE_SERVER,        4    }
};

static const std::map<DeviceTypeID, int> c_MaxAllowedCharsCount
{
    {    DeviceTypeID::PDU,                 6    },
    {    DeviceTypeID::EXTENSION_BAR,       8    },
    {    DeviceTypeID::UPS,                11    },
    {    DeviceTypeID::POWER_SUPPLY,       12    },
    {    DeviceTypeID::LAN_SWITCH,         13    },
    {    DeviceTypeID::SAN_SWITCH,         10    },
    {    DeviceTypeID::INFINIBAND_SWITCH,  10    },
    {    DeviceTypeID::KVM_SWITCH,         13    },
    {    DeviceTypeID::RACK_SERVER,         8    },
    {    DeviceTypeID::STORAGE,             7    },
    {    DeviceTypeID::BLADE_SERVER,        7    }
};

enum class ErrorCode
{
    EMPTY_CELL = 1,
    MAX_CHARS_EXCEEDED,
    UNKNOWN_DEVICE,
    FEWER_CELLS,
    WRONG_CONNECTION_FORMAT,
    PLACEMENT_OUT_OF_RANGE,
    NO_DEVICE_PLACED_IN_POSITION,
    DEVICE_CONNECTED_TO_ITSELF,
    NULL_NR_OF_CONNECTIONS,
    ErrorCodesCount
};

/* *FUNCTIONS* */

/* This function reads a substring starting with index until reaching a comma character (end of .csv cell) or the string has no more characters
   If the index points to CSV separator (',') then reading starts with next character
*/
int readDataField(const std::string& src, std::string& dest, const int index);

/* This function initializes the application no matter which option is chosen (define connections, process connection input from user)
*/
bool init(std::string& connectionsFilename, std::string& inputFilename, std::string& outputFilename, std::string& errorFilename, std::ofstream& errorStream);

/* This function opens the input and output files for each of the options
*/
bool enableReadWriteOperations(std::ifstream& inputStream, std::ofstream& outputStream, const std::string& inputFilename, const std::string& outputFilename);

/* This function writes the resulting output to the corresponding file for each option
*/
void writeOutputToFile(std::ofstream& outputStream, const std::vector<std::string>& inputRows, const std::string& header);

/* This function converts the input string containing the device type into a numeric device type ID that is then used for further processing (Device object creation, etc)
*/
DeviceTypeID getDeviceTypeID(const std::string& deviceType);

/* This function is used for getting the device type string for writing back into connection input file
*/
std::string getDeviceType(DeviceTypeID deviceTypeID);

/* This function converts the string case to upper or lower
*/
void convertStringCase(std::string &source, std::string& destination, bool upperCase);

#endif // LABELUTILS_H
