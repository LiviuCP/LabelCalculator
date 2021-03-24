#ifndef CONNECTIONINPUTPARSER_H
#define CONNECTIONINPUTPARSER_H

#include <fstream>
#include <string>
#include <vector>

#include "device.h"

class ConnectionInputParser
{
public:
    ConnectionInputParser(std::ifstream& inputStream, std::ofstream& outputStream, std::ofstream& errorStream);
    ConnectionInputParser() = delete;
    ConnectionInputParser(const ConnectionInputParser&) = delete;
    ConnectionInputParser& operator=(const ConnectionInputParser&) = delete;

    bool parse();

private:
    /* This function reads all rows from connectioninput.csv and caches them for further processing.
       The user should have previously filled in the placeholders with useful connection data.
    */
    void _readConnectionInput();

    /* This function parses all rows read by previous function and generates intermediary data to be used for generating the labelling table
    */
    bool _parseConnectionInput();

    /* Builds the labelling table to be written into labellingtable.csv
    */
    void _buildLabellingOutput();

    /* Resets the internal state after each parsing session
    */
    void _reset();

    /* This function creates an entry for a connection between two devices.
       String is written to labellingtable.csv in a subsequent operation.
    */
    static void _buildConnectionEntry(std::string& entry,
                                      int& entryNumber,
                                      const Device* const pFirstDevice,
                                      const Device* const pSecondDevice,
                                      const std::string& cablePartNumber);

    /* file streams used by parser, each one should correspond to a file that had been previously correctly opened */
    std::ifstream& mInputStream;
    std::ofstream& mOutputStream;
    std::ofstream& mErrorStream;

    /* stores rows read from connection input file */
    std::vector<std::string> mConnectionInputRows;

    /* all created Device objects (except the ones used for checking device validity which are destroyed immediately) */
    std::vector<Device*> mDevices;

    /* the cable part number of each connection to be stored here */
    std::vector<std::string> mCablePartNumbersEntries;

    /* stores rows to be written into labelling table */
    std::vector<std::string> mLabellingTableRows;

    /* reset is required after first usage of the parser */
    bool mIsResetRequired;
};

#endif // CONNECTIONINPUTPARSER_H
