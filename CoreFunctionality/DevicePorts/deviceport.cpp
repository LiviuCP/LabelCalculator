#include <algorithm>
#include <sstream>
#include <cassert>

#include "isubparserobserver.h"
#include "errorcodes.h"
#include "deviceportdata.h"
#include "deviceportutils.h"
#include "deviceport.h"

namespace Core = Utilities::Core;
namespace Ports = Utilities::DevicePorts;

DevicePort::DevicePort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice, const bool parseFromRowStart)
    : mDeviceUPosition{deviceUPosition}
    , mFileRowNumber{fileRowNumber}
    , mFileColumnNumber{1}
    , mIsSourceDevice{isSourceDevice}
    , mIsInitialized{false}
    , mpErrorHandler{nullptr}
    , m_pISubParserObserver{nullptr}
    , mParseFromRowStart{parseFromRowStart}
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

void DevicePort::parseInputData(std::vector<ErrorPtr>& parsingErrors)
{
    if (_isCurrentPositionAllowed() &&
        mInputParametersCount == mInputData.size() &&
        0u == std::count_if(mInputData.cbegin(), mInputData.cend(), [](const std::string* pElement) {return !pElement;}))
    {
        size_t currentParameter{0u};          // current field (cell) containining a device input parameter (e.g. device name)
        bool fewerCellsProvided{false};       // for checking if the "fewer cells" error occurred
        ErrorPtr lastError{nullptr};          // last found error

        parsingErrors.clear();

        while(currentParameter < mInputParametersCount) // check the "useful" fields (required input parameters for the device)
        {
            if (!mCurrentPosition.has_value()) // check if characters are available for current (required) field
            {
                fewerCellsProvided = true;
                break;
            }

            mCurrentPosition = Core::readDataField(mRawInputData, *mInputData[currentParameter], mCurrentPosition);

            if (0u == mInputData[currentParameter]->size())
            {
                lastError = mpErrorHandler->logError(static_cast<Error_t>(ErrorCode::EMPTY_CELL), mFileRowNumber, mFileColumnNumber);
                parsingErrors.push_back(lastError);
            }
            else if (Core::areInvalidCharactersContained(*mInputData[currentParameter]))
            {
                lastError = mpErrorHandler->logError(static_cast<Error_t>(ErrorCode::INVALID_CHARACTERS), mFileRowNumber, mFileColumnNumber);
                parsingErrors.push_back(lastError);
            }

            ++mFileColumnNumber;
            ++currentParameter;
        }

        while(!fewerCellsProvided && currentParameter < Data::c_MaxPortInputParametersCount) // check the padding fields (if any)
        {
            if (!mCurrentPosition.has_value())
            {
                if (mIsSourceDevice)
                {
                    fewerCellsProvided = true;
                    continue;
                }

                break;
            }

            std::string unusedField;
            mCurrentPosition = Core::readDataField(mRawInputData, unusedField, mCurrentPosition);
            ++mFileColumnNumber;
            ++currentParameter;
        }

        if (fewerCellsProvided) // handle parameter-independent errors
        {
            lastError = mpErrorHandler->logError(static_cast<Error_t>(ErrorCode::FEWER_CELLS), mFileRowNumber, mFileColumnNumber);
            parsingErrors.push_back(lastError);
        }
    }

    if (m_pISubParserObserver) // notify parser no matter the (sub)parsing outcome
    {
        m_pISubParserObserver->subParserFinished(this);
    }
}

Index_t DevicePort::getCurrentPosition() const
{
    Index_t currentPosition{mCurrentPosition};

    if (mCurrentPosition.has_value() && !mParseFromRowStart)
    {
        if (const size_t c_CurrentPosition{mCurrentPosition.value()}, c_PaddingSize{Data::c_Padding.size()};
            c_CurrentPosition >= c_PaddingSize)
        {
            currentPosition = c_CurrentPosition - c_PaddingSize;
        }
        else
        {
            currentPosition.reset();
            assert(false);
        }
    }

    return currentPosition;
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

void DevicePort::setSubParserObserver(ISubParserObserver* const pISubParserObserver)
{
    m_pISubParserObserver = pISubParserObserver;
}

void DevicePort::setErrorHandler(const ErrorHandlerPtr pErrorHandler)
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

void DevicePort::setRawInputData(const std::string_view rawInputData)
{
    mRawInputData.clear();
    mCurrentPosition.reset();

    if (!rawInputData.empty())
    {
        if (!mParseFromRowStart)
        {
            mRawInputData.append(Data::c_Padding);
        }

        mCurrentPosition = mRawInputData.size();
        mRawInputData.append(rawInputData);
    }
}

void DevicePort::_registerRequiredParameter(std::string* const pRequiredParameter)
{
    if (pRequiredParameter)
    {
        mInputData.push_back(pRequiredParameter);
    }
}

void DevicePort::_appendDataToDescription(const std::string_view data)
{
    if (mIsInitialized)
    {
        mDescription += data;
    }
}

void DevicePort::_appendDataToLabel(const std::string_view data)
{
    if (mIsInitialized)
    {
        mLabel += data;
    }
}

void DevicePort::_setInvalidDescriptionAndLabel(const std::string_view descriptionInput, const std::string_view labelInput)
{
    if (const size_t c_DescriptionInputLength{descriptionInput.size()}; c_DescriptionInputLength > 0u)
    {
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
}

void DevicePort::_checkLabel()
{
    if (const size_t c_LabelCharsCount{mLabel.size()}; c_LabelCharsCount > 0u)
    {
        if (std::string::npos == mLabel.find(Ports::c_LabelErrorText) &&
            c_LabelCharsCount > Data::c_MaxLabelCharsCount)
        {
            std::stringstream stream;
            const ssize_t c_DeltaCharsCount{static_cast<ssize_t>(c_LabelCharsCount - Data::c_MaxLabelCharsCount)};
            stream << Ports::c_MaxLabelCharsCountExceededErrorText << c_DeltaCharsCount;

            _setInvalidDescriptionAndLabel(stream.str());
        }
    }
    else
    {
        _setInvalidDescriptionAndLabel(Ports::c_LabelErrorText, Ports::c_LabelErrorText); // defensive programming
        assert(false);
    }
}

std::pair<std::string, std::string> DevicePort::_getDeviceTypeDescriptionAndLabel() const
{
    return {"Device", ""};
}

bool DevicePort::_isCurrentPositionAllowed() const
{
    bool isAllowed{true};

    if (mCurrentPosition.has_value())
    {
        isAllowed = (mCurrentPosition <= mRawInputData.size()); // end position (similar to end iterator) is also allowed
    }

    return isAllowed;
}

void DevicePort::_initializeRequiredParameters()
{
    if (mFileRowNumber > 0u &&
        mFileColumnNumber > 0u &&
        !mIsInitialized)
    {
        mInputParametersCount = _getInputParametersCount();

        // there should be at least two parameters (device name and port number))
        if (mInputParametersCount > 1u &&
            mInputParametersCount <= Data::c_MaxPortInputParametersCount)
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
    if (!mIsInitialized)
    {
        const auto[deviceTypeDescription, deviceTypeLabel]{_getDeviceTypeDescriptionAndLabel()};

        mDescription = deviceTypeDescription.size() > 0 ? deviceTypeDescription : "Device";
        mDescription += " placed at U";
        mDescription += mDeviceUPosition;

        mLabel = "U" + mDeviceUPosition;

        if (deviceTypeLabel.size() > 0)
        {
            mLabel += "_";
            mLabel += deviceTypeLabel;
        }
    }
}
