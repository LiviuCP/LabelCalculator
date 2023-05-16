#ifndef PARSERCREATOR_H
#define PARSERCREATOR_H

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

    ParserPtr createParser(const ParserTypes parserType, const Core::InputStreamPtr pInputStream, const Core::OutputStreamPtr pOutputStream, const Core::ErrorStreamPtr pErrorStream);

    bool isParserAlreadyCreated() const;

private:
    ParserTypes mCreatedParserType;
};

#endif // PARSERCREATOR_H
