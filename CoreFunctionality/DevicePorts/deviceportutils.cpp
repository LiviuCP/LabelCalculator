#include "appsettings.h"
#include "deviceportutils.h"

std::string Utilities::DevicePorts::getCheckConnectionInputFileText()
{
    std::string promptCheckingFileText{"PLEASE REVIEW INPUT FILE: "};
    promptCheckingFileText += AppSettings::getInstance()->getConnectionInputFile().string();

    return  promptCheckingFileText;
}

bool Utilities::DevicePorts::isManagementPortNumber(std::string_view portNumber)
{
    return ("m" == portNumber || "M" == portNumber);
}

bool Utilities::DevicePorts::isPowerPortType(std::string_view portType)
{
    return ("p" == portType || "P" == portType);
}
