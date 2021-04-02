#ifndef CONNECTIONINPUTPARSER_H
#define CONNECTIONINPUTPARSER_H

#include "device.h"
#include "parser.h"

class ConnectionInputParser final : public Parser
{
public:
    ConnectionInputParser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream);

    ConnectionInputParser() = delete;
    ConnectionInputParser(const ConnectionInputParser&) = delete;
    ConnectionInputParser& operator=(const ConnectionInputParser&) = delete;

protected:
    virtual void _readInput() override;
    virtual bool _parseInput() override;
    virtual void _buildOutput() override;

    virtual void _reset() override;

private:
    /* Used for storing externally generated errors (from Device class and its subclasses)
       It is assumed that the error location (row and column) has been correctly setup at error creation
    */
    bool _storeExternalParsingErrors(const std::vector<ErrorPtr>& deviceParsingErrors);

    /* This function creates an entry for a connection between two devices.
       String is written to the labelling table in a subsequent operation.
    */
    static void _buildConnectionEntry(std::string& entry,
                                      int& entryNumber,
                                      const DevicePtr pFirstDevice,
                                      const DevicePtr pSecondDevice,
                                      const std::string& cablePartNumber);

    /* all created Device objects (except the ones used for checking device validity which are destroyed immediately) */
    std::vector<DevicePtr> mDevices;

    /* the cable part number of each connection to be stored here */
    std::vector<std::string> mCablePartNumbersEntries;

    /* number of devices still not parsed on current row */
    int mRowDevicesStillNotParsedCount;
};

#endif // CONNECTIONINPUTPARSER_H
