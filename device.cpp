#include <cassert>

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

int Device::parseInputData(const std::string& input, const int initialPosition, std::vector<ErrorPtr>& parsingErrors, std::ofstream& errorStream)
{
    int currentPosition{initialPosition};
    int totalParsedCharsCount{0}; // current total number of parsed input characters for the device (excluding comma)
    std::vector<int> fieldSizes; // stores the size of each input field read for the device
    ErrorPtr lastError{nullptr};

    parsingErrors.clear();
    fieldSizes.resize(scRequiredNrOfInputDataFields);

    for (int fieldNumber{0}; fieldNumber < scRequiredNrOfInputDataFields; ++fieldNumber) //cat timp sunt inca substringuri de citit din sirul de intrare...
    {
        // check if characters are available for current (required) field
        if (-1 != currentPosition)
        {
            currentPosition = readDataField(input, *mInputData[fieldNumber], currentPosition);
            fieldSizes[fieldNumber] = mInputData[fieldNumber]->size();

            if (0 == fieldSizes[fieldNumber])
            {
                lastError = std::make_shared<EmptyCellError>(errorStream);
                lastError->setRow(mRow);
                lastError->setColumn(mColumn);
                parsingErrors.push_back(lastError);
            }

            ++mColumn;
            totalParsedCharsCount += fieldSizes[fieldNumber]; // comma (,) is not taken into consideration when updating the number of parsed characters
        }
        else
        {
            lastError = std::make_shared<FewerCellsError>(errorStream);
            lastError->setRow(mRow);
            lastError->setColumn(mColumn);
            parsingErrors.push_back(lastError);
            break;
        }
    }

    if (nullptr == dynamic_cast<FewerCellsError*>(lastError.get()))
    {
        // check if the total number of characters from the parsed fields exceed the device-specific maximum allowed count (e.g. 8 chars for the PDU class)
        if (totalParsedCharsCount > mMaxAllowedNrOfChars)
        {
            mDeltaNrOfChars = totalParsedCharsCount - mMaxAllowedNrOfChars;
            lastError = std::make_shared<ExceedingCharsCountError>(errorStream, mMaxAllowedNrOfChars, mDeltaNrOfChars, mIsSourceDevice);
            lastError->setRow(mRow);
            lastError->setColumn(mColumn);
            parsingErrors.push_back(lastError);
        }
    }

    return currentPosition;
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
