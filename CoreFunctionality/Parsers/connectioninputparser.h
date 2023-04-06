#ifndef CONNECTIONINPUTPARSER_H
#define CONNECTIONINPUTPARSER_H

#include <memory>

#include "deviceportsfactory.h"
#include "deviceport.h"
#include "parser.h"

class ConnectionInputParser final : public Parser
{
public:
    ConnectionInputParser(const InputStreamPtr pInputStream, const OutputStreamPtr pOutputStream, const ErrorStreamPtr pErrorStream);

    ConnectionInputParser() = delete;
    ConnectionInputParser(const ConnectionInputParser&) = delete;
    ConnectionInputParser& operator=(const ConnectionInputParser&) = delete;

protected:
    virtual void _readPayload() override;
    virtual bool _parseInput() override;
    virtual void _buildOutput() override;

    virtual void _reset() override;

private:
    /* Used for processing the cable PN cell for each CSV row
    */
    void _parseCablePartNumber(const size_t rowIndex);

    /* Used for parsing the data for one of the device ports contained in each CSV connection input row
    */
    bool _parseDevicePort(const size_t rowIndex);

    /* This function creates an entry for a connection between two devices.
       String is written to the labelling table in a subsequent operation.
    */
    bool _buildOutputRow(const size_t rowIndex, std::string& currentRow);

    /* the cable part number of each connection to be stored here */
    std::vector<std::string> mCablePartNumbersEntries;

    /* number of devices still not parsed on current row */
    int mRowPortsStillNotParsedCount;

    /* stores the cable part number determined for previous row */
    std::string mCurrentCablePartNumber;

    /* device port factory used for creating the device port objects for each connection */
    std::unique_ptr<DevicePortsFactory> mpDevicePortsFactory;
};

#endif // CONNECTIONINPUTPARSER_H
