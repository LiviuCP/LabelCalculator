#include "connectiondefinitionparser.h"
#include "connectioninputparser.h"
#include "parsercreator.h"

ParserCreator::ParserCreator()
    : mCreatedParserType{ParserTypes::UNKNOWN}
{
}

ParserPtr ParserCreator::createParser(ParserCreator::ParserTypes parserType, std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream)
{
    ParserPtr pCreatedParser{nullptr};

    if (ParserTypes::UNKNOWN == mCreatedParserType) // the parser creator should create exactly ONE parser
    {
        if (ParserTypes::CONNECTION_DEFINITION == parserType)
        {
            pCreatedParser = std::make_unique<ConnectionDefinitionParser>(pInputStream, pOutputStream, pErrorStream);
        }
        else if (ParserTypes::CONNECTION_INPUT == parserType)
        {
            pCreatedParser = std::make_unique<ConnectionInputParser>(pInputStream, pOutputStream, pErrorStream);
        }
        else
        {
            // no action, defensive programming
        }
    }

    return pCreatedParser;
}

bool ParserCreator::isParserAlreadyCreated() const
{
    const bool c_IsParserCreated{ParserTypes::UNKNOWN != mCreatedParserType};
    return c_IsParserCreated;
}
