#ifndef CONNECTIONINPUTPARSER_H
#define CONNECTIONINPUTPARSER_H

#include "parser.h"
#include "device.h"

class ConnectionInputParser : public Parser
{
public:
    ConnectionInputParser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream);

    ConnectionInputParser() = delete;
    ConnectionInputParser(const ConnectionInputParser&) = delete;
    ConnectionInputParser& operator=(const ConnectionInputParser&) = delete;

    ~ConnectionInputParser();

protected:
    virtual void _readInput() override;
    virtual bool _parseInput() override;
    virtual void _buildOutput() override;

    virtual void _reset() override;

    /* This function creates an entry for a connection between two devices.
       String is written to the labelling table in a subsequent operation.
    */
    static void _buildConnectionEntry(std::string& entry,
                                      int& entryNumber,
                                      const Device* const pFirstDevice,
                                      const Device* const pSecondDevice,
                                      const std::string& cablePartNumber);

    /* all created Device objects (except the ones used for checking device validity which are destroyed immediately) */
    std::vector<Device*> mDevices;

    /* the cable part number of each connection to be stored here */
    std::vector<std::string> mCablePartNumbersEntries;
};

#endif // CONNECTIONINPUTPARSER_H
