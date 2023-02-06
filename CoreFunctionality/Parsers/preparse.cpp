#include <cassert>

#include "applicationdata.h"
#include "preparse.h"

void Utilities::Other::createEmptyConnectionDefinitionsFile(std::ofstream& outputStream)
{
    assert(outputStream.is_open());

    outputStream << Data::c_ConnectionDefinitionsHeader << "\n";

    for (Data::UNumber_t uNumber{Data::c_MaxRackUnitsCount}; uNumber > 0u; --uNumber)
    {
        outputStream << uNumber << "\n";
    }
}
