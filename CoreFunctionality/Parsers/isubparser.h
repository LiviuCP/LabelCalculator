#ifndef ISUBPARSER_H
#define ISUBPARSER_H

#include "errorhandler.h"

class ISubParserObserver;

class ISubParser
{
public:
    virtual ~ISubParser() {};

    // used for performing specific initializations that cannot be handled within constructor
    virtual void init() {};

    virtual void parseInputData(std::vector<ErrorPtr>& parsingErrors) = 0;

    virtual void setSubParserObserver(ISubParserObserver* const pISubParserObserver) = 0;
    virtual void setErrorHandler(const ErrorHandlerPtr pErrorHandler) = 0;
    virtual void setFileColumnNumber(const size_t fileColumnNumber) = 0;
    virtual void setRawInputData(const std::string_view rawInputData) = 0;

    virtual Core::Index_t getCurrentPosition() const = 0;
    virtual size_t getFileRowNumber() const = 0;
    virtual size_t getFileColumnNumber() const = 0;
};

#endif // ISUBPARSER_H
