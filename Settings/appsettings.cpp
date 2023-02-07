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

Path_t AppSettings::getConnectionDefinitionsFile() const
{
    return mConnectionDefinitionsFile;
}

Path_t AppSettings::getConnectionInputFile() const
{
    return mConnectionInputFile;
}

Path_t AppSettings::getLabellingOutputFile() const
{
    return mLabellingOutputFile;
}

Path_t AppSettings::getParsingErrorsFile() const
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
            Path_t appFilesDir{scCentralHomeDir};
            appFilesDir /= mUsername;
            appFilesDir /= scDocumentsDirName;

            mConnectionDefinitionsFile = appFilesDir;
            mConnectionDefinitionsFile /= scConnectionDefinitionsFilename;
            mConnectionInputFile = appFilesDir;
            mConnectionInputFile /= scConnectionInputFilename;
            mLabellingOutputFile = appFilesDir;
            mLabellingOutputFile /= scLabellingTableFilename;
            mParsingErrorsFile = appFilesDir;
            mParsingErrorsFile /= scParsingErrorsFilename;

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
