#ifndef COREUTILS_H
#define COREUTILS_H

#include <string>
#include <optional>
#include <filesystem>

#ifdef _WIN32
#include "auxdata.h"
#endif

using Index_t = std::optional<size_t>;
using Path_t = std::filesystem::path;

namespace Utilities::Core
{
    /* This function reads a substring starting with index until reaching a comma character (end of .csv cell) or the string has no more characters
       If the index points to CSV separator (',') then reading starts with next character
    */
    Index_t readDataField(const std::string& src, std::string& dest, const Index_t index);

    /* This function converts the string case to upper or lower
    */
    void convertStringCase(std::string& str, bool upperCase);

    /* This function checks if the string contains only digits
    */
    bool isDigitString(const std::string& str);

    /* This function checks if the string contains invalid characters
       Only alphabet, numeric characters and '-' are considered valid.
    */
    bool areInvalidCharactersContained(const std::string& str);

    /* This function checks if a string contains parseable characters
       Any character that is not whitespace or comma is considered parseable
       Parseable chars are not necessarily valid ones
    */
    bool areParseableCharactersContained(const std::string& str);

    /* This function returns a string that represents a customized date/time value related to a file (e.g. last modified date)
    */
    std::string getFileTimeString(const std::filesystem::file_time_type& fileTime);
}

#endif // COREUTILS_H
