#ifndef CONNECTIONDEFINITIONPARSER_H
#define CONNECTIONDEFINITIONPARSER_H

#include <map>

#include "labelutils.h"
#include "parser.h"

class ConnectionDefinitionParser final : public Parser
{
public:
    ConnectionDefinitionParser(std::ifstream* const pInputStream, std::ofstream* const pOutputStream, std::ofstream* const pErrorStream);

    ConnectionDefinitionParser() = delete;
    ConnectionDefinitionParser(const ConnectionDefinitionParser&) = delete;
    ConnectionDefinitionParser& operator=(const ConnectionDefinitionParser&) = delete;

protected:
    void _readInput() override;
    bool _parseInput() override;
    void _buildOutput() override;

    void _reset() override;

private:
    /* This function checks the format of the connection of the first device from each row of the connectiondefinitions.csv file to another device from the row
       If the format is correct (true returned) it will also fill in the two arguments with the U placement of the other device and the number of connections between the two
    */
    static bool _parseConnectionFormatting(const std::string& source, int& secondDevice, int& connectionsCount);

    /* Stores devices contained in the rack.
       For each device the type will be memorized at the index representing the lowest U position occupied within rack.
       The vector will have 50 elements (maximum rack size).
    */
    std::vector<DeviceTypeID> mMapping;

    /* Stores the U positions (lowest in rack, e.g. U5 for a device occupying U5-10) of all discovered devices (in the order of their discovery)
       Storing occurs in decreasing order of their appearance (first device is the one from the highest U position)
    */
    std::vector<int> mUNumbers;

    /* Stores the devices to which each device from a csv line connects */
    std::vector<std::vector<int>> mConnectedTo;

    /* stores the number of connections to each device mentioned in previous vector */
    std::vector<std::vector<int>> mConnectionsCount;

    /* total number of actual physical devices discovered within rack during the parsing process */
    int mDiscoveredDevicesCount;

    /* placeholders used for creating the connection input template file (should be filled in by user in next step) */
    static const std::map<DeviceTypeID, std::string> scConnectionInputPlaceholders;
};

#endif // CONNECTIONDEFINITIONPARSER_H
