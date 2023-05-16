#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include "error.h"

class ErrorHandler
{
public:
    ErrorHandler() = delete;
    ErrorHandler(const Core::ErrorStreamPtr pErrorStream);

    /* This function is responsible for creating the actual error objects which are then used for logging parsing errors into error file
       An additional info string parameter could be added in case passing additional information to error objects is needed in the future
       Data items passed through this parameter should be separated by comma (',') similar to CSV cells
    */
    ErrorPtr logError(const Core::Error_t errorCode, const size_t fileRowNumber, const size_t fileColumnNumber);

private:
    const Core::ErrorStreamPtr mpErrorStream;
};

using ErrorHandlerPtr = std::shared_ptr<ErrorHandler>;

#endif // ERRORHANDLER_H
