#ifndef IPARSER_H
#define IPARSER_H

class ISubParser;

class IParser
{
public:
    /* This function is used by sub-parser for notifying the parser that the parsing work has been finished (parser will then take appropriate actions)
    */
    virtual void subParserFinished(ISubParser* const pISubParser) = 0;
};

#endif // IPARSER_H
