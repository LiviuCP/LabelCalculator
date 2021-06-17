#include "applicationdata.h"
#include "preparse.h"

void Utilities::createEmptyConnectionDefinitionsFile(std::ofstream& outputStream)
{
    assert(outputStream.is_open());

    outputStream << Data::c_ConnectionDefinitionsHeader << "\n";

    for (Data::UNumber_t uNumber{Data::c_MaxNrOfRackUnits}; uNumber > 0u; --uNumber)
    {
        outputStream << uNumber << "\n";
    }
}
