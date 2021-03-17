#ifndef DISPLAYUTILS_H
#define DISPLAYUTILS_H

#include <string>

/* Functions used by application to display data to the console */

void displayErrorMessage(const std::string& inputFilename, const std::string& errorFilename);

void displaySuccessMessage(const std::string& outputFilename, bool displayFurtherInstructions);

void displayAbortMessage();

void displayMenu();

void displayVersion();

#endif // DISPLAYUTILS_H
