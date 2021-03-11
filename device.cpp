#include "device.h"

const int Device::scRequiredNrOfInputDataFields{4};

Device::Device(const std::string& deviceType, int maxAllowedNrOfChars, bool isSourceDevice)
    : mDeviceType{deviceType}
    , mErrorCode{0}
    , mRow{1}
    , mColumn{1}
    , mMaxAllowedNrOfChars{maxAllowedNrOfChars}
    , mDeltaNrOfChars{0}
    , mIsSourceDevice{isSourceDevice}
{
    assert(mMaxAllowedNrOfChars > 0);
}

Device::Device(int errorCode)
    : mErrorCode{errorCode}
    , mRow{0}
    , mColumn{0}
    , mMaxAllowedNrOfChars{0}
    , mDeltaNrOfChars{0}
    , mIsSourceDevice{false}
{
    assert(mErrorCode > 0);
}

Device::~Device()
{
}

void Device::parseInputData(const std::string& input, int& pos, bool& error, std::ofstream& err)
{
    int totalParsedCharsCount{0}; // current total number of parsed input characters for the device (excluding comma)
    int position{pos}; // position in the input string to be stored in this temporary variable as it should only be written back to if error 4 does not occur
    std::vector<int> fieldSizes; // stores the size of each input field read for the device

    fieldSizes.resize(scRequiredNrOfInputDataFields);

    for (int fieldNumber{0}; fieldNumber < scRequiredNrOfInputDataFields; ++fieldNumber) //cat timp sunt inca substringuri de citit din sirul de intrare...
    {
        // check if characters are available for current (required) field
        if (-1 == position)
        {
            mErrorCode = 4;
            handleError(err);
            error = true;
            position = pos; //TODO: check if this instruction is necessary (or should it be pos = position???)
            break;
        }

        position = readDataField(input, *mInputData[fieldNumber], position);
        fieldSizes[fieldNumber] = mInputData[fieldNumber]->size();

        if (0 == fieldSizes[fieldNumber])
        {
            mErrorCode = 1;
            handleError(err); // read substring is empty, invoke error 1
            error = true;
        }

        ++mColumn;
        totalParsedCharsCount += fieldSizes[fieldNumber]; // comma (,) is not taken into consideration when updating the number of parsed characters
    }

    if (4 != mErrorCode)
    {
        // check if the total number of characters from the parsed fields exceed the device-specific maximum allowed count (e.g. 8 chars for the PDU class)
        if (totalParsedCharsCount > mMaxAllowedNrOfChars)
        {
            mDeltaNrOfChars = totalParsedCharsCount - mMaxAllowedNrOfChars;
            mErrorCode = 2;
            handleError(err);
            error = true;
        }

        pos = position;
    }
}

void Device::writeDescriptionAndLabel(std::string& out) const
{
    out += mDescription;
    out += ',';
    out += mLabel;
}

void Device::setRow(int row)
{
    mRow = row;
}

void Device::setColumn(int column)
{
    mColumn = column;
}

int Device::getRow() const
{
    return mRow;
}

int Device::getColumn() const
{
    return mColumn;
}

// the function also mentiones the error "coordinates" (row or cell from input .csv file where the error originated)
void Device::handleError(std::ofstream& err)
{
    using namespace std;

    switch(mErrorCode)
    {
    case 1:
        err << "Error: A cell is empty" << endl;
        err << "Row number: " << mRow << "    "<< "Column number: " << mColumn << endl;
        break;
    case 2:
        err << "Error: The total number of characters entered for the ";
        if (mIsSourceDevice)
        {
            err << "first ";
        }
        else
        {
            err << "second ";
        }
        err << "device exceeds the maximum allowed (" << mMaxAllowedNrOfChars << " characters)" << endl;
        err << "Row number: " << mRow << endl;
        err << "Maximum total number of characters exceeded by " << mDeltaNrOfChars << endl;
        break;
    case 3:
        err << "Error: an unknown device type has been entered in cell"<<endl;
        err << "Row number: " << mRow << "    " << "Column number: " << mColumn << endl;
        break;
    case 4:
        err << "Error: less cells have been filled on the row than required in order to store the parameters of the 2 devices" << endl;
        err << "A total number of 11 contiguous cells are required to be filled (starting with the first cell on the row)" << endl;
        err << "Row number: " << mRow << endl;
        break;
    case 5:
        err << "Error: format of the connection is wrong" << endl;
        err << "Row number: " << mRow << "    "<<"Column number: "<< mColumn << endl;
        break;
    case 6:
        err << "Error: U number of the device is out of range (should be between 1 and 50)" << endl;
        err << "Row number: " << mRow << "    " << "Column number: " << mColumn << endl;
        break;
    case 7:
        err << "Error: connection to a non-existent device. No device is mounted in the rack in the mentioned U position" << endl;
        err << "Row number: " << mRow << "    " << "Column number: " << mColumn << endl;
        break;
    case 8:
        err << "Error: device is connected to itself. This is not allowed." << endl;
        err << "Row number: " << mRow << "    " << "Column number: " << mColumn << endl;
        break;
    case 9:
        err << "Error: number of connections between the 2 devices is 0." << endl;
        err << "Row number: " << mRow << "    " << "Column number: " << mColumn << endl;
        break;
    default:
        err << "Unknown error" << endl;
    }
}


DeviceID::DeviceID(int connectionId, int deviceId)
    : mConnectionId{connectionId}
    , mDeviceId{deviceId}
{
}

bool DeviceID::operator<(const DeviceID &id) const
{
    bool result{false};

    if (mConnectionId < id.mConnectionId || (mConnectionId == id.mConnectionId && mDeviceId < id.mDeviceId))
    {
        result = true;
    }

    return result;
}

bool DeviceID::operator<=(const DeviceID &id) const
{
    bool result{true};

    if (mConnectionId > id.mConnectionId || (mConnectionId == id.mConnectionId && mDeviceId > id.mDeviceId))
    {
        result = false;
    }

    return result;
}

bool DeviceID::operator>(const DeviceID &id) const
{
    bool result{false};

    if (mConnectionId > id.mConnectionId || (mConnectionId == id.mConnectionId && mDeviceId > id.mDeviceId))
    {
        result = true;
    }

    return result;
}

bool DeviceID::operator>=(const DeviceID &id) const
{
    bool result{true};

    if (mConnectionId < id.mConnectionId || (mConnectionId == id.mConnectionId && mDeviceId < id.mDeviceId))
    {
        result = false;
    }

    return result;
}
