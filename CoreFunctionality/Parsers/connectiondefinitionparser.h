#ifndef CONNECTIONDEFINITIONPARSER_H
#define CONNECTIONDEFINITIONPARSER_H

#include <map>

#include "applicationdata.h"
#include "parser.h"

class ConnectionDefinitionParser final : public Parser
{
public:
    ConnectionDefinitionParser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream);

    ConnectionDefinitionParser() = delete;
    ConnectionDefinitionParser(const ConnectionDefinitionParser&) = delete;
    ConnectionDefinitionParser& operator=(const ConnectionDefinitionParser&) = delete;

protected:
    void _readPayload() override;
    bool _parseInput() override;
    void _buildOutput() override;

    void _reset() override;

private:
    /* This function passes through the first cell on each CSV row (rack U position).
       It discards the cell content.
    */
    void _parseUPosition(const size_t rowIndex);

    /* This function is used for parsing the device type (second CSV cell).
       There are 3 possible cases:
        - empty cell (no device): stop CSV row parsing
        - invalid device type: stop CSV row parsing
        - valid device type: continue checking the upper U position devices to which it is connected
    */
    bool _parseDeviceType(const size_t rowIndex);

    /* This function parses the devices connected to the device from current row (if its type is valid).
       Each connected device should be on a higher U position comparing to position contained by current row (connections are considered bottom-up).
    */
    void _parseRowConnections(const size_t rowIndex);

    /* This function checks the format of the connection of the first device from each row of the connectiondefinitions.csv file to another device from the row.
       If the format is correct (true returned) it will also fill in the two arguments with the U placement of the other device and the number of connections between the two.
    */
    static bool _parseConnectionFormatting(const std::string& source, Data::UNumber_t& secondDevice, size_t& connectionsCount);

    /* This function creates the template parameters (including device type and U position) for each device.
       These parameters are being filled in into output file (connection input file) for each entry where the device is connected to another one.
    */
    void _buildTemplateDeviceParameters();

    /* Stores devices contained in the rack.
       For each device the type will be memorized at the index representing the lowest U position occupied within rack.
       The vector will have 50 elements (maximum rack size).
    */
    std::vector<Data::DeviceTypeID> mMapping;

    /* Stores the U positions (lowest in rack, e.g. U5 for a device occupying U5-10) of all discovered devices (in the order of their discovery).
       Storing occurs in decreasing order of their appearance (first device is the one from the highest U position).
    */
    std::vector<Data::UNumber_t> mUNumbers;

    /* stores the devices to which each device from a csv line connects */
    std::vector<std::vector<Data::UNumber_t>> mConnectedTo;

    /* stores the number of connections to each device mentioned in previous vector */
    std::vector<std::vector<size_t>> mConnectionsCount;

    /* stores the device parameters for each device (including device type and U position)
       which will be used for creating the connection input file template
    */
    std::vector<std::string> mTemplateDeviceParameters;
};

#endif // CONNECTIONDEFINITIONPARSER_H
