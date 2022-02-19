#include "appsettings.h"
#include "errorsettingsproxy.h"

std::string Settings::getConnectionDefinitionsFile()
{
    const std::string c_ConnectionsDefinitionsFile{AppSettings::getInstance()->getConnectionDefinitionsFile()};
    return c_ConnectionsDefinitionsFile;
}

std::string Settings::getConnectionInputFile()
{
    const std::string c_ConnectionsInputFile{AppSettings::getInstance()->getConnectionInputFile()};
    return c_ConnectionsInputFile;
}
