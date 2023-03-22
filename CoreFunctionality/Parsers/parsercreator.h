#ifndef PARSERCREATOR_H
#define PARSERCREATOR_H

#include <fstream>

#include "parser.h"

class ParserCreator
{
public:
    enum class ParserTypes
    {
        CONNECTION_DEFINITION,
        CONNECTION_INPUT,
        UNKNOWN
    };

    ParserCreator();

    ParserCreator(const ParserCreator& parserCreator) = delete;
    ParserCreator& operator=(const ParserCreator&) = delete;

    ParserPtr createParser(const ParserTypes parserType, const InputStreamPtr pInputStream, const OutputStreamPtr pOutputStream, const ErrorStreamPtr pErrorStream);

    bool isParserAlreadyCreated() const;

private:
    ParserTypes mCreatedParserType;
};

#endif // PARSERCREATOR_H
