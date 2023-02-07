#include "appsettings.h"
#include "errorsettingsproxy.h"

Path_t Settings::getConnectionDefinitionsFile()
{
    const Path_t c_ConnectionsDefinitionsFile{AppSettings::getInstance()->getConnectionDefinitionsFile()};
    return c_ConnectionsDefinitionsFile;
}

Path_t Settings::getConnectionInputFile()
{
    const Path_t c_ConnectionsInputFile{AppSettings::getInstance()->getConnectionInputFile()};
    return c_ConnectionsInputFile;
}
