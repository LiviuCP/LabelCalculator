#ifndef PARSERCREATOR_H
#define PARSERCREATOR_H

#include <fstream>

class Parser;

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

    Parser* createParser(ParserTypes parserType, std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream);

    bool isParserAlreadyCreated() const;

private:
    ParserTypes mCreatedParserType;
};

#endif // PARSERCREATOR_H
