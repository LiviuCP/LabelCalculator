#ifndef CONNECTIONINPUTPARSER_H
#define CONNECTIONINPUTPARSER_H

#include <memory>

#include "deviceportsfactory.h"
#include "deviceport.h"
#include "parser.h"

class ConnectionInputParser final : public Parser
{
public:
    ConnectionInputParser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream);

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
    Index_t _parseCablePartNumber(const size_t rowIndex, const Index_t currentPosition);

    /* Used for parsing the data for one of the device ports contained in each CSV connection input row
    */
    bool _parseDevicePort(const size_t rowIndex);

    /* Used for storing externally generated errors (from Device class and its subclasses)
       It is assumed that the error location (row and column) has been correctly setup at error creation
    */
    void _storeMultipleParsingErrors(const std::vector<ErrorPtr>& parsingErrors);

    /* This function creates an entry for a connection between two devices.
       String is written to the labelling table in a subsequent operation.
    */
    static size_t _buildConnectionEntry(const size_t currentEntryNumber,
                                        const std::string_view cablePartNumber,
                                        const DevicePortPtr pFirstDevicePort,
                                        const DevicePortPtr pSecondDevicePort,
                                        std::string& currentEntry);

    /* all created DevicePort objects used for calculating labels and descriptions */
    std::vector<DevicePortPtr> mDevicePorts;

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
