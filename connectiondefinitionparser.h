#ifndef CONNECTIONDEFINITIONPARSER_H
#define CONNECTIONDEFINITIONPARSER_H

#include <fstream>
#include <string>
#include <vector>
#include <map>

class ConnectionDefinitionParser
{
public:
    ConnectionDefinitionParser(std::ifstream& inputStream, std::ofstream& outputStream, std::ofstream& errorStream);

    ConnectionDefinitionParser() = delete;
    ConnectionDefinitionParser(const ConnectionDefinitionParser&) = delete;
    ConnectionDefinitionParser& operator=(const ConnectionDefinitionParser&) = delete;

    bool parse();

private:
    /* This function reads all rows from connectiondefinitions.csv and caches them for further processing
       Maximum 50 lines are read from connection definition file (the rack can have maximum 50U)
    */
    void _readConnectionDefinitions();

    /* This function parses all rows read by previous function and generates intermediary data to be used for generating the connections input
    */
    bool _parseConnectionDefinitions();

    /* Builds the connection input (including placeholders) to be written into connectioninput.csv
       The user will have to fill in the placeholder and then run the application with option 2 for getting the labelling table
    */
    void _buildConnectionsInputTemplate();

    /* Resets the internal state after each parsing session
    */
    void _reset();

    /* This function checks the format of the connection of the first device from each row of the connectiondefinitions.csv file to another device from the row
       If the format is correct (true returned) it will also fill in the two arguments with the U placement of the other device and the number of connections between the two
    */
    static bool _parseConnectionFormatting(const std::string& source, int& secondDevice, int& connectionsCount);

    /* file streams used by parser, each one should correspond to a file that had been previously correctly opened */
    std::ifstream& mInputStream;
    std::ofstream& mOutputStream;
    std::ofstream& mErrorStream;

    /* "raw" input read from connection definitions file */
    std::vector<std::string> mConnectionDefinitionRows;

    /* Stores devices contained in the rack.
       For each device the type will be memorized at the index representing the lowest U position occupied within rack.
       The vector will have 50 elements (maximum rack size).
    */
    std::vector<std::string> mMapping;

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

    /* resulting output to be written in the connection input file */
    std::vector<std::string> mConnectionInputRows;

    /* reset is required after first usage of the parser */
    bool mIsResetRequired;

    /* placeholders used for creating the connection input template file (should be filled in by user in next step) */
    static const std::map<std::string, std::string> scConnectionInputPlaceholders;
};

#endif // CONNECTIONDEFINITIONPARSER_H
