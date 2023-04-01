#ifndef ISUBPARSEROBSERVER_H
#define ISUBPARSEROBSERVER_H

class ISubParser;

class ISubParserObserver
{
public:
    virtual ~ISubParserObserver() {};

    /* This function is used by sub-parser for notifying the parser that the parsing work has been finished (parser will then take appropriate actions)
    */
    virtual void subParserFinished(ISubParser* const pISubParser) = 0;
};

#endif // ISUBPARSEROBSERVER_H
