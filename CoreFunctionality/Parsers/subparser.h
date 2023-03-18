#ifndef SUBPARSER_H
#define SUBPARSER_H

#include <memory>
#include <utility>

#include "errorhandler.h"

class ISubParser
{
public:
    virtual void setErrorHandler(std::shared_ptr<ErrorHandler> pErrorHandler) = 0;
    virtual void setFileColumnNumber(const size_t fileColumnNumber) = 0;
    virtual void setCurrentPosition(const Index_t currentPosition) = 0;

    virtual Index_t getCurrentPosition() const = 0;
    virtual size_t getFileRowNumber() const = 0;
    virtual size_t getFileColumnNumber() const = 0;
};

#endif // SUBPARSER_H
