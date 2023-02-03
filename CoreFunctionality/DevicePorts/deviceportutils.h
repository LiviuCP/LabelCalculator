#ifndef DEVICEPORTUTILS_H
#define DEVICEPORTUTILS_H

#include <string_view>
#include <string>

namespace Utilities
{
    inline constexpr std::string_view c_InvalidPortTypeErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE PORT TYPE"};
    inline constexpr std::string_view c_InvalidModuleTypeErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE MODULE TYPE"};
    inline constexpr std::string_view c_InvalidPlacementErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE PLACEMENT TYPE"};
    inline constexpr std::string_view c_InvalidControllerNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE CONTROLLER NUMBER"};
    inline constexpr std::string_view c_InvalidModuleNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE MODULE NUMBER"};
    inline constexpr std::string_view c_InvalidLoadSegmentNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE LOAD SEGMENT NUMBER"};
    inline constexpr std::string_view c_InvalidPortNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE PORT NUMBER"};
    inline constexpr std::string_view c_MaxLabelCharsCountExceededErrorText{"LABELLING ERROR: MAXIMUM CHARACTERS COUNT EXCEEDED BY: "};
    inline constexpr std::string_view c_LabelErrorText{"INVALID LABEL! "};

    std::string getCheckConnectionInputFileText();
}

#endif // DEVICEPORTUTILS_H
