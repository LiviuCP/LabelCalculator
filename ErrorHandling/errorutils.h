#ifndef ERRORUTILS_H
#define ERRORUTILS_H

#include <string>

namespace Utilities
{
    /* the error handling classes are not supposed to directly access the settings
       but instead use these intermediary functions to get the settings data on a "need to know" basis
    */
    std::string getConnectionDefinitionsFile();
    std::string getConnectionInputFile();
}

#endif // ERRORUTILS_H
