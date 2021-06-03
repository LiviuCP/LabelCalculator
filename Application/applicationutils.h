#ifndef APPLICATIONUTILS_H
#define APPLICATIONUTILS_H

#include <string>

/* This function reads a substring starting with index until reaching a comma character (end of .csv cell) or the string has no more characters
   If the index points to CSV separator (',') then reading starts with next character
*/
ssize_t readDataField(const std::string& src, std::string& dest, const ssize_t index);

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

#endif // APPLICATIONUTILS_H
