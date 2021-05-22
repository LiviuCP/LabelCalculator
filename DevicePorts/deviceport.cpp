#include <cassert>

#include "errortypes.h"
#include "deviceport.h"

const size_t DevicePort::scMaxInputParametersCount{3u};

DevicePort::DevicePort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const size_t requiredNumberOfParameters, const size_t maxAllowedCharsCount, const bool isSourceDevice)
    : mDeviceUPosition{deviceUPosition}
    , mFileRowNumber{fileRowNumber}
    , mFileColumnNumber{fileColumnNumber}
    , mInputParametersCount{requiredNumberOfParameters}
    , mMaxAllowedCharsCount{maxAllowedCharsCount}
    , mIsSourceDevice{isSourceDevice}
{
    assert(mFileRowNumber > 0u &&
           mFileColumnNumber > 0u);
    assert(mMaxAllowedCharsCount > 0u);
    assert(mInputParametersCount > 1u &&
           mInputParametersCount <= scMaxInputParametersCount); // there should be at least two parameters (device name and port number)

    mInputData.reserve(mInputParametersCount);
}

DevicePort::~DevicePort()
{
}

ssize_t DevicePort::parseInputData(const std::string& input, const ssize_t initialPosition, std::vector<ErrorPtr>& parsingErrors, std::ofstream& errorStream)
{
    assert(mInputData.size() == mInputParametersCount); // check if all required parameters have been registered by derived class

    ssize_t currentPosition{initialPosition}; // position in input string (.csv row) where the input parameters of the device begin
    size_t currentParameter{0u};              // current field (cell) containining a device input parameter (e.g. device name)
    size_t totalParsedCharsCount{0u};         // current total number of parsed input characters for the device (excluding comma)
    bool fewerCellsProvided{false};       // for checking if the "fewer cells" error occurred
    std::vector<size_t> fieldSizes;          // stores the size of each input field read for the device
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

            bool noErrorsDetectedInCell{false};

            if (0u == fieldSizes[currentParameter])
            {
                lastError = std::make_shared<EmptyCellError>(errorStream);
            }
            else if (areInvalidCharactersContained(*mInputData[currentParameter]))
            {
                lastError = std::make_shared<InvalidCharactersError>(errorStream);
            }
            else
            {
                noErrorsDetectedInCell = true;
            }

            if (!noErrorsDetectedInCell)
            {
                lastError->setCSVRowNumber(mFileRowNumber);
                lastError->setCSVColumnNumber(mFileColumnNumber);
                parsingErrors.push_back(lastError);
            }

            ++mFileColumnNumber;
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
        lastError->setCSVRowNumber(mFileRowNumber);
        lastError->setCSVColumnNumber(mFileColumnNumber);
        parsingErrors.push_back(lastError);

    }
    else if (totalParsedCharsCount > mMaxAllowedCharsCount)
    {
        const ssize_t c_DeltaNrOfChars{static_cast<ssize_t>(totalParsedCharsCount - mMaxAllowedCharsCount)};
        lastError = std::make_shared<ExceedingCharsCountError>(errorStream, mMaxAllowedCharsCount, c_DeltaNrOfChars, mIsSourceDevice);
        lastError->setCSVRowNumber(mFileRowNumber);
        lastError->setCSVColumnNumber(mFileColumnNumber);
        parsingErrors.push_back(lastError);
    }
    else
    {
        // no parameter-independent errors, defensive programming
    }

    return currentPosition;
}

size_t DevicePort::getCSVRowNumber() const
{
    return mFileRowNumber;
}

size_t DevicePort::getCSVColumnNumber() const
{
    return mFileColumnNumber;
}

std::string DevicePort::getDescription() const
{
    return mDescription;
}

std::string DevicePort::getLabel() const
{
    return mLabel;
}

void DevicePort::_registerRequiredParameter(std::string* const pRequiredParameter)
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
