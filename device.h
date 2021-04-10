#ifndef DEVICE_H
#define DEVICE_H

#include <fstream>
#include <vector>
#include <memory>

#include "labelutils.h"
#include "error.h"

class Device
{
public:
    Device(const std::string& deviceType, int requiredNumberOfParameters, int maxAllowedNrOfChars, bool isSourceDevice);

    virtual ~Device();

    /* Calculates the description and label field for each device
       This data will be subsequently used for building the final table
    */
    virtual void computeDescriptionAndLabel() = 0;

    /* reads and parses the input fields for the device from string starting position pos (which is subsequently updated)
       uses the ofstream for logging any errors in the corresponding file and the boolean to report the occurence of these errors
    */
    int parseInputData(const std::string &s, const int initialPosition, std::vector<ErrorPtr>& parsingErrors, std::ofstream& errorStream);

    // appends the description and label to the string
    void writeDescriptionAndLabel(std::string& out) const;

    // setters for the input .csv row and column index
    void setRow(int row);
    void setColumn(int column);

    // getters
    int getRow() const;
    int getColumn() const;

protected:
    void _registerRequiredParameter(std::string* const pRequiredParameter);

    std::string mDeviceName;  // device name (e.g. for PDU-A the name is "A")
    std::string mDeviceType;  // device type (ex: "pdu" for basic PDUs without load segments)
    std::string mPortNumber;  // power or data port number that should be mentioned on the cable label

    std::string mDescription; // text to be written in the source (first device) / destination (second device) field of the labelling table (device description)
    std::string mLabel;       // text to be written in the source (first device) / destination (second device) field of the labelling table (device label)

    int mRow;            // input .csv row from which the error originated
    int mColumn;         // input .csv column from which the error originated

    int mMaxAllowedNrOfChars; // maximum number of characters that can be filled in the connectioninput.csv file for a specific device type (including any placeholders where character '-' could be filled in)
    int mDeltaNrOfChars; // number of characters above the maximum number (mMaxAllowedNrOfChars)
    bool mIsSourceDevice; // true if first of the two devices in each connection entered in the input .csv file

    static const int scMaxInputParametersCount; // number of fields that should be filled in connectioninput.csv for EACH device (not used fields can be filled in with '-')

private:
    std::vector<std::string*> mInputData; // reference to substrings storing the fields parsed by parseInputData()
    int mInputParametersCount;
};

using DevicePtr = std::shared_ptr<Device>;

#endif // DEVICE_H
