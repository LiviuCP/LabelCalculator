#include "appsettings.h"

#ifdef _WIN32
#include <windows.h>
#endif

AppSettings::AppSettings()
    : mIsInitialized{false}
{
    _init();
}

std::shared_ptr<AppSettings> AppSettings::getInstance()
{
    if (nullptr == s_pAppSettings)
    {
        s_pAppSettings.reset(new AppSettings);
    }

    return s_pAppSettings;
}

bool AppSettings::areSettingsValid() const
{
    return mIsInitialized;
}

std::string AppSettings::getUsername() const
{
    return mUsername;
}

std::string AppSettings::getConnectionDefinitionsFile() const
{
    return mConnectionDefinitionsFile;
}

std::string AppSettings::getConnectionInputFile() const
{
    return mConnectionInputFile;
}

std::string AppSettings::getLabellingOutputFile() const
{
    return mLabellingOutputFile;
}

std::string AppSettings::getParsingErrorsFile() const
{
    return mParsingErrorsFile;
}

void AppSettings::_init()
{
    if (!mIsInitialized)
    {
        _retrieveUsername();

        if (mUsername.size() > 0u)
        {
            const std::string c_AppFilesDir{scCentralHomeDir + scPathSeparator + mUsername + scPathSeparator + scDocumentsDirName + scPathSeparator};

            mConnectionDefinitionsFile = c_AppFilesDir + scConnectionDefinitionsFilename;
            mConnectionInputFile = c_AppFilesDir + scConnectionInputFilename;
            mLabellingOutputFile = c_AppFilesDir + scLabellingTableFilename;
            mParsingErrorsFile = c_AppFilesDir + scParsingErrorsFilename;

            mIsInitialized = true;
        }
    }
}

void AppSettings::_retrieveUsername()
{
    mUsername.clear();
#if defined (__APPLE__) && defined (__MACH__)
    char* pUsername{getenv("USER")};

    if (pUsername != nullptr)
    {
        mUsername = pUsername;
    }
#elif defined (__unix__)
    char* pUsername{getenv("USERNAME")};

    if (pUsername == nullptr)
    {
        pUsername = getenv("USER");
    }

    if (pUsername != nullptr)
    {
        mUsername = pUsername;
    }
#elif defined(_WIN32)
    char buffer[100];
    DWORD length{sizeof(buffer)};
    const BOOL c_IsValid{GetUserNameA(buffer, &length)};

    if (c_IsValid)
    {
        mUsername = std::string{buffer};
    }
#endif
}

std::shared_ptr<AppSettings> AppSettings::s_pAppSettings{nullptr};

#if defined (__APPLE__) && defined (__MACH__)
const std::string AppSettings::scCentralHomeDir{"/Users"};
#elif defined (__unix__)
const std::string AppSettings::scCentralHomeDir{"/home"};
#else
const std::string AppSettings::scCentralHomeDir{"C:\\Users"};
#endif

const std::string AppSettings::scDocumentsDirName{"Documents"};
const std::string AppSettings::scConnectionDefinitionsFilename{"connectiondefinitions.csv"};
const std::string AppSettings::scConnectionInputFilename{"connectioninput.csv"};
const std::string AppSettings::scLabellingTableFilename{"labellingtable.csv"};
const std::string AppSettings::scParsingErrorsFilename{"error.txt"};

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
const char AppSettings::scPathSeparator{'/'};
#else
const char AppSettings::scPathSeparator{'\\'};
#endif
