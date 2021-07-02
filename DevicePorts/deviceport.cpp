#include <sstream>
#include <cassert>

#include "applicationdata.h"
#include "applicationutils.h"
#include "deviceportdata.h"
#include "deviceportutils.h"
#include "deviceport.h"

DevicePort::DevicePort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const size_t requiredNumberOfParameters, const bool isSourceDevice)
    : mDeviceUPosition{deviceUPosition}
    , mFileRowNumber{fileRowNumber}
    , mFileColumnNumber{fileColumnNumber}
    , mInputParametersCount{requiredNumberOfParameters}
    , mIsSourceDevice{isSourceDevice}
{
    assert(mFileRowNumber > 0u &&
           mFileColumnNumber > 0u);
    assert(mInputParametersCount > 1u &&
           mInputParametersCount <= Data::c_MaxPortInputParametersCount); // there should be at least two parameters (device name and port number)

    mInputData.reserve(mInputParametersCount);
}

DevicePort::~DevicePort()
{
}

ssize_t DevicePort::parseInputData(const std::string& input, const ssize_t initialPosition, ErrorHandler& errorHandler, std::ofstream& errorStream, std::vector<ErrorPtr>& parsingErrors)
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
            currentPosition = Utilities::readDataField(input, *mInputData[currentParameter], currentPosition);
            fieldSizes[currentParameter] = mInputData[currentParameter]->size();

            bool noErrorsDetectedInCell{false};

            if (0u == fieldSizes[currentParameter])
            {
                lastError = errorHandler.logError(ErrorCode::EMPTY_CELL, mFileRowNumber, mFileColumnNumber, errorStream);
            }
            else if (Utilities::areInvalidCharactersContained(*mInputData[currentParameter]))
            {
                lastError = errorHandler.logError(ErrorCode::INVALID_CHARACTERS, mFileRowNumber, mFileColumnNumber, errorStream);
            }
            else
            {
                noErrorsDetectedInCell = true;
            }

            if (!noErrorsDetectedInCell)
            {
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
        while(currentParameter < Data::c_MaxPortInputParametersCount)
        {
            if (-1 != currentPosition)
            {
                std::string unusedField;
                currentPosition = Utilities::readDataField(input, unusedField, currentPosition);
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
        lastError = errorHandler.logError(ErrorCode::FEWER_CELLS, mFileRowNumber, mFileColumnNumber, errorStream);
        parsingErrors.push_back(lastError);
    }

    return currentPosition;
}

size_t DevicePort::getFileRowNumber() const
{
    return mFileRowNumber;
}

size_t DevicePort::getFileColumnNumber() const
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

void DevicePort::_checkLabel()
{
    const size_t c_LabelCharsCount{mLabel.size()};

    assert(c_LabelCharsCount > 0u);

    if (Utilities::c_LabelErrorText != mLabel)
    {
        if (c_LabelCharsCount > Data::c_MaxLabelCharsCount)
        {
            std::stringstream stream;
            const ssize_t c_DeltaCharsCount{static_cast<ssize_t>(c_LabelCharsCount - Data::c_MaxLabelCharsCount)};
            stream << Utilities::c_MaxLabelCharsCountExceededErrorText << c_DeltaCharsCount;

            mDescription = stream.str();
            mLabel = Utilities::c_LabelErrorText + Utilities::getCheckConnectionInputFileText();
        }
    }
    else
    {
        mLabel += Utilities::getCheckConnectionInputFileText();
    }
}
