#include <iostream>
#include <cassert>
#include <algorithm>

#if not defined (__APPLE__) && not defined (__MACH__) && not defined(__unix__)
#include <windows.h>
#endif

#include "labelutils.h"

/* The input/output index need to be signed integer as value -1 can also be returned if the CSV string has been consumed.
   However internally the processing of the string can be done with an unsigned int (size_t) when the index is "in bounds".
*/
ssize_t readDataField(const std::string& src, std::string& dest, const ssize_t index)
{
    const size_t c_Length{src.size()};

    ssize_t nextIndex{-1};

    if(c_Length > 0u && index >= 0)
    {
        size_t currentIndex{static_cast<size_t>(index)};
        assert(currentIndex < c_Length);

        dest.clear();

        bool emptyStartingFieldParsed{false};

        if (c_CSVSeparator == src[currentIndex])
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
                if(src[currentIndex] != c_CSVSeparator)
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

DeviceTypeID getDeviceTypeID(const std::string& deviceType)
{
    DeviceTypeID deviceTypeID{DeviceTypeID::UNKNOWN_DEVICE};

    std::string deviceTypeLowerCase{deviceType};
    convertStringCase(deviceTypeLowerCase, false);

    std::map<std::string, DeviceTypeID>::const_iterator it{c_DeviceTypeTextToIDMapping.find(deviceTypeLowerCase)};

    if (it != c_DeviceTypeTextToIDMapping.cend())
    {
        deviceTypeID = it->second;
    }

    return deviceTypeID;
}

std::string getDeviceTypeAsString(DeviceTypeID deviceTypeID)
{
    std::string deviceType;

    for (std::map<std::string, DeviceTypeID>::const_iterator it{c_DeviceTypeTextToIDMapping.cbegin()}; it != c_DeviceTypeTextToIDMapping.cend(); ++it)
    {
        if (deviceTypeID == it->second)
        {
            deviceType = it->first;
            break;
        }
    }

    return deviceType;
}

/* converts a string "in place" to upper- or lowercase
*/
void convertStringCase(std::string& str, bool upperCase)
{
    auto transformation = upperCase ? [](char c){return std::toupper(c);} : [](char c){return std::tolower(c);};
    std::transform(str.begin(), str.end(), str.begin(), transformation);
}

bool isDigitString(const std::string& str)
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

bool areInvalidCharactersContained(const std::string& str)
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

std::string getUsername()
{
#if defined (__APPLE__) && defined (__MACH__)
    const std::string username{getenv("USER")};
#elif defined (__unix__)
    const std::string username{getenv("USERNAME")};
#else
    std::string username;

    char buffer[100];
    DWORD length{sizeof(buffer)};
    const bool c_IsValid{GetUserNameA(buffer, &length)};

    if (c_IsValid)
    {
        username = std::string{buffer};
    }
#endif
    return username;
}
