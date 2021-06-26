#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include "error.h"

class ErrorHandler
{
public:
    ErrorHandler();

    /* This function is used for resetting the factory functionality (currently only the counter of created errors)
    */
    void reset();

    /* This function is responsible for creating the actual error objects which are then used for logging parsing errors into error file
       An additional info string parameter could be added in case passing additional information to error objects is needed in the future
       Data items passed through this parameter should be separated by comma (',') similar to CSV cells
    */
    ErrorPtr logError(ErrorCode errorCode, const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream);

    /* This function checks if the "fewer cells" error occcurred on the last row number where an error was logged
    */
    bool fewerCellsErrorLogged() const;

    /* This function returns the last CSV row where an error has been logged (0: no errors logged so far)
    */
    size_t getLastLoggingRowNumber() const;

private:
    void _setLastLoggingRowNumber(const size_t fileRowNumber);

    size_t mLastLoggingRowNumber;
    bool mFewerCellsErrorOccurred;
};

#endif // ERRORHANDLER_H
