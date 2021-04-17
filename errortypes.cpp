#include "errortypes.h"

EmptyCellError::EmptyCellError(std::ofstream& errorStream)
    : Error{ErrorCode::EMPTY_CELL, errorStream}
{
}

void EmptyCellError::execute()
{
    mErrorStream << "Error: A cell is empty" << std::endl;
    mErrorStream << "Row number: " << mRow << "    "<< "Column number: " << mColumn << std::endl;

    Error::execute();
}

ExceedingCharsCountError::ExceedingCharsCountError(std::ofstream& errorStream, int maxAllowedCharsCount, int deltaCharsCount, bool isSourceDevice)
    : Error{ErrorCode::MAX_CHARS_EXCEEDED, errorStream}
    , mMaxAllowedCharsCount{maxAllowedCharsCount}
    , mDeltaCharsCount{deltaCharsCount}
    , mIsSourceDevice{isSourceDevice}
{
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
    mErrorStream << "Row number: " << mRow << std::endl;
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
    mErrorStream << "Row number: " << mRow << "    " << "Column number: " << mColumn << std::endl;

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
    mErrorStream << "Row number: " << mRow << std::endl;

    Error::execute();
}

WrongFormatError::WrongFormatError(std::ofstream& errorStream)
    : Error{ErrorCode::WRONG_CONNECTION_FORMAT, errorStream}
{
}

void WrongFormatError::execute()
{
    mErrorStream << "Error: format of the connection is wrong" << std::endl;
    mErrorStream << "Row number: " << mRow << "    "<<"Column number: "<< mColumn << std::endl;

    Error::execute();
}

WrongUNumberError::WrongUNumberError(std::ofstream& errorStream)
    : Error{ErrorCode::PLACEMENT_OUT_OF_RANGE, errorStream}
{
}

void WrongUNumberError::execute()
{
    mErrorStream << "Error: U number of the device is out of range (should be between 1 and 50)" << std::endl;
    mErrorStream << "Row number: " << mRow << "    " << "Column number: " << mColumn << std::endl;

    Error::execute();
}

NoDevicePresentError::NoDevicePresentError(std::ofstream& errorStream)
    : Error{ErrorCode::NO_DEVICE_PLACED_IN_POSITION, errorStream}
{
}

void NoDevicePresentError::execute()
{
    mErrorStream << "Error: connection to a non-existent device. No device is mounted in the rack in the mentioned U position" << std::endl;
    mErrorStream << "Row number: " << mRow << "    " << "Column number: " << mColumn << std::endl;

    Error::execute();
}

DeviceConnectedToItselfError::DeviceConnectedToItselfError(std::ofstream& errorStream)
    : Error{ErrorCode::DEVICE_CONNECTED_TO_ITSELF, errorStream}
{
}

void DeviceConnectedToItselfError::execute()
{
    mErrorStream << "Error: device is connected to itself. This is not allowed." << std::endl;
    mErrorStream << "Row number: " << mRow << "    " << "Column number: " << mColumn << std::endl;

    Error::execute();
}

NoConnectionsError::NoConnectionsError(std::ofstream& errorStream)
    : Error{ErrorCode::NULL_NR_OF_CONNECTIONS, errorStream}
{
}

void NoConnectionsError::execute()
{
    mErrorStream << "Error: number of connections between the 2 devices is 0." << std::endl;
    mErrorStream << "Row number: " << mRow << "    " << "Column number: " << mColumn << std::endl;

    Error::execute();
}
