#include "errorsettingsproxy.h"
#include "errortypes.h"
#include "errorcodes.h"
#include "applicationdata.h"
#include "parserdata.h"
#include "deviceportdata.h"

EmptyCellError::EmptyCellError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{static_cast<Error_t>(ErrorCode::EMPTY_CELL), fileRowNumber, fileColumnNumber, errorStream}
{
}

void EmptyCellError::execute()
{
    _logMessage("a CSV cell is empty.", true);
    _logRowAndColumnNumber();

    Error::execute();
}

UnknownDeviceError::UnknownDeviceError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{static_cast<Error_t>(ErrorCode::UNKNOWN_DEVICE), fileRowNumber, fileColumnNumber, errorStream}
{
}

void UnknownDeviceError::execute()
{
    _logMessage("an unknown or currently not supported device type has been entered.", true);
    _logRowAndColumnNumber();

    Error::execute();
}

FewerCellsError::FewerCellsError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream &errorStream)
    : Error{static_cast<Error_t>(ErrorCode::FEWER_CELLS), fileRowNumber, fileColumnNumber, errorStream}
{
}

void FewerCellsError::execute()
{
    const size_t c_RequiredCellsCountPerRow{Data::c_NrOfCablesPerConnectionInputRow +
                                            Data::c_NrOfPortsPerConnectionInputRow *
                                           (Data::c_NrOfDeviceTypeAndUPositionCellsPerPort +
                                            Data::c_MaxPortInputParametersCount)};

    _logMessage("less cells have been filled on the row than required in order to store the parameters of the two device ports.", true);
    _logMessage("A total number of " + std::to_string(c_RequiredCellsCountPerRow) + " contiguous cells are required to be filled (starting with the first cell on the row)");
    _logRowNumber();

    Error::execute();
}

InvalidConnectionFormatError::InvalidConnectionFormatError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{static_cast<Error_t>(ErrorCode::INVALID_CONNECTION_FORMAT), fileRowNumber, fileColumnNumber, errorStream}
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

DeviceUPositionOutOfRangeError::DeviceUPositionOutOfRangeError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{static_cast<Error_t>(ErrorCode::DEVICE_U_POSITION_OUT_OF_RANGE), fileRowNumber, fileColumnNumber, errorStream}
{
}

void DeviceUPositionOutOfRangeError::execute()
{
    _logMessage("U number of the device is out of range.", true);
    _logMessage("Each device should be placed between " + std::to_string(Data::c_FirstRackUPositionNumber) + "U and " + std::to_string(Data::c_MaxRackUnitsCount) + "U.");
    _logRowAndColumnNumber();

    Error::execute();
}

TargetDeviceNotFoundError::TargetDeviceNotFoundError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{static_cast<Error_t>(ErrorCode::TARGET_DEVICE_NOT_FOUND), fileRowNumber, fileColumnNumber, errorStream}
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

DeviceConnectedToItselfError::DeviceConnectedToItselfError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{static_cast<Error_t>(ErrorCode::DEVICE_CONNECTED_TO_ITSELF), fileRowNumber, fileColumnNumber, errorStream}
{
}

void DeviceConnectedToItselfError::execute()
{
    _logMessage("device is connected to itself. This is not allowed.", true);
    _logRowAndColumnNumber();

    Error::execute();
}

NullNrOfConnectionsError::NullNrOfConnectionsError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{static_cast<Error_t>(ErrorCode::NULL_NR_OF_CONNECTIONS), fileRowNumber, fileColumnNumber, errorStream}
{
}

void NullNrOfConnectionsError::execute()
{
    _logMessage("number of connections between the two devices is 0.", true);
    _logRowAndColumnNumber();

    Error::execute();
}


InvalidCharactersError::InvalidCharactersError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{static_cast<Error_t>(ErrorCode::INVALID_CHARACTERS), fileRowNumber, fileColumnNumber, errorStream}
{
}

void InvalidCharactersError::execute()
{
    _logMessage("the cell contains invalid characters.", true);
    _logMessage("Only \'-\', alphabet and numeric characters are allowed.");
    _logRowAndColumnNumber();

    Error::execute();
}

InvalidUPositionValueError::InvalidUPositionValueError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{static_cast<Error_t>(ErrorCode::INVALID_U_POSITION_VALUE), fileRowNumber, fileColumnNumber, errorStream}
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

EmptyConnectionInputFileError::EmptyConnectionInputFileError(std::ofstream& errorStream)
    : Error{static_cast<Error_t>(ErrorCode::EMPTY_CONNECTION_INPUT_FILE), 2u, 1u, errorStream}
{
}

void EmptyConnectionInputFileError::execute()
{
    _logMessage("the connections input file contains no data.", true);
    _logMessage("File: " + Settings::getConnectionInputFile().string());

    Error::execute();
}

NoConnectionsDefinedError::NoConnectionsDefinedError(std::ofstream& errorStream)
    : Error{static_cast<Error_t>(ErrorCode::NO_CONNECTIONS_DEFINED), 2u, 1u, errorStream}
{
}

void NoConnectionsDefinedError::execute()
{
    _logMessage("the connection definitions file contains no devices or no connections between them.", true);
    _logMessage("File: " + Settings::getConnectionDefinitionsFile().string());

    Error::execute();
}
