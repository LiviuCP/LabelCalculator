#include "errorsettingsproxy.h"
#include "errortypes.h"
#include "errorcodes.h"
#include "applicationdata.h"

EmptyCellError::EmptyCellError(const size_t fileRowNumber, const size_t fileColumnNumber, const Core::ErrorStreamPtr pErrorStream)
    : Error{static_cast<Core::Error_t>(ErrorCode::EMPTY_CELL), fileRowNumber, fileColumnNumber, pErrorStream}
{
}

void EmptyCellError::execute()
{
    _logMessage("a CSV cell is empty.", true);
    _logRowAndColumnNumber();

    Error::execute();
}

UnknownDeviceError::UnknownDeviceError(const size_t fileRowNumber, const size_t fileColumnNumber, const Core::ErrorStreamPtr pErrorStream)
    : Error{static_cast<Core::Error_t>(ErrorCode::UNKNOWN_DEVICE), fileRowNumber, fileColumnNumber, pErrorStream}
{
}

void UnknownDeviceError::execute()
{
    _logMessage("an unknown or currently not supported device type has been entered.", true);
    _logRowAndColumnNumber();

    Error::execute();
}

FewerCellsError::FewerCellsError(const size_t fileRowNumber, const size_t fileColumnNumber, const Core::ErrorStreamPtr pErrorStream)
    : Error{static_cast<Core::Error_t>(ErrorCode::FEWER_CELLS), fileRowNumber, fileColumnNumber, pErrorStream}
{
}

void FewerCellsError::execute()
{
    _logMessage("fewer cells exist on the row than required for storing the connection data of the two device ports.", true);
    _logMessage("Please ensure a minimum number of cells are available for filling-in:");
    _logMessage(" - cable part number");
    _logMessage(" - device type and U position of each connected port");
    _logMessage(" - required input parameters for each port depeding on device type");
    _logMessage(" - padding fields (for first device port) if the number of input parameters is less than the maximum allowed");
    _logMessage("Please check Documentation for more details and only use spreadsheet editors (avoid text file editors) to modify each csv file.");
    _logRowNumber();

    Error::execute();
}

InvalidConnectionFormatError::InvalidConnectionFormatError(const size_t fileRowNumber, const size_t fileColumnNumber, const Core::ErrorStreamPtr pErrorStream)
    : Error{static_cast<Core::Error_t>(ErrorCode::INVALID_CONNECTION_FORMAT), fileRowNumber, fileColumnNumber, pErrorStream}
{
}

void InvalidConnectionFormatError::execute()
{
    _logMessage("format of the connection is invalid.", true);
    _logMessage("Two positive integers should be entered, separated by slash.");
    _logMessage("No other character types are allowed.");
    _logRowAndColumnNumber();

    Error::execute();
}

DeviceUPositionOutOfRangeError::DeviceUPositionOutOfRangeError(const size_t fileRowNumber, const size_t fileColumnNumber, const Core::ErrorStreamPtr pErrorStream)
    : Error{static_cast<Core::Error_t>(ErrorCode::DEVICE_U_POSITION_OUT_OF_RANGE), fileRowNumber, fileColumnNumber, pErrorStream}
{
}

void DeviceUPositionOutOfRangeError::execute()
{
    _logMessage("U number of the device is out of range.", true);
    _logMessage("Each device should be placed between " + std::to_string(Data::c_FirstRackUPositionNumber) + "U and " + std::to_string(Data::c_MaxRackUnitsCount) + "U.");
    _logRowAndColumnNumber();

    Error::execute();
}

TargetDeviceNotFoundError::TargetDeviceNotFoundError(const size_t fileRowNumber, const size_t fileColumnNumber, const Core::ErrorStreamPtr pErrorStream)
    : Error{static_cast<Core::Error_t>(ErrorCode::TARGET_DEVICE_NOT_FOUND), fileRowNumber, fileColumnNumber, pErrorStream}
{
}

