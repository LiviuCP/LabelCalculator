#include <cassert>

#include "errortypes.h"
#include "device.h"

const int Device::scMaxInputParametersCount{4};

Device::Device(const std::string& deviceType, int requiredNumberOfParameters, int maxAllowedNrOfChars, bool isSourceDevice)
    : mDeviceType{deviceType}
    , mRow{1}
    , mColumn{1}
    , mMaxAllowedNrOfChars{maxAllowedNrOfChars}
    , mDeltaNrOfChars{0}
    , mIsSourceDevice{isSourceDevice}
    , mInputParametersCount{requiredNumberOfParameters}
{
    assert(mMaxAllowedNrOfChars > 0);
    assert(mInputParametersCount > 1 &&
           mInputParametersCount <= scMaxInputParametersCount); // there should be at least two parameters (device name and port number)

    mInputData.reserve(mInputParametersCount);
}

Device::~Device()
{
}

int Device::parseInputData(const std::string& input, const int initialPosition, std::vector<ErrorPtr>& parsingErrors, std::ofstream& errorStream)
{
    assert(static_cast<int>(mInputData.size()) == mInputParametersCount); // check if all required parameters have been registered by derived class

    int currentPosition{initialPosition}; // position in input string (.csv row) where the input parameters of the device begin
    int currentParameter{0};              // current field (cell) containining a device input parameter (e.g. device name)
    int totalParsedCharsCount{0};         // current total number of parsed input characters for the device (excluding comma)
    bool fewerCellsProvided{false};       // for checking if the "fewer cells" error occurred
    std::vector<int> fieldSizes;          // stores the size of each input field read for the device
    ErrorPtr lastError{nullptr};          // last found error

    parsingErrors.clear();
    fieldSizes.resize(mInputParametersCount);

    // check the "useful" fields (required input parameters for the device)
    while(currentParameter < mInputParametersCount)
    {
        assert(nullptr != mInputData[currentParameter]); // defensive programming, this code should not evaluate to true (the registering function should prevent this)

        if (-1 != currentPosition) // check if characters are available for current (required) field
        {
            currentPosition = readDataField(input, *mInputData[currentParameter], currentPosition);
            fieldSizes[currentParameter] = mInputData[currentParameter]->size();

            if (0 == fieldSizes[currentParameter])
            {
                lastError = std::make_shared<EmptyCellError>(errorStream);
                lastError->setRow(mRow);
                lastError->setColumn(mColumn);
                parsingErrors.push_back(lastError);
            }

            ++mColumn;
            totalParsedCharsCount += fieldSizes[currentParameter]; // comma (,) is not taken into consideration when updating the number of parsed characters
        }
        else
        {
            fewerCellsProvided = true;
            break;
        }

        ++currentParameter;
    }

    // check the padding fields (if any)
    if (!fewerCellsProvided)
    {
        while(currentParameter < scMaxInputParametersCount)
        {
            if (-1 != currentPosition)
            {
                std::string unusedField;
                currentPosition = readDataField(input, unusedField, currentPosition);
            }
            else
            {
                if (mIsSourceDevice)
                {
                    fewerCellsProvided = true;
                }

                break;
            }

            ++currentParameter;
        }
    }

    // handle parameter-independent errors
    if (fewerCellsProvided)
    {
        lastError = std::make_shared<FewerCellsError>(errorStream);
        lastError->setRow(mRow);
        lastError->setColumn(mColumn);
        parsingErrors.push_back(lastError);

    }
    else if (totalParsedCharsCount > mMaxAllowedNrOfChars)
    {
        mDeltaNrOfChars = totalParsedCharsCount - mMaxAllowedNrOfChars;
        lastError = std::make_shared<ExceedingCharsCountError>(errorStream, mMaxAllowedNrOfChars, mDeltaNrOfChars, mIsSourceDevice);
        lastError->setRow(mRow);
        lastError->setColumn(mColumn);
        parsingErrors.push_back(lastError);
    }
    else
    {
        // no parameter-independent errors, defensive programming
    }

    return currentPosition;
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

std::string Device::getDescription() const
{
    return mDescription;
}

std::string Device::getLabel() const
{
    return mLabel;
}

void Device::_registerRequiredParameter(std::string* const pRequiredParameter)
{
    if (nullptr != pRequiredParameter)
    {
        mInputData.push_back(pRequiredParameter);
    }
    else
    {
        assert(false);
    }
}
