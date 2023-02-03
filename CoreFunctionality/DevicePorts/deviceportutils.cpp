#include "appsettings.h"
#include "deviceportutils.h"

std::string Utilities::DevicePorts::getCheckConnectionInputFileText()
{
    std::string promptCheckingFileText{"PLEASE REVIEW INPUT FILE: "};
    promptCheckingFileText += AppSettings::getInstance()->getConnectionInputFile();

    return  promptCheckingFileText;
}
