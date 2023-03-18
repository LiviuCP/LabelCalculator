#include <sstream>
#include <cassert>

#include "errorcodes.h"
#include "deviceportdata.h"
#include "deviceportutils.h"
#include "deviceport.h"

namespace Core = Utilities::Core;
namespace Ports = Utilities::DevicePorts;

DevicePort::DevicePort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice)
    : mDeviceUPosition{deviceUPosition}
    , mFileRowNumber{fileRowNumber}
    , mFileColumnNumber{1}
    , mIsSourceDevice{isSourceDevice}
    , mIsInitialized{false}
{
}

DevicePort::~DevicePort()
{
}

void DevicePort::init()
{
    if (!mIsInitialized)
    {
        _initializeRequiredParameters();
        _initializeDescriptionAndLabel();

        mIsInitialized = true;
    }
}

void DevicePort::parseInputData(const std::string_view input, std::vector<ErrorPtr>& parsingErrors)
{
    assert(mInputData.size() == mInputParametersCount); // check if all required parameters have been registered by derived class

    size_t currentParameter{0u};              // current field (cell) containining a device input parameter (e.g. device name)
    bool fewerCellsProvided{false};       // for checking if the "fewer cells" error occurred
    ErrorPtr lastError{nullptr};          // last found error

    parsingErrors.clear();

    // check the "useful" fields (required input parameters for the device)
    while(currentParameter < mInputParametersCount)
    {
        assert(nullptr != mInputData[currentParameter]); // defensive programming, this code should not evaluate to true (the registering function should prevent this)

        if (mCurrentPosition.has_value()) // check if characters are available for current (required) field
        {
            mCurrentPosition = Core::readDataField(input, *mInputData[currentParameter], mCurrentPosition);

            bool noErrorsDetectedInCell{false};

            if (0u == mInputData[currentParameter]->size())
            {
                lastError = mpErrorHandler->logError(static_cast<Error_t>(ErrorCode::EMPTY_CELL), mFileRowNumber, mFileColumnNumber);
            }
            else if (Core::areInvalidCharactersContained(*mInputData[currentParameter]))
            {
                lastError = mpErrorHandler->logError(static_cast<Error_t>(ErrorCode::INVALID_CHARACTERS), mFileRowNumber, mFileColumnNumber);
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
            if (mCurrentPosition.has_value())
            {
                std::string unusedField;
                mCurrentPosition = Core::readDataField(input, unusedField, mCurrentPosition);
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
        lastError = mpErrorHandler->logError(static_cast<Error_t>(ErrorCode::FEWER_CELLS), mFileRowNumber, mFileColumnNumber);
        parsingErrors.push_back(lastError);
    }
}

Index_t DevicePort::getCurrentPosition() const
{
    return mCurrentPosition;
}

size_t DevicePort::getFileColumnNumber() const
{
    return mFileColumnNumber;
}

size_t DevicePort::getFileRowNumber() const
{
    return mFileRowNumber;
}

std::string DevicePort::getDescription() const
{
    return mDescription;
}

std::string DevicePort::getLabel() const
{
    return mLabel;
}

void DevicePort::setErrorHandler(std::shared_ptr<ErrorHandler> pErrorHandler)
{
    if (!mpErrorHandler && pErrorHandler)
    {
        mpErrorHandler = pErrorHandler;
    }
}

void DevicePort::setFileColumnNumber(const size_t fileColumnNumber)
{
    mFileColumnNumber = fileColumnNumber;
}

void DevicePort::setCurrentPosition(const Index_t currentPosition)
{
    mCurrentPosition = currentPosition;
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

void DevicePort::_appendDataToDescription(const std::string_view data)
{
    if (mIsInitialized)
    {
        mDescription += data;
    }
    else
    {
        assert(false);
    }
}

void DevicePort::_appendDataToLabel(const std::string_view data)
{
    if (mIsInitialized)
    {
        mLabel += data;
    }
    else
    {
        assert(false);
    }
}

void DevicePort::_setInvalidDescriptionAndLabel(const std::string_view descriptionInput, const std::string_view labelInput)
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

std::pair<std::string, std::string> DevicePort::_getDeviceTypeDescriptionAndLabel() const
{
    return {"Device", ""};
}

void DevicePort::_initializeRequiredParameters()
{
    if (mFileRowNumber > 0u &&
        mFileColumnNumber > 0u &&
        !mIsInitialized)
    {
        mInputParametersCount = _getInputParametersCount();

        if (mInputParametersCount > 1u &&
            mInputParametersCount <= Data::c_MaxPortInputParametersCount) // there should be at least two parameters (device name and port number))
        {
            mInputData.reserve(mInputParametersCount);
            _registerRequiredParameters();
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        assert(false);
    }
}

void DevicePort::_initializeDescriptionAndLabel()
{
    const auto[deviceTypeDescription, deviceTypeLabel]{_getDeviceTypeDescriptionAndLabel()};
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
