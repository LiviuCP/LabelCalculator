#ifndef CONNECTIONDEFINITION_H
#define CONNECTIONDEFINITION_H

#include <fstream>
#include <string>
#include <vector>

/* This function reads all rows from connectiondefinitions.csv and caches them for further processing
*/
void readConnectionDefinitions(std::ifstream& inputStream, std::vector<std::string>& connectionDefinitionRows);

/* This function parses all rows read by previous function and generates intermediary data to be used for generating the connections input
*/
bool parseConnectionDefinitions(std::ofstream& errorStream,
                                std::vector<std::string>& mapping,
                                std::vector<int>& uNumbers,
                                std::vector<std::vector<int>>& connectedTo,
                                std::vector<std::vector<int>>& connectionsCount,
                                int& devicesCount,
                                const std::vector<std::string>& connectionDefinitionRows);

/* Builds the connection input (including placeholders) to be written into connectioninput.csv
   The user will have to fill in the placeholder and then run the application with option 2 for getting the labelling table
*/
void buildConnectionsInputTemplate(std::vector<std::string>& outputRows,
                                   const std::vector<std::string>& mapping,
                                   const std::vector<int>& uNumbers,
                                   const std::vector<std::vector<int>>& connectedTo,
                                   const std::vector<std::vector<int>>& connectionsCount,
                                   const int devicesCount);

/* This function checks the format of the connection of the first device from each row of the connectiondefinitions.csv file to another device from the row
   If the format is correct (true returned) it will also fill in the two arguments with the U placement of the other device and the number of connections between the two
*/
bool parseConnectionFormatting(const std::string& source, int& secondDevice, int& connectionsCount);

/* This function fills in the placeholder information that will be written subsequently in connectioninput.csv along with the name and type of the device
*/
void createPlaceholders(const std::string& deviceType, std::string& dest);

#endif // CONNECTIONDEFINITION_H