void TargetDeviceNotFoundError::execute()
{
    _logMessage("the target device has not been found.", true);
    _logMessage("Either no device is mounted within rack at the mentioned U position or the target position is lower than the one of the source device.");
    _logMessage("Please note that the connections should always be defined bottom-up.");
    _logRowAndColumnNumber();

    Error::execute();
}

DeviceConnectedToItselfError::DeviceConnectedToItselfError(const size_t fileRowNumber, const size_t fileColumnNumber, const Core::ErrorStreamPtr pErrorStream)
    : Error{static_cast<Core::Error_t>(ErrorCode::DEVICE_CONNECTED_TO_ITSELF), fileRowNumber, fileColumnNumber, pErrorStream}
{
}

void DeviceConnectedToItselfError::execute()
{
    _logMessage("device is connected to itself. This is not allowed.", true);
    _logRowAndColumnNumber();

    Error::execute();
}

NullNrOfConnectionsError::NullNrOfConnectionsError(const size_t fileRowNumber, const size_t fileColumnNumber, const Core::ErrorStreamPtr pErrorStream)
    : Error{static_cast<Core::Error_t>(ErrorCode::NULL_NR_OF_CONNECTIONS), fileRowNumber, fileColumnNumber, pErrorStream}
{
}

void NullNrOfConnectionsError::execute()
{
    _logMessage("number of connections between the two devices is 0.", true);
    _logRowAndColumnNumber();

    Error::execute();
}


InvalidCharactersError::InvalidCharactersError(const size_t fileRowNumber, const size_t fileColumnNumber, const Core::ErrorStreamPtr pErrorStream)
    : Error{static_cast<Core::Error_t>(ErrorCode::INVALID_CHARACTERS), fileRowNumber, fileColumnNumber, pErrorStream}
{
}

void InvalidCharactersError::execute()
{
    _logMessage("the cell contains invalid characters.", true);
    _logMessage("Only \'-\', alphabet and numeric characters are allowed.");
    _logRowAndColumnNumber();

    Error::execute();
}

InvalidUPositionValueError::InvalidUPositionValueError(const size_t fileRowNumber, const size_t fileColumnNumber, const Core::ErrorStreamPtr pErrorStream)
    : Error{static_cast<Core::Error_t>(ErrorCode::INVALID_U_POSITION_VALUE), fileRowNumber, fileColumnNumber, pErrorStream}
{
}

void InvalidUPositionValueError::execute()
{
    _logMessage("the device U position is invalid.", true);
    _logMessage("The cell is empty, contains non-numeric characters or the number is not within the required range.");
    _logMessage("A integer between " + std::to_string(Data::c_FirstRackUPositionNumber) + " and " + std::to_string(Data::c_MaxRackUnitsCount) + " should be entered.");
    _logRowAndColumnNumber();

    Error::execute();
}

EmptyConnectionInputFileError::EmptyConnectionInputFileError(const Core::ErrorStreamPtr pErrorStream)
    : Error{static_cast<Core::Error_t>(ErrorCode::EMPTY_CONNECTION_INPUT_FILE), 2u, 1u, pErrorStream}
{
}

void EmptyConnectionInputFileError::execute()
{
    _logMessage("the connections input file contains no data.", true);
    _logMessage("File: " + Settings::getConnectionInputFile().string());

    Error::execute();
}

NoConnectionsDefinedError::NoConnectionsDefinedError(const Core::ErrorStreamPtr pErrorStream)
    : Error{static_cast<Core::Error_t>(ErrorCode::NO_CONNECTIONS_DEFINED), 2u, 1u, pErrorStream}
{
}

void NoConnectionsDefinedError::execute()
{
    _logMessage("the connection definitions file contains no devices or no connections between them.", true);
    _logMessage("File: " + Settings::getConnectionDefinitionsFile().string());

    Error::execute();
}
