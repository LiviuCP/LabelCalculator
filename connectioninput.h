#ifndef CONNECTIONINPUT_H
#define CONNECTIONINPUT_H

#include <fstream>
#include <string>
#include <vector>

#include "device.h"

/* This function reads all rows from connectioninput.csv and caches them for further processing.
   The user should have previously filled in the placeholders with useful connection data.
*/
void readConnectionInput(std::ifstream& inputStream, std::vector<std::string>& connectionInputRows);

/* This function parses all rows read by previous function and generates intermediary data to be used for generating the labelling table
*/
bool parseConnectionInput(std::ofstream& errorStream,
                          std::vector<Device*>& devices,
                          std::vector<std::string>& cablePartNumbersEntries,
                          const std::vector<std::string>& connectionInputRows);

/* Builds the labelling table to be written into labellingtable.csv
*/
void buildLabellingOutput(std::vector<std::string>& outputRows,
                          const std::vector<Device*>& devices,
                          const std::vector<std::string>& cablePartNumbersEntries);

/* This function creates an entry for a connection between two devices.
   String is written to labellingtable.csv in a subsequent operation.
*/
void buildConnectionEntry(std::string& entry,
                          int& entryNumber,
                          const Device* const pFirstDevice,
                          const Device* const pSecondDevice,
                          const std::string& cablePartNumber);

#endif // CONNECTIONINPUT_H
