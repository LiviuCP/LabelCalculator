#ifndef DEVICEPORT_H
#define DEVICEPORT_H

#include <vector>
#include <utility>

#include "isubparser.h"

#ifdef _WIN32
#include "auxdata.h"
#endif

class ISubParserObserver;

class DevicePort : public ISubParser
{
public:
    DevicePort(const std::string_view deviceUPosition,
               const size_t fileRowNumber,
               const bool isSourceDevice,
               const bool parseFromRowStart = false);

    virtual ~DevicePort();

    /* Initialization function that fulfills two purposes:
      - initialize description and label
      - register required parameters
    */
    virtual void init() override;

    /* Updates the description and label fields for each device port
       These two had been previously initialized by the base class (e.g. by writing the rack U position of the device)
       The updated data will subsequently be used for building the final labelling table
    */
    virtual void updateDescriptionAndLabel() = 0;

    /* reads and parses the input fields for the device port from string starting position pos (which is subsequently updated)
       uses the ofstream for logging any errors in the corresponding file and the boolean to report the occurence of these errors
    */
    virtual void parseInputData(std::vector<ErrorPtr>& parsingErrors) override;

    // getters
    virtual Core::Index_t getCurrentPosition() const override final;
    virtual size_t getFileRowNumber() const override final;
    virtual size_t getFileColumnNumber() const override final;
    std::string getDescription() const;
    std::string getLabel() const;

    // setters
    virtual void setSubParserObserver(ISubParserObserver* const pISubParserObserver) override final;
    virtual void setErrorHandler(const ErrorHandlerPtr pErrorHandler) override final;
    virtual void setFileColumnNumber(const size_t fileColumnNumber) override final;
    virtual void setRawInputData(const std::string_view rawInputData) override final;

protected:
    /* Registers the required parameters; these are subsequently requested (parsed) from the connection input file:
       - the registration order should match the order in which parameters are given within file for the specific device port
       - the number of registered parameters should match the mInputParametersCount
       - needs to be implemented by derived classes, each class having its own specifics regarding required input parameters
    */
    virtual void _registerRequiredParameters() = 0;

    /* Registers each required input parameter so it can be subsequently requested from the connection input file (used by the _registerRequiredParameters() method)
    */
    void _registerRequiredParameter(std::string* const pRequiredParameter);

    /* This function is used for appending data calculated based on the parsed input parameters
       (other than the ones used for initializing description and label) to device port description
    */
    void _appendDataToDescription(const std::string_view data);

    /* This function is used for appending data calculated based on the parsed input parameters
       (other than the ones used for initializing description and label) to device port label
    */
    void _appendDataToLabel(const std::string_view data);

    /* This function is used for replacing port description and label with a "soft" parsing error message
       "Hard" parsing errors are being logged within error file by using the error handling functionality (see ErrorHandling directory)
       If no label argument is entered the default invalid label message is filled-in
    */
    void _setInvalidDescriptionAndLabel(const std::string_view descriptionInput, const std::string_view labelInput = "");

    /* This function performs a final check on the calculated label and initiates a size check if no "soft" parsing errors occurred
    */
    void _checkLabel();

    /* Retrieves the number of input parameters required for each device port. Each port is responsible for determining the number of required params
    */
    virtual size_t _getInputParametersCount() const = 0;

    /* Required for initializing the device port description and label
    */
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const;

    /* Checks if current position can be used in the parsing process (should either be null or within raw data bounds - including end position)
    */
    bool _isCurrentPositionAllowed() const;

private:
    /*This function is used for initializing the input parameters for each device: determine their count, initiate registration process
    */
    void _initializeRequiredParameters();

    /* This function is used for setting initial data (device U position, device type) into device port description and label
    */
    void _initializeDescriptionAndLabel();

    // string passed to the device port object for parsing; the device port might parse part or the whole content (padding prefix data might also be required)
    std::string mRawInputData;

    // position of the device containing the port in rack
    std::string mDeviceUPosition;

    // text to be written in the source (first device) / destination (second device) field of the labelling table (device port description)
    std::string mDescription;

    // text to be written in the source (first device) / destination (second device) field of the labelling table (device port label)
    std::string mLabel;

    // input CSV file row from which the device info is being parsed, used for error reporting
    size_t mFileRowNumber;

    // current CSV file column from which device info is being parsed, used for error reporting
    size_t mFileColumnNumber;

    // current character index in the currently parsed CSV row string
    Core::Index_t mCurrentPosition;

    // reference to substrings storing the fields parsed by parseInputData()
    std::vector<std::string*> mInputData;

    // total number of parameters that should be entered by user in the CSV file for the device port (including port number)
    size_t mInputParametersCount;

    // true if first of the two devices in each connection entered in the input .csv file
    bool mIsSourceDevice;

    // initialization flag, ensures label/description are initialized and required input parameters are registered
    bool mIsInitialized;

    // error handler (should be received from parent parser)
    std::shared_ptr<ErrorHandler> mpErrorHandler;

    // parent parser (used for passing data via callback after subparser finishes the parsing job)
    ISubParserObserver* m_pISubParserObserver;

    // flag that signals that parsing begins from start of row (if false: some padding needs to be prepended to raw input data, otherwise first cell is always considered empty)
    const bool mParseFromRowStart;
};

#endif // DEVICEPORT_H
