#ifndef PREPARSE_H
#define PREPARSE_H

#include "coreutils.h"

namespace Core = Utilities::Core;

namespace Utilities::Other
{
    /* This function is used for creating an empty connection definitions file
       that should be filled-in by user before executing option 1
    */
    void createEmptyConnectionDefinitionsFile(const Core::OutputStreamPtr pOutputStream);
}

#endif // PREPARSE_H
