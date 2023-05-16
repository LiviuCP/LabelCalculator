#ifndef ERRORSETTINGSPROXY_H
#define ERRORSETTINGSPROXY_H

#include "coreutils.h"

namespace Core = Utilities::Core;

namespace Settings
{
    /* the error handling classes are not supposed to directly access the settings
       but instead use these intermediary functions to get the settings data on a "need to know" basis
    */
    Core::Path_t getConnectionDefinitionsFile();
    Core::Path_t getConnectionInputFile();
}

#endif // ERRORSETTINGSPROXY_H
