#ifndef DEVICEPORT_H
#define DEVICEPORT_H

#include <fstream>
#include <vector>
#include <memory>

#include "errorhandler.h"

class DevicePort
{
public:
    DevicePort(const std::string& deviceUPosition,
               const size_t fileRowNumber,
               const size_t fileColumnNumber,
               const size_t requiredNumberOfParameters,
               const bool isSourceDevice);

    virtual ~DevicePort();

    /* Calculates the description and label field for each device port
       This data will be subsequently used for building the final table
    */
    virtual void computeDescriptionAndLabel() = 0;

    /* reads and parses the input fields for the device port from string starting position pos (which is subsequently updated)
       uses the ofstream for logging any errors in the corresponding file and the boolean to report the occurence of these errors
    */
    ssize_t parseInputData(const std::string& input, const ssize_t initialPosition, ErrorHandler &errorHandler, std::ofstream& errorStream, std::vector<ErrorPtr>& parsingErrors);

    // getters
    size_t getFileRowNumber() const;
    size_t getFileColumnNumber() const;
    std::string getDescription() const;
    std::string getLabel() const;

protected:
    /* Registers the required parameter so it can be requested from the connection input file:
       - the registration order should match the order in which parameters are given in the file
       for the specific device port
       - the number of registered parameters should match the mInputParametersCount
    */
    void _registerRequiredParameter(std::string* const pRequiredParameter);

    /* This function performs a final check on the calculated label and initiates a size check if no "soft" parsing errors occurred
    */
    void _checkLabel();

    /* This function is used for replacing port description and label with a "soft" parsing error message
       "Hard" parsing errors are being logged within error file by using the error handling functionality (see ErrorHandling directory)
       If no label argument is entered the default invalid label message is filled-in
    */
    void _setInvalidDescriptionAndLabel(const std::string& description, const std::string& label = "");

    // position of the device containing the port in rack
    std::string mDeviceUPosition;

    // power or data port number that should be mentioned on the cable label
    std::string mPortNumber;

    // text to be written in the source (first device) / destination (second device) field of the labelling table (device port description)
    std::string mDescription;

    // text to be written in the source (first device) / destination (second device) field of the labelling table (device port label)
    std::string mLabel;

    // input CSV file row from which the device info is being parsed, used for error reporting
    size_t mFileRowNumber;

    // current CSV file column from which device info is being parsed, used for error reporting
    size_t mFileColumnNumber;

private:
    // reference to substrings storing the fields parsed by parseInputData()
    std::vector<std::string*> mInputData;

    // total number of parameters that should be entered by user in the CSV file for the device port (including port number)
    size_t mInputParametersCount;

    // true if first of the two devices in each connection entered in the input .csv file
    bool mIsSourceDevice;
};

using DevicePortPtr = std::shared_ptr<DevicePort>;

#endif // DEVICEPORT_H
