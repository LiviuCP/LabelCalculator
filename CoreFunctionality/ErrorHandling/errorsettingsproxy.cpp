#include "appsettings.h"
#include "errorsettingsproxy.h"

Core::Path_t Settings::getConnectionDefinitionsFile()
{
    const Core::Path_t c_ConnectionsDefinitionsFile{AppSettings::getInstance()->getConnectionDefinitionsFile()};
    return c_ConnectionsDefinitionsFile;
}

Core::Path_t Settings::getConnectionInputFile()
{
    const Core::Path_t c_ConnectionsInputFile{AppSettings::getInstance()->getConnectionInputFile()};
    return c_ConnectionsInputFile;
}
