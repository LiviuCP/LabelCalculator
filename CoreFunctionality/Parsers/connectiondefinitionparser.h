#ifndef CONNECTIONDEFINITIONPARSER_H
#define CONNECTIONDEFINITIONPARSER_H

#include <string_view>

#include "applicationdata.h"
#include "parser.h"

class ConnectionDefinitionParser final : public Parser
{
public:
    ConnectionDefinitionParser(const InputStreamPtr pInputStream, const OutputStreamPtr pOutputStream, const ErrorStreamPtr pErrorStream);

    ConnectionDefinitionParser() = delete;
    ConnectionDefinitionParser(const ConnectionDefinitionParser&) = delete;
    ConnectionDefinitionParser& operator=(const ConnectionDefinitionParser&) = delete;

protected:
    void _readPayload() override;
    bool _parseInput() override;
    void _buildOutput() override;

    void _reset() override;

private:
    /* Describes connection of current (source) device to a destination device (a.k.a. connected device);
       - first: connected device U number
       - second: number of connections from source to destination device
    */
    using ConnectedDevice = std::pair<Data::UNumber_t, size_t>;

    /* All destination devices to which a (source) device defined at a specific U position connects (bottom-up connections)
    */
    using ConnectedDevices = std::vector<ConnectedDevice>;

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
    void _parseDeviceConnections(const size_t rowIndex);

    /* This function checks the format of the connection of the first device from each row of the connectiondefinitions.csv file to another device from the row.
       If the format is correct (true returned) it will also fill in the two arguments with the U placement of the other device and the number of connections between the two.
    */
    static bool _parseConnectionFormatting(const std::string_view source, ConnectedDevice& connectedDevice);

    /* This function computes the output data for each device. This includes template parameters to be subsequently filled-in by user (in next step, in order to get the labelling table calculated).
       This data is being appended to connection input file for each entry where the device is connected to another one.
    */
    void _buildDeviceOutputData();

    struct DeviceData
    {
        DeviceData();

        Data::DeviceTypeID mDeviceTypeID;
        std::string mDeviceOutputData; // data to be written into csv file for the device wherever there is a connection between it and another device
    };

    struct DeviceConnections
    {
        DeviceConnections() = delete;
        DeviceConnections(Data::UNumber_t sourceDevice);

        Data::UNumber_t mSourceDevice; // source device
        ConnectedDevices mConnectedDevices; // destination devices to which the source connects (all of them on higher U positions than source)
    };

    /* Stores data for the devices contained within rack:
       - device type ID
       - a string containing the computed output data for the device (including device type as string, U position, other (template) parameters that should later be filled-in by user)
       Each array element is mapped to a rack U position (e.g. element at index 3 to rack position U4) so the array size equals the maximum number of units within a rack.
       For each device only the lowest U position should be marked within array (all other positions are marked "empty" (a.k.a. NO_DEVICE) although they might actually be occupied by physical devices).
    */
    std::vector<DeviceData> mRackPositionToDeviceDataMapping;

    /* Stores the U positions (lowest in rack, e.g. U5 for a device occupying U5-10) of all discovered devices (in the order of their discovery).
       Storing occurs in decreasing order of their appearance (first device is the one from the highest U position).
       Also for each stored (source) device the (destination) devices to which it connects and the number of connections to each destination device are memorized.
    */
    std::vector<DeviceConnections> mConnections;
};

#endif // CONNECTIONDEFINITIONPARSER_H
