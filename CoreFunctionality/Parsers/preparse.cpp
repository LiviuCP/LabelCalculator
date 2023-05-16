#include "applicationdata.h"
#include "preparse.h"

void Utilities::Other::createEmptyConnectionDefinitionsFile(const Core::OutputStreamPtr pOutputStream)
{
    if(pOutputStream && pOutputStream->is_open())
    {
        *pOutputStream << Data::c_ConnectionDefinitionsHeader << "\n";

        for (Data::UNumber_t uNumber{Data::c_MaxRackUnitsCount}; uNumber > 0u; --uNumber)
        {
            *pOutputStream << uNumber << "\n";
        }
    }
    else
    {
        ASSERT(false, "The output file stream is invalid or not open");
    }
}
