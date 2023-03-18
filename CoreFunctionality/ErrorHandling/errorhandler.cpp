#include <cassert>

#include "errortypes.h"
#include "errorcodes.h"
#include "errorhandler.h"

ErrorHandler::ErrorHandler(std::ofstream* errorStream)
    : mpErrorStream{errorStream}
{
    assert(mpErrorStream && mpErrorStream->is_open());
}

ErrorPtr ErrorHandler::logError(const Error_t errorCode,
                                const size_t fileRowNumber,
                                const size_t fileColumnNumber)
{
    ErrorPtr pError{nullptr};

    if (mpErrorStream && mpErrorStream->is_open())
    {
        switch(static_cast<ErrorCode>(errorCode))
        {
        case ErrorCode::EMPTY_CELL:
            pError = std::make_shared<EmptyCellError>(fileRowNumber, fileColumnNumber, *mpErrorStream);
            break;
        case ErrorCode::UNKNOWN_DEVICE:
            pError = std::make_shared<UnknownDeviceError>(fileRowNumber, fileColumnNumber, *mpErrorStream);
            break;
        case ErrorCode::FEWER_CELLS:
            pError = std::make_shared<FewerCellsError>(fileRowNumber, fileColumnNumber, *mpErrorStream);
            break;
        case ErrorCode::INVALID_CONNECTION_FORMAT:
            pError = std::make_shared<InvalidConnectionFormatError>(fileRowNumber, fileColumnNumber, *mpErrorStream);
            break;
        case ErrorCode::DEVICE_U_POSITION_OUT_OF_RANGE:
            pError = std::make_shared<DeviceUPositionOutOfRangeError>(fileRowNumber, fileColumnNumber, *mpErrorStream);
            break;
        case ErrorCode::TARGET_DEVICE_NOT_FOUND:
            pError = std::make_shared<TargetDeviceNotFoundError>(fileRowNumber, fileColumnNumber, *mpErrorStream);
            break;
        case ErrorCode::DEVICE_CONNECTED_TO_ITSELF:
            pError = std::make_shared<DeviceConnectedToItselfError>(fileRowNumber, fileColumnNumber, *mpErrorStream);
            break;
        case ErrorCode::NULL_NR_OF_CONNECTIONS:
            pError = std::make_shared<NullNrOfConnectionsError>(fileRowNumber, fileColumnNumber, *mpErrorStream);
            break;
        case ErrorCode::INVALID_CHARACTERS:
            pError = std::make_shared<InvalidCharactersError>(fileRowNumber, fileColumnNumber, *mpErrorStream);
            break;
        case ErrorCode::INVALID_U_POSITION_VALUE:
            pError = std::make_shared<InvalidUPositionValueError>(fileRowNumber, fileColumnNumber, *mpErrorStream);
            break;
        case ErrorCode::EMPTY_CONNECTION_INPUT_FILE:
            pError = std::make_shared<EmptyConnectionInputFileError>(*mpErrorStream);
            break;
        case ErrorCode::NO_CONNECTIONS_DEFINED:
            pError = std::make_shared<NoConnectionsDefinedError>(*mpErrorStream);
            break;
        default:
            assert(false);
        }
    }

    return pError;
}
