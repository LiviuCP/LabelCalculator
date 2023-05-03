#include <algorithm>
#include <ctime>
#include <iomanip>

#include "applicationdata.h"
#include "coreutils.h"

/* An empty optional is returned if:
   - the CSV row string has been consumed or is empty
   - a null or out-of-bounds index has been passed
   It is the caller's responsibility to check if the passed index is valid (non-null and within string bounds)!
   Note: a field can also be partially read if index is within field substring (starting with the given index until separator/end of string).
*/
Index_t Utilities::Core::readDataField(const std::string_view src, std::string& dest, const Index_t index)
{
    Index_t nextIndex;

    if(const size_t c_Length{src.size()};
       c_Length > 0u && index.has_value() && index < c_Length)
    {
        dest.clear();

        size_t currentIndex{index.value()};
        bool emptyStartingFieldParsed{false};

        if (Data::c_CSVSeparator == src[currentIndex])
        {
            // if the parsed cell is the first one AND empty it should be taken into consideration and an empty destination string should be returned
            if (0u == currentIndex)
            {
                emptyStartingFieldParsed = true;
            }

            ++currentIndex;
        }

        if (!emptyStartingFieldParsed)
        {
            while(currentIndex < c_Length)
            {
                if(src[currentIndex] != Data::c_CSVSeparator)
                {
                    dest += src[currentIndex];
                    ++currentIndex;
                    continue;
                }

                break;
            }
        }

        if(currentIndex != c_Length)
        {
            nextIndex = currentIndex;
        }
    }

    return nextIndex;
}

/* converts a string "in place" to upper- or lowercase
*/
void Utilities::Core::convertStringCase(std::string& str, bool upperCase)
{
    auto toUpperTransformation{[](char c){return std::toupper(c);}};
    auto toLowerTransformation{[](char c){return std::tolower(c);}};

    if (upperCase)
    {
        std::transform(str.begin(), str.end(), str.begin(), toUpperTransformation);
    }
    else
    {
        std::transform(str.begin(), str.end(), str.begin(), toLowerTransformation);
    }
}

bool Utilities::Core::isDigitString(const std::string_view str)
{
    bool hasOnlyDigits{true};

    for (const auto& c : str)
    {
        if (!std::isdigit(c))
        {
            hasOnlyDigits = false;
            break;
        }
    }

    return hasOnlyDigits;
}

bool Utilities::Core::areInvalidCharactersContained(const std::string_view str)
{
    bool containsInvalidCharacters{false};

    for (const auto& c : str)
    {
        if (!std::isdigit(c) &&
            !std::isalpha(c) &&
            '-' != c)
        {
            containsInvalidCharacters = true;
            break;
        }
    }

    return containsInvalidCharacters;
}

bool Utilities::Core::areParseableCharactersContained(const std::string_view str)
{
    bool containsParseableCharacters{false};

    for (const auto& c : str)
    {
        if (!std::isspace(c) && Data::c_CSVSeparator != c)
        {
            containsParseableCharacters = true;
            break;
        }
    }

    return containsParseableCharacters;
}
