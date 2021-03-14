#include <cassert>

#include "error.h"
#include "device.h"

const int Device::scRequiredNrOfInputDataFields{4};

Device::Device(const std::string& deviceType, int maxAllowedNrOfChars, bool isSourceDevice)
    : mDeviceType{deviceType}
    , mRow{1}
    , mColumn{1}
    , mMaxAllowedNrOfChars{maxAllowedNrOfChars}
    , mDeltaNrOfChars{0}
    , mIsSourceDevice{isSourceDevice}
{
    assert(mMaxAllowedNrOfChars > 0);
}

Device::~Device()
{
}

std::vector<Error*> Device::parseInputData(const std::string& input, int& pos, bool& error, std::ofstream& errorStream)
{
    std::vector<Error*> parsingErrors;

    int totalParsedCharsCount{0}; // current total number of parsed input characters for the device (excluding comma)
    int position{pos}; // position in the input string to be stored in this temporary variable as it should only be written back to if error 4 does not occur
    std::vector<int> fieldSizes; // stores the size of each input field read for the device

    fieldSizes.resize(scRequiredNrOfInputDataFields);

    Error* lastError{nullptr};

    for (int fieldNumber{0}; fieldNumber < scRequiredNrOfInputDataFields; ++fieldNumber) //cat timp sunt inca substringuri de citit din sirul de intrare...
    {
        // check if characters are available for current (required) field
        if (-1 == position)
        {
            lastError = new FewerCellsError{errorStream};
            lastError->setRow(mRow);
            lastError->setColumn(mColumn);
            parsingErrors.push_back(lastError);
            error = true;
            position = pos; //TODO: check if this instruction is necessary (or should it be pos = position???)
            break;
        }

        position = readDataField(input, *mInputData[fieldNumber], position);
        fieldSizes[fieldNumber] = mInputData[fieldNumber]->size();

        if (0 == fieldSizes[fieldNumber])
        {
            lastError = new EmptyCellError{errorStream};
            lastError->setRow(mRow);
            lastError->setColumn(mColumn);
            parsingErrors.push_back(lastError);
            error = true;
        }

        ++mColumn;
        totalParsedCharsCount += fieldSizes[fieldNumber]; // comma (,) is not taken into consideration when updating the number of parsed characters
    }

    if (nullptr == dynamic_cast<FewerCellsError*>(lastError))
    {
        // check if the total number of characters from the parsed fields exceed the device-specific maximum allowed count (e.g. 8 chars for the PDU class)
        if (totalParsedCharsCount > mMaxAllowedNrOfChars)
        {
            mDeltaNrOfChars = totalParsedCharsCount - mMaxAllowedNrOfChars;
            lastError = new ExceedingCharsCountError{errorStream, mMaxAllowedNrOfChars, mDeltaNrOfChars, mIsSourceDevice};
            lastError->setRow(mRow);
            lastError->setColumn(mColumn);
            parsingErrors.push_back(lastError);
            error = true;
        }

        pos = position;
    }

    return parsingErrors;
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
