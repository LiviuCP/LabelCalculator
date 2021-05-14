#include <cassert>

#include "errortypes.h"

EmptyCellError::EmptyCellError(std::ofstream& errorStream)
    : Error{ErrorCode::EMPTY_CELL, errorStream}
{
}

void EmptyCellError::execute()
{
    mErrorStream << "Error: A cell is empty" << std::endl;
    mErrorStream << "Row number: " << mCSVRowNumber << "    "<< "Column number: " << mCSVColumnNumber << std::endl;

    Error::execute();
}

ExceedingCharsCountError::ExceedingCharsCountError(std::ofstream& errorStream, size_t maxAllowedCharsCount, ssize_t deltaCharsCount, bool isSourceDevice)
    : Error{ErrorCode::MAX_CHARS_EXCEEDED, errorStream}
    , mMaxAllowedCharsCount{maxAllowedCharsCount}
    , mDeltaCharsCount{deltaCharsCount}
    , mIsSourceDevice{isSourceDevice}
{
    assert(mDeltaCharsCount > 0);
}

void ExceedingCharsCountError::execute()
{
    mErrorStream << "Error: The total number of characters entered for the ";

    if (mIsSourceDevice)
    {
        mErrorStream << "first ";
    }
    else
    {
        mErrorStream << "second ";
    }

    mErrorStream << "device exceeds the maximum allowed (" << mMaxAllowedCharsCount << " characters)" << std::endl;
    mErrorStream << "Row number: " << mCSVRowNumber << std::endl;
    mErrorStream << "Maximum total number of characters exceeded by " << mDeltaCharsCount << std::endl;

    Error::execute();
}

UnknownDeviceError::UnknownDeviceError(std::ofstream& errorStream)
    : Error{ErrorCode::UNKNOWN_DEVICE, errorStream}
{
}

void UnknownDeviceError::execute()
{
    mErrorStream << "Error: an unknown or currently not supported device type has been entered" << std::endl;
    mErrorStream << "Row number: " << mCSVRowNumber << "    " << "Column number: " << mCSVColumnNumber << std::endl;

    Error::execute();
}

FewerCellsError::FewerCellsError(std::ofstream &errorStream)
    : Error{ErrorCode::FEWER_CELLS, errorStream}
{
}

void FewerCellsError::execute()
{
    mErrorStream << "Error: less cells have been filled on the row than required in order to store the parameters of the 2 devices" << std::endl;
    mErrorStream << "A total number of 11 contiguous cells are required to be filled (starting with the first cell on the row)" << std::endl;
    mErrorStream << "Row number: " << mCSVRowNumber << std::endl;

    Error::execute();
}

WrongFormatError::WrongFormatError(std::ofstream& errorStream)
    : Error{ErrorCode::WRONG_CONNECTION_FORMAT, errorStream}
{
}

void WrongFormatError::execute()
{
    mErrorStream << "Error: format of the connection is wrong" << std::endl;
    mErrorStream << "Row number: " << mCSVRowNumber << "    "<<"Column number: "<< mCSVColumnNumber << std::endl;

    Error::execute();
}

WrongUNumberError::WrongUNumberError(std::ofstream& errorStream)
    : Error{ErrorCode::PLACEMENT_OUT_OF_RANGE, errorStream}
{
}

void WrongUNumberError::execute()
{
    mErrorStream << "Error: U number of the device is out of range (should be between 1 and 50)" << std::endl;
    mErrorStream << "Row number: " << mCSVRowNumber << "    " << "Column number: " << mCSVColumnNumber << std::endl;

    Error::execute();
}

InvalidTargetDevicePositionError::InvalidTargetDevicePositionError(std::ofstream& errorStream)
    : Error{ErrorCode::INVALID_TARGET_DEVICE_POSITION, errorStream}
{
}

void InvalidTargetDevicePositionError::execute()
{
    mErrorStream << "Error: the target device position is invalid." << std::endl;
    mErrorStream << "Either no device is mounted within rack in the mentioned U position or the target position is lower than the one of the source device." << std::endl;
    mErrorStream << "Please note that the connections should always be defined bottom-up." << std::endl;
    mErrorStream << "Row number: " << mCSVRowNumber << "    " << "Column number: " << mCSVColumnNumber << std::endl;

    Error::execute();
}

DeviceConnectedToItselfError::DeviceConnectedToItselfError(std::ofstream& errorStream)
    : Error{ErrorCode::DEVICE_CONNECTED_TO_ITSELF, errorStream}
{
}

void DeviceConnectedToItselfError::execute()
{
    mErrorStream << "Error: device is connected to itself. This is not allowed." << std::endl;
    mErrorStream << "Row number: " << mCSVRowNumber << "    " << "Column number: " << mCSVColumnNumber << std::endl;

    Error::execute();
}

NoConnectionsError::NoConnectionsError(std::ofstream& errorStream)
    : Error{ErrorCode::NULL_NR_OF_CONNECTIONS, errorStream}
{
}

void NoConnectionsError::execute()
{
    mErrorStream << "Error: number of connections between the 2 devices is 0." << std::endl;
    mErrorStream << "Row number: " << mCSVRowNumber << "    " << "Column number: " << mCSVColumnNumber << std::endl;

    Error::execute();
}


InvalidCharactersError::InvalidCharactersError(std::ofstream& errorStream)
    : Error{ErrorCode::INVALID_CHARS, errorStream}
{
}

void InvalidCharactersError::execute()
{
    mErrorStream << "Error: the cell contains invalid characters." << std::endl;
    mErrorStream << "Only \'-\', alphabet and numeric characters are allowed." << std::endl;
    mErrorStream << "Row number: " << mCSVRowNumber << "    " << "Column number: " << mCSVColumnNumber << std::endl;

    Error::execute();
}

InvalidDeviceUPositionError::InvalidDeviceUPositionError(std::ofstream& errorStream)
    : Error{ErrorCode::INVALID_DEVICE_U_POSITION, errorStream}
{
}

void InvalidDeviceUPositionError::execute()
{
    mErrorStream << "Error: the device U position is invalid." << std::endl;
    mErrorStream << "The cell is empty, contains non-numeric characters or the number is not within the required range." << std::endl;
    mErrorStream << "A integer between 0 and 50 should be entered." << std::endl;
    mErrorStream << "Row number: " << mCSVRowNumber << "    " << "Column number: " << mCSVColumnNumber << std::endl;

    Error::execute();
}
