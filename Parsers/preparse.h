#ifndef PREPARSE_H
#define PREPARSE_H

#include <fstream>

namespace Utilities
{
    /* This function is used for creating an empty connection definitions file
       that should be filled-in by user before executing option 1
    */
    void createEmptyConnectionDefinitionsFile(std::ofstream& outputStream);
}

#endif // PREPARSE_H
