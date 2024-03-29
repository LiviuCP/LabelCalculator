#ifndef DEVICEPORTUTILS_H
#define DEVICEPORTUTILS_H

#include <string_view>
#include <string>

namespace Utilities::DevicePorts
{
    inline constexpr std::string_view c_InvalidPortTypeErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE PORT TYPE"};
    inline constexpr std::string_view c_InvalidModuleTypeErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE MODULE TYPE"};
    inline constexpr std::string_view c_InvalidPlacementErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE PLACEMENT TYPE"};
    inline constexpr std::string_view c_InvalidControllerNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE CONTROLLER NUMBER"};
    inline constexpr std::string_view c_InvalidModuleNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE MODULE NUMBER"};
    inline constexpr std::string_view c_InvalidSlotNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE SLOT NUMBER"};
    inline constexpr std::string_view c_InvalidBladeNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE BLADE NUMBER"};
    inline constexpr std::string_view c_InvalidLoadSegmentNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE LOAD SEGMENT NUMBER"};
    inline constexpr std::string_view c_InvalidPortNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE PORT NUMBER"};
    inline constexpr std::string_view c_MaxLabelCharsCountExceededErrorText{"LABELLING ERROR: MAXIMUM CHARACTERS COUNT EXCEEDED BY: "};
    inline constexpr std::string_view c_LabelErrorText{"INVALID LABEL! "};

    std::string getCheckConnectionInputFileText();
    bool isManagementPortNumber(const std::string_view portNumber);
    bool isPowerPortType(const std::string_view portType);
}

#endif // DEVICEPORTUTILS_H
