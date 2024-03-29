#ifndef COREUTILS_H
#define COREUTILS_H

#include <string>
#include <string_view>
#include <optional>
#include <filesystem>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <ctime>
#include <iomanip>
#include <cassert>

#ifdef _WIN32
#include "auxdata.h"
#endif

#ifdef NDEBUG
#define ASSERT(condition, message) \
    if (!(condition) && (std::string{} != message)) \
    { \
        std::cerr << "\n\nINTERNAL ERROR! " << message << "\n\n"; \
    }
#else
#define ASSERT(condition, message) \
    (void)message; \
    assert(condition);
#endif

namespace Utilities::Core
{
    using Index_t = std::optional<size_t>;
    using Path_t = std::filesystem::path;
    using Error_t = unsigned short;

    using InputStreamPtr = std::shared_ptr<std::ifstream>;
    using OutputStreamPtr = std::shared_ptr<std::ofstream>;
    using ErrorStreamPtr = std::shared_ptr<std::ofstream>;

    /* This function reads a substring starting with index until reaching a comma character (end of .csv cell) or the string has no more characters
       If the index points to CSV separator (',') then reading starts with next character
    */
    Index_t readDataField(const std::string_view src, std::string& dest, const Index_t index);

    /* This function converts the string case to upper or lower
    */
    void convertStringCase(std::string& str, bool upperCase);

    /* This function checks if the string contains only digits
    */
    bool isDigitString(const std::string_view str);

    /* This function checks if the string contains invalid characters
       Only alphabet, numeric characters and '-' are considered valid.
    */
    bool areInvalidCharactersContained(const std::string_view str);

    /* This function checks if a string contains parseable characters
       Any character that is not whitespace or comma is considered parseable
       Parseable chars are not necessarily valid ones
    */
    bool areParseableCharactersContained(const std::string_view str);

    /* This function returns a string that represents a customized date/time value (e.g. current date and time)
    */
    template<typename clockType> std::string getDateTimeString(const std::chrono::time_point<clockType>& timePoint)
    {
        const std::time_t c_Time{clockType::to_time_t(timePoint)};
        struct std::tm timeStruct;
#ifndef _WIN32
        (void)localtime_r(&c_Time, &timeStruct);
#else
        (void)localtime_s(&timeStruct, &c_Time);
#endif
        std::stringstream stream;
        stream << std::put_time(&timeStruct, "%F_%H%M%S");

        const std::string c_DateTimeString{stream.str()};

        return c_DateTimeString;
    }
}

#endif // COREUTILS_H
