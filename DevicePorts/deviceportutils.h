#ifndef DEVICEPORTUTILS_H
#define DEVICEPORTUTILS_H

#include <string>

static const std::string c_UnknownPortTypeErrorText{"ERROR: UNKNOWN PORT TYPE (PLEASE CHECK INPUT FILE connectioninput.csv)"};
static const std::string c_UnknownModuleTypeErrorText{"ERROR: UNKNOWN MODULE TYPE (PLEASE CHECK INPUT FILE connectioninput.csv)"};
static const std::string c_InvalidPlacementErrorText{"ERROR: THE PLACEMENT TYPE YOU HAVE CHOSEN DOES NOT EXIST. PLEASE REVIEW INPUT FILE (connectioninput.csv)."};
static const std::string c_InvalidControllerNumberErrorText{"ERROR: THE CONTROLLER NUMBER IS INVALID. PLEASE REVIEW INPUT FILE (connectioninput.csv)."};
static const std::string c_InvalidModuleNumberErrorText{"ERROR: THE MODULE NUMBER IS INVALID. PLEASE REVIEW INPUT FILE (connectioninput.csv)."};
static const std::string c_InvalidLoadSegmentNumberErrorText{"ERROR. THE LOAD SEGMENT NUMBER IS INVALID. PLEASE REVIEW INPUT FILE (connectioninput.csv)."};
static const std::string c_InvalidPortNumberErrorText{"ERROR: THE PORT NUMBER IS INVALID. PLEASE REVIEW INPUT FILE (connectioninput.csv)."};
static const std::string c_LabelErrorText{"ERROR!!!"};

#endif // DEVICEPORTUTILS_H
