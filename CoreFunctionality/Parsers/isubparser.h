#ifndef ISUBPARSER_H
#define ISUBPARSER_H

#include <memory>
#include <utility>

#include "errorhandler.h"

class IParser;

class ISubParser
{
public:
    virtual void setParentParser(IParser* const pIParser) = 0;
    virtual void setErrorHandler(const ErrorHandlerPtr pErrorHandler) = 0;
    virtual void setFileColumnNumber(const size_t fileColumnNumber) = 0;
    virtual void setCurrentPosition(const Index_t currentPosition) = 0;

    virtual Index_t getCurrentPosition() const = 0;
    virtual size_t getFileRowNumber() const = 0;
    virtual size_t getFileColumnNumber() const = 0;
};

#endif // ISUBPARSER_H