#ifndef DEVICEPORTUTILS_H
#define DEVICEPORTUTILS_H

#include <string>

namespace Utilities
{
    static const std::string c_InvalidPortTypeErrorText{"LABELLING ERROR: INVALID PORT TYPE"};
    static const std::string c_InvalidModuleTypeErrorText{"LABELLING ERROR: INVALID MODULE TYPE"};
    static const std::string c_InvalidPlacementErrorText{"LABELLING ERROR: INVALID PLACEMENT TYPE"};
    static const std::string c_InvalidControllerNumberErrorText{"LABELLING ERROR: INVALID CONTROLLER NUMBER"};
    static const std::string c_InvalidModuleNumberErrorText{"LABELLING ERROR: INVALID MODULE NUMBER"};
    static const std::string c_InvalidLoadSegmentNumberErrorText{"LABELLING ERROR: INVALID LOAD SEGMENT NUMBER"};
    static const std::string c_InvalidPortNumberErrorText{"LABELLING ERROR: INVALID PORT NUMBER"};
    static const std::string c_MaxLabelCharsCountExceededErrorText{"LABELLING ERROR: MAXIMUM CHARACTERS COUNT EXCEEDED BY: "};
    static const std::string c_LabelErrorText{"INVALID LABEL! "};

    std::string getCheckConnectionInputFileText();
}

#endif // DEVICEPORTUTILS_H
