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

        const size_t c_UserNameCharsCount{mUsername.size()};

        if (c_UserNameCharsCount > 0u)
        {
            std::string appFilesDir;

            appFilesDir.reserve(scCentralHomeDir.size() + c_UserNameCharsCount + scDocumentsDirName.size() + 3 * sizeof(scPathSeparator));

            appFilesDir += scCentralHomeDir;
            appFilesDir += scPathSeparator;
            appFilesDir += mUsername;
            appFilesDir += scPathSeparator;
            appFilesDir += scDocumentsDirName;
            appFilesDir += scPathSeparator;

            mConnectionDefinitionsFile = appFilesDir + scConnectionDefinitionsFilename.data();
            mConnectionInputFile = appFilesDir + scConnectionInputFilename.data();
            mLabellingOutputFile = appFilesDir + scLabellingTableFilename.data();
            mParsingErrorsFile = appFilesDir + scParsingErrorsFilename.data();

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
