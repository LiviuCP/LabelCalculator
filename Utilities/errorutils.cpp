#include "appsettings.h"
#include "errorutils.h"

std::string Utilities::getConnectionDefinitionsFile()
{
    const std::string c_ConnectionsDefinitionsFile{AppSettings::getInstance()->getConnectionDefinitionsFile()};
    return c_ConnectionsDefinitionsFile;
}

std::string Utilities::getConnectionInputFile()
{
    const std::string c_ConnectionsInputFile{AppSettings::getInstance()->getConnectionInputFile()};
    return c_ConnectionsInputFile;
}
