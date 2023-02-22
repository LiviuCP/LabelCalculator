#include <sstream>
#include <cassert>

#include "applicationdata.h"
#include "coreutils.h"
#include "deviceportdata.h"
#include "deviceportutils.h"
#include "deviceport.h"

namespace Core = Utilities::Core;
namespace Ports = Utilities::DevicePorts;

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

Index_t DevicePort::parseInputData(const std::string& input, const Index_t initialPosition, ErrorHandler& errorHandler, std::ofstream& errorStream, std::vector<ErrorPtr>& parsingErrors)
{
    assert(mInputData.size() == mInputParametersCount); // check if all required parameters have been registered by derived class

    Index_t currentPosition{initialPosition}; // position in input string (.csv row) where the input parameters of the device begin
    size_t currentParameter{0u};              // current field (cell) containining a device input parameter (e.g. device name)
    bool fewerCellsProvided{false};       // for checking if the "fewer cells" error occurred
    ErrorPtr lastError{nullptr};          // last found error

    parsingErrors.clear();

    // check the "useful" fields (required input parameters for the device)
    while(currentParameter < mInputParametersCount)
    {
        assert(nullptr != mInputData[currentParameter]); // defensive programming, this code should not evaluate to true (the registering function should prevent this)

        if (currentPosition.has_value()) // check if characters are available for current (required) field
        {
            currentPosition = Core::readDataField(input, *mInputData[currentParameter], currentPosition);

            bool noErrorsDetectedInCell{false};

            if (0u == mInputData[currentParameter]->size())
            {
                lastError = errorHandler.logError(ErrorCode::EMPTY_CELL, mFileRowNumber, mFileColumnNumber, errorStream);
            }
            else if (Core::areInvalidCharactersContained(*mInputData[currentParameter]))
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
            if (currentPosition.has_value())
            {
                std::string unusedField;
                currentPosition = Core::readDataField(input, unusedField, currentPosition);
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

void DevicePort::_initializeDescriptionAndLabel(std::string_view deviceTypeDescription, std::string_view deviceTypeLabel)
{
    const std::string c_DeviceTypeDescription{deviceTypeDescription.size() > 0 ? deviceTypeDescription : "Device"};

    mDescription = c_DeviceTypeDescription;
    mDescription += " placed at U";
    mDescription += mDeviceUPosition;

    mLabel = "U" + mDeviceUPosition;

    if (deviceTypeLabel.size() > 0)
    {
        mLabel += "_";
        mLabel += deviceTypeLabel;
    }
}

void DevicePort::_appendDataToDescription(std::string_view data)
{
    mDescription += data;
}

void DevicePort::_appendDataToLabel(std::string_view data)
{
    mLabel += data;
}

void DevicePort::_setInvalidDescriptionAndLabel(std::string_view descriptionInput, std::string_view labelInput)
{
    const size_t c_DescriptionInputLength{descriptionInput.size()};
    assert(c_DescriptionInputLength > 0u);

    mDescription.clear();
    mDescription.reserve(1/*"U"*/ + mDeviceUPosition.size()  + 2/*": "*/ + c_DescriptionInputLength);
    mDescription.push_back('U');
    mDescription.append(mDeviceUPosition);
    mDescription.append(": ");
    mDescription.append(descriptionInput);

    if (labelInput.size() > 0u)
    {
        mLabel = labelInput;
    }
    else
    {
        const std::string c_CheckConnectionInputFileText{Ports::getCheckConnectionInputFileText()};

        mLabel.clear();
        mLabel.reserve(Ports::c_LabelErrorText.size() + c_CheckConnectionInputFileText.size());
        mLabel.append(Ports::c_LabelErrorText);
        mLabel.append(c_CheckConnectionInputFileText);
    }
}

void DevicePort::_checkLabel()
{
    const size_t c_LabelCharsCount{mLabel.size()};

    assert(c_LabelCharsCount > 0u);

    if (std::string::npos == mLabel.find(Ports::c_LabelErrorText) &&
        c_LabelCharsCount > Data::c_MaxLabelCharsCount)
    {
        std::stringstream stream;
        const ssize_t c_DeltaCharsCount{static_cast<ssize_t>(c_LabelCharsCount - Data::c_MaxLabelCharsCount)};
        stream << Ports::c_MaxLabelCharsCountExceededErrorText << c_DeltaCharsCount;

        _setInvalidDescriptionAndLabel(stream.str());
    }
}
