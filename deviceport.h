#ifndef DEVICEPORT_H
#define DEVICEPORT_H

#include <fstream>
#include <vector>
#include <memory>

#include "labelutils.h"
#include "error.h"

class DevicePort
{
public:
    DevicePort(int requiredNumberOfParameters, int maxAllowedCharsCount, bool isSourceDevice);

    virtual ~DevicePort();

    /* Calculates the description and label field for each device port
       This data will be subsequently used for building the final table
    */
    virtual void computeDescriptionAndLabel() = 0;

    /* reads and parses the input fields for the device port from string starting position pos (which is subsequently updated)
       uses the ofstream for logging any errors in the corresponding file and the boolean to report the occurence of these errors
    */
    int parseInputData(const std::string& input, const int initialPosition, std::vector<ErrorPtr>& parsingErrors, std::ofstream& errorStream);

    // setters for the input .csv row and column index
    void setRow(int row);
    void setColumn(int column);

    // getters
    int getRow() const;
    int getColumn() const;
    std::string getDescription() const;
    std::string getLabel() const;

protected:
    /* Registers the required parameter so it can be requested from the connection input file:
       - the registration order should match the order in which parameters are given in the file
       for the specific device port
       - the number of registered parameters should match the mInputParametersCount
    */
    void _registerRequiredParameter(std::string* const pRequiredParameter);

    std::string mDeviceUPosition;  // position of the device containing the port in rack
    std::string mPortNumber;  // power or data port number that should be mentioned on the cable label

    std::string mDescription; // text to be written in the source (first device) / destination (second device) field of the labelling table (device port description)
    std::string mLabel;       // text to be written in the source (first device) / destination (second device) field of the labelling table (device port label)

    int mRow;            // input .csv row from which the error originated
    int mColumn;         // input .csv column from which the error originated

    int mMaxAllowedCharsCount; // maximum number of characters that can be filled in the connectioninput.csv file for a specific device type (including any placeholders where character '-' could be filled in)
    bool mIsSourceDevice; // true if first of the two devices in each connection entered in the input .csv file

    static const int scMaxInputParametersCount; // number of fields that should be filled in connectioninput.csv for EACH device (not used fields can be filled in with '-')

private:
    std::vector<std::string*> mInputData; // reference to substrings storing the fields parsed by parseInputData()
    int mInputParametersCount;
};

using DevicePortPtr = std::shared_ptr<DevicePort>;

#endif // DEVICEPORT_H
