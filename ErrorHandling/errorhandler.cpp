#include <cassert>

#include "errortypes.h"
#include "errorhandler.h"

ErrorHandler::ErrorHandler()
    : mLastLoggingRowNumber{0u} // this row number does not exist but is required as initial value (row numbering starts at 1)
    , mFewerCellsErrorOccurred{false}
{
}

void ErrorHandler::reset()
{
    mLastLoggingRowNumber = 0u;
    mFewerCellsErrorOccurred = false;
}

ErrorPtr ErrorHandler::logError(ErrorCode errorCode,
                                const size_t fileRowNumber,
                                const size_t fileColumnNumber,
                                std::ofstream& errorStream)
{
    _setLastLoggingRowNumber(fileRowNumber);

    ErrorPtr pError{nullptr};

    switch(errorCode)
    {
    case ErrorCode::EMPTY_CELL:
        pError = std::make_shared<EmptyCellError>(fileRowNumber, fileColumnNumber, errorStream);
        break;
    case ErrorCode::UNKNOWN_DEVICE:
        pError = std::make_shared<UnknownDeviceError>(fileRowNumber, fileColumnNumber, errorStream);
        break;
    case ErrorCode::FEWER_CELLS:
        pError = std::make_shared<FewerCellsError>(fileRowNumber, fileColumnNumber, errorStream);
        mFewerCellsErrorOccurred = true;
        break;
    case ErrorCode::WRONG_CONNECTION_FORMAT:
        pError = std::make_shared<WrongConnectionFormatError>(fileRowNumber, fileColumnNumber, errorStream);
        break;
    case ErrorCode::DEVICE_U_POSITION_OUT_OF_RANGE:
        pError = std::make_shared<DeviceUPositionOutOfRangeError>(fileRowNumber, fileColumnNumber, errorStream);
        break;
    case ErrorCode::TARGET_DEVICE_NOT_FOUND:
        pError = std::make_shared<TargetDeviceNotFoundError>(fileRowNumber, fileColumnNumber, errorStream);
        break;
    case ErrorCode::DEVICE_CONNECTED_TO_ITSELF:
        pError = std::make_shared<DeviceConnectedToItselfError>(fileRowNumber, fileColumnNumber, errorStream);
        break;
    case ErrorCode::NULL_NR_OF_CONNECTIONS:
        pError = std::make_shared<NoConnectionsError>(fileRowNumber, fileColumnNumber, errorStream);
        break;
    case ErrorCode::INVALID_CHARS:
        pError = std::make_shared<InvalidCharactersError>(fileRowNumber, fileColumnNumber, errorStream);
        break;
    case ErrorCode::INVALID_U_POSITION_VALUE:
        pError = std::make_shared<InvalidUPositionValueError>(fileRowNumber, fileColumnNumber, errorStream);
        break;
    case ErrorCode::EMPTY_CONNECTION_INPUT_FILE:
        pError = std::make_shared<EmptyConnectionInputFileError>(errorStream);
        break;
    case ErrorCode::NO_CONNECTIONS_DEFINED:
        pError = std::make_shared<NoConnectionsDefinedError>(errorStream);
        break;
    default:
        assert(false);
    }

    return pError;
}

bool ErrorHandler::fewerCellsErrorLogged() const
{
    return mFewerCellsErrorOccurred;
}

size_t ErrorHandler::getLastLoggingRowNumber() const
{
    return mLastLoggingRowNumber;
}

void ErrorHandler::_setLastLoggingRowNumber(const size_t fileRowNumber)
{
    if (fileRowNumber != mLastLoggingRowNumber)
    {
        mLastLoggingRowNumber = fileRowNumber;
        mFewerCellsErrorOccurred = false;
    }
}
