#ifndef ERRORCODES_H
#define ERRORCODES_H

enum class ErrorCode : unsigned short
{
    EMPTY_CELL = 1u,
    UNKNOWN_DEVICE,
    FEWER_CELLS,
    INVALID_CONNECTION_FORMAT,
    DEVICE_U_POSITION_OUT_OF_RANGE,
    TARGET_DEVICE_NOT_FOUND,
    DEVICE_CONNECTED_TO_ITSELF,
    NULL_NR_OF_CONNECTIONS,
    INVALID_CHARACTERS,
    INVALID_U_POSITION_VALUE,
    EMPTY_CONNECTION_INPUT_FILE,
    NO_CONNECTIONS_DEFINED,
    ErrorCodesUpperBound
};

#endif // ERRORCODES_H
