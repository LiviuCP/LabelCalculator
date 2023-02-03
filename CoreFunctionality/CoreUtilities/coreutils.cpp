#include <cassert>
#include <algorithm>

#include "applicationdata.h"
#include "coreutils.h"

/* The input/output index need to be signed integer as value -1 can also be returned if the CSV string has been consumed.
   However internally the processing of the string can be done with an unsigned int (size_t) when the index is "in bounds".
*/
ssize_t Utilities::Core::readDataField(const std::string& src, std::string& dest, const ssize_t index)
{
    const size_t c_Length{src.size()};

    ssize_t nextIndex{-1};

    if(c_Length > 0u && index >= 0)
    {
        size_t currentIndex{static_cast<size_t>(index)};
        assert(currentIndex < c_Length);

        dest.clear();

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
                }
                else
                {
                    break;
                }

                ++currentIndex;
            }
        }

        if(currentIndex != c_Length)
        {
            // no issue in converting to int as the size is reasonable (same for other similar situations in this project)
            nextIndex = static_cast<ssize_t>(currentIndex);
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

bool Utilities::Core::isDigitString(const std::string& str)
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

bool Utilities::Core::areInvalidCharactersContained(const std::string& str)
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

bool Utilities::Core::areParseableCharactersContained(const std::string& str)
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
