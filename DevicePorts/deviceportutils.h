#ifndef DEVICEPORTUTILS_H
#define DEVICEPORTUTILS_H

#include <string>

namespace Utilities
{
    static const std::string c_InvalidPortTypeErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE PORT TYPE"};
    static const std::string c_InvalidModuleTypeErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE MODULE TYPE"};
    static const std::string c_InvalidPlacementErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE PLACEMENT TYPE"};
    static const std::string c_InvalidControllerNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE CONTROLLER NUMBER"};
    static const std::string c_InvalidModuleNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE MODULE NUMBER"};
    static const std::string c_InvalidLoadSegmentNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE LOAD SEGMENT NUMBER"};
    static const std::string c_InvalidPortNumberErrorText{"LABELLING ERROR: INVALID OR INCOMPATIBLE PORT NUMBER"};
    static const std::string c_MaxLabelCharsCountExceededErrorText{"LABELLING ERROR: MAXIMUM CHARACTERS COUNT EXCEEDED BY: "};
    static const std::string c_LabelErrorText{"INVALID LABEL! "};

    std::string getCheckConnectionInputFileText();
}

#endif // DEVICEPORTUTILS_H
