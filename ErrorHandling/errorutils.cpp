#include "appsettings.h"
#include "errorutils.h"

std::string getConnectionDefinitionsFile()
{
    const std::string c_ConnectionsDefinitionsFile{AppSettings::getInstance()->getConnectionDefinitionsFile()};
    return c_ConnectionsDefinitionsFile;
}

std::string getConnectionInputFile()
{
    const std::string c_ConnectionsInputFile{AppSettings::getInstance()->getConnectionInputFile()};
    return c_ConnectionsInputFile;
}
