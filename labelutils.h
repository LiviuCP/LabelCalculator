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

static const std::string c_NoDevice{"NoDevice"}; // no device placed in the rack U position
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
   If the index points to ',' then reading starts with next character
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

#endif // LABELUTILS_H
