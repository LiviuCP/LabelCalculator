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

    ParserPtr createParser(ParserTypes parserType, std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream);

    bool isParserAlreadyCreated() const;

private:
    ParserTypes mCreatedParserType;
};

#endif // PARSERCREATOR_H
