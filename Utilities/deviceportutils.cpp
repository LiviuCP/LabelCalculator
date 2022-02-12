#include "errorutils.h"
#include "deviceportutils.h"

std::string Utilities::getCheckConnectionInputFileText()
{
    std::string promptCheckingFileText{"PLEASE REVIEW INPUT FILE: "};
    promptCheckingFileText += Utilities::getConnectionInputFile();

    return  promptCheckingFileText;
}
