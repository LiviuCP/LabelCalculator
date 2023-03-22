#ifndef PREPARSE_H
#define PREPARSE_H

#include <fstream>

#include "coreutils.h"

namespace Utilities::Other
{
    /* This function is used for creating an empty connection definitions file
       that should be filled-in by user before executing option 1
    */
    void createEmptyConnectionDefinitionsFile(const OutputStreamPtr pOutputStream);
}

#endif // PREPARSE_H
