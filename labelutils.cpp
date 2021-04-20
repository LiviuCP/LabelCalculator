#include <iostream>
#include <cassert>
#include <algorithm>

#include "labelutils.h"

int readDataField(const std::string& src, std::string& dest, const int index)
{
    const int c_Length{static_cast<int>(src.size())}; // no issue in converting to int as the size is reasonable (same for other similar situations in this project)

    int nextIndex{-1};

    if(c_Length > 0)
    {
        assert(index < c_Length);

        dest.clear();

        int currentIndex{index};

        if (c_CSVSeparator == src[currentIndex])
        {
            ++currentIndex;
        }

        while(currentIndex < c_Length)
        {
            if(src[currentIndex] != c_CSVSeparator)
            {
                dest += src[currentIndex];
            }
            else
            {
                break;
            }

            ++currentIndex;
        }

        if(currentIndex != c_Length)
        {
            nextIndex = currentIndex;
        }
    }

    return nextIndex;
}

bool init(std::string& connectionsFilename, std::string& inputFilename, std::string& outputFilename, std::string& errorFilename, std::ofstream& errorStream)
{
    using namespace std;

    bool commonFilesSuccessfullyOpened{false}; // for each option corresponding files (e.g. connectioninput.csv) should be successfully opened in the desired mode (read/write)

    ifstream inputStream{c_ConfigurationFilePath};

    if(inputStream.is_open())
    {
        // username is used for determining the paths of all other relevant files (see below)
        string username;
        getline(inputStream, username);

        const string c_AppFilesDir{c_HomeDirParent + c_PathSeparator + username + c_PathSeparator + c_DocumentsDirName + c_PathSeparator};
        connectionsFilename = c_AppFilesDir + c_ConnectionDefinitionsFilename;
        inputFilename = c_AppFilesDir + c_ConnectionInputFilename;
        outputFilename = c_AppFilesDir + c_LabellingTableFilename;
        errorFilename = c_AppFilesDir + c_ErrorFilename;

        errorStream.open(errorFilename);

        if (errorStream.is_open())
        {
            commonFilesSuccessfullyOpened = true;
        }
        else
        {
            system(c_ClearScreenCommand.c_str());
            cerr << "Error! File " << c_ErrorFilename << " cannot be opened for writing" << endl;
            cerr << "File path: " << errorFilename << endl;
        }
    }
    else
    {
        system(c_ClearScreenCommand.c_str());
        cerr << "Error! File " << c_ConfigurationFilePath << " cannot be opened for reading" << endl;
    }

    return commonFilesSuccessfullyOpened;
}

bool enableReadWriteOperations(std::ifstream& inputStream, std::ofstream& outputStream, const std::string& inputFilename, const std::string& outputFilename)
{
    using namespace std;

    bool filesSuccessfullyOpened{false};

    inputStream.open(inputFilename);

    if (inputStream.is_open())
    {
        outputStream.open(outputFilename);

        if(outputStream.is_open())
        {
            filesSuccessfullyOpened = true;
        }
        else
        {
            system(c_ClearScreenCommand.c_str());
            cerr << "Error! Output file cannot be opened for writing" << endl;
            cerr << "File path: "<< outputFilename << endl;
        }
    }
    else
    {
        system(c_ClearScreenCommand.c_str());
        std::cerr << "Error! Input file cannot be opened for reading" << std::endl;
        std::cerr << "File path: "<< inputFilename << std::endl;
    }

    return filesSuccessfullyOpened;
}

void writeOutputToFile(std::ofstream& outputStream, const std::vector<std::string>& inputRows, const std::string& header)
{
    assert(outputStream.is_open());

    outputStream << header << std::endl;

    for (const auto& payloadRow : inputRows)
    {
        outputStream << payloadRow << std::endl;
    }
}

DeviceTypeID getDeviceTypeID(const std::string& deviceType)
{
    DeviceTypeID deviceTypeID{DeviceTypeID::UNKNOWN_DEVICE};

    std::string deviceTypeLowerCase{deviceType};
    convertStringCase(deviceTypeLowerCase, false);

    std::map<std::string, DeviceTypeID>::const_iterator it{c_DeviceTypeToIDMapping.find(deviceTypeLowerCase)};

    if (it != c_DeviceTypeToIDMapping.cend())
    {
        deviceTypeID = it->second;
    }

    return deviceTypeID;
}

std::string getDeviceType(DeviceTypeID deviceTypeID)
{
    std::string deviceType;

    for (std::map<std::string, DeviceTypeID>::const_iterator it{c_DeviceTypeToIDMapping.cbegin()}; it != c_DeviceTypeToIDMapping.cend(); ++it)
    {
        if (deviceTypeID == it->second)
        {
            deviceType = it->first;
            break;
        }
    }

    return deviceType;
}

/* converts a string "in place" to upper- or lowercase
*/
void convertStringCase(std::string& str, bool upperCase)
{
    auto transformation = upperCase ? [](char c){return std::toupper(c);} : [](char c){return std::tolower(c);};
    std::transform(str.begin(), str.end(), str.begin(), transformation);
}

bool isDigitString(const std::string& str)
{
    bool hasOnlyDigits{true};

    for (const auto& c : str)
    {
        if (!std::isdigit(c))
        {
            hasOnlyDigits = false;
            break;
        }
    }

    return hasOnlyDigits;
}

bool areInvalidCharactersContained(const std::string& str)
{
    bool containsInvalidCharacters{false};

    for (const auto& c : str)
    {
        if (!std::isdigit(c) &&
            !std::isalpha(c) &&
            '-' != c)
        {
            containsInvalidCharacters = true;
            break;
        }
    }

    return containsInvalidCharacters;
}
