#include <cassert>

#include "errorutils.h"
#include "errortypes.h"

EmptyCellError::EmptyCellError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{ErrorCode::EMPTY_CELL, fileRowNumber, fileColumnNumber, errorStream}
{
}

void EmptyCellError::execute()
{
    mErrorStream << "Error: A cell is empty\n";
    mErrorStream << "Row number: " << mFileRowNumber << "    "<< "Column number: " << mFileColumnNumber << "\n";

    Error::execute();
}

UnknownDeviceError::UnknownDeviceError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{ErrorCode::UNKNOWN_DEVICE, fileRowNumber, fileColumnNumber, errorStream}
{
}

void UnknownDeviceError::execute()
{
    mErrorStream << "Error: an unknown or currently not supported device type has been entered\n";
    mErrorStream << "Row number: " << mFileRowNumber << "    " << "Column number: " << mFileColumnNumber << "\n";

    Error::execute();
}

FewerCellsError::FewerCellsError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream &errorStream)
    : Error{ErrorCode::FEWER_CELLS, fileRowNumber, fileColumnNumber, errorStream}
{
}

void FewerCellsError::execute()
{
    mErrorStream << "Error: less cells have been filled on the row than required in order to store the parameters of the 2 devices\n";
    mErrorStream << "A total number of 11 contiguous cells are required to be filled (starting with the first cell on the row)\n";
    mErrorStream << "Row number: " << mFileRowNumber << "\n";

    Error::execute();
}

WrongConnectionFormatError::WrongConnectionFormatError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{ErrorCode::WRONG_CONNECTION_FORMAT, fileRowNumber, fileColumnNumber, errorStream}
{
}

void WrongConnectionFormatError::execute()
{
    mErrorStream << "Error: format of the connection is wrong\n";
    mErrorStream << "Row number: " << mFileRowNumber << "    "<<"Column number: "<< mFileColumnNumber << "\n";

    Error::execute();
}

DeviceUPositionOutOfRangeError::DeviceUPositionOutOfRangeError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{ErrorCode::DEVICE_U_POSITION_OUT_OF_RANGE, fileRowNumber, fileColumnNumber, errorStream}
{
}

void DeviceUPositionOutOfRangeError::execute()
{
    mErrorStream << "Error: U number of the device is out of range\n";
    mErrorStream << "Each device should be placed between 1U and 50U";
    mErrorStream << "Row number: " << mFileRowNumber << "    " << "Column number: " << mFileColumnNumber << "\n";

    Error::execute();
}

TargetDeviceNotFoundError::TargetDeviceNotFoundError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{ErrorCode::TARGET_DEVICE_NOT_FOUND, fileRowNumber, fileColumnNumber, errorStream}
{
}

void TargetDeviceNotFoundError::execute()
{
    mErrorStream << "Error: the target device has not been found.\n";
    mErrorStream << "Either no device is mounted within rack at the mentioned U position or the target position is lower than the one of the source device.\n";
    mErrorStream << "Please note that the connections should always be defined bottom-up.\n";
    mErrorStream << "Row number: " << mFileRowNumber << "    " << "Column number: " << mFileColumnNumber << "\n";

    Error::execute();
}

DeviceConnectedToItselfError::DeviceConnectedToItselfError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{ErrorCode::DEVICE_CONNECTED_TO_ITSELF, fileRowNumber, fileColumnNumber, errorStream}
{
}

void DeviceConnectedToItselfError::execute()
{
    mErrorStream << "Error: device is connected to itself. This is not allowed.\n";
    mErrorStream << "Row number: " << mFileRowNumber << "    " << "Column number: " << mFileColumnNumber << "\n";

    Error::execute();
}

NoConnectionsError::NoConnectionsError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{ErrorCode::NULL_NR_OF_CONNECTIONS, fileRowNumber, fileColumnNumber, errorStream}
{
}

void NoConnectionsError::execute()
{
    mErrorStream << "Error: number of connections between the 2 devices is 0.\n";
    mErrorStream << "Row number: " << mFileRowNumber << "    " << "Column number: " << mFileColumnNumber << "\n";

    Error::execute();
}


InvalidCharactersError::InvalidCharactersError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{ErrorCode::INVALID_CHARS, fileRowNumber, fileColumnNumber, errorStream}
{
}

void InvalidCharactersError::execute()
{
    mErrorStream << "Error: the cell contains invalid characters.\n";
    mErrorStream << "Only \'-\', alphabet and numeric characters are allowed.\n";
    mErrorStream << "Row number: " << mFileRowNumber << "    " << "Column number: " << mFileColumnNumber << "\n";

    Error::execute();
}

InvalidUPositionValueError::InvalidUPositionValueError(const size_t fileRowNumber, const size_t fileColumnNumber, std::ofstream& errorStream)
    : Error{ErrorCode::INVALID_U_POSITION_VALUE, fileRowNumber, fileColumnNumber, errorStream}
{
}

void InvalidUPositionValueError::execute()
{
    mErrorStream << "Error: the device U position is invalid.\n";
    mErrorStream << "The cell is empty, contains non-numeric characters or the number is not within the required range.\n";
    mErrorStream << "A integer between 0 and 50 should be entered.\n";
    mErrorStream << "Row number: " << mFileRowNumber << "    " << "Column number: " << mFileColumnNumber << "\n";

    Error::execute();
}

EmptyConnectionsInputFileError::EmptyConnectionsInputFileError(std::ofstream& errorStream)
    : Error{ErrorCode::EMPTY_CONNECTIONS_INPUT_FILE, 2u, 1u, errorStream}
{
}

void EmptyConnectionsInputFileError::execute()
{
    mErrorStream << "Error: the connections input file contains no data.\n";
    mErrorStream << "File: " << Utilities::getConnectionInputFile() << "\n";

    Error::execute();
}

NoConnectionsDefinedError::NoConnectionsDefinedError(std::ofstream& errorStream)
    : Error{ErrorCode::NO_CONNECTIONS_DEFINED, 2u, 1u, errorStream}
{
}

void NoConnectionsDefinedError::execute()
{
    mErrorStream << "Error: the connection definitions file contains no devices or no connections between them.\n";
    mErrorStream << "File: " << Utilities::getConnectionDefinitionsFile() << "\n";

    Error::execute();
}
