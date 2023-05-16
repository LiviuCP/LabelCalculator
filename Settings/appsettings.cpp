#include <cstdio>

#include "appsettings.h"

#ifdef _WIN32
#include <windows.h>
#elif defined (__APPLE__) && defined (__MACH__)
#include "mach-o/dyld.h"
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

Core::Path_t AppSettings::getAppDataDir() const
{
    return mAppDataDir;
}

Core::Path_t AppSettings::getInputBackupDir() const
{
    return mInputBackupDir;
}

Core::Path_t AppSettings::getOutputBackupDir() const
{
    return mOutputBackupDir;
}

Core::Path_t AppSettings::getAppExamplesDir() const
{
    return mAppExamplesDir;
}

Core::Path_t AppSettings::getAppDataExamplesDir() const
{
    return mAppDataExamplesDir;
}

Core::Path_t AppSettings::getConnectionDefinitionsFile() const
{
    return mConnectionDefinitionsFile;
}

Core::Path_t AppSettings::getConnectionInputFile() const
{
    return mConnectionInputFile;
}

Core::Path_t AppSettings::getLabellingOutputFile() const
{
    return mLabellingOutputFile;
}

Core::Path_t AppSettings::getParsingErrorsFile() const
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
            _retrieveAppDir();
            _retrieveAppExamplesDir();

            mAppDataDir = scCentralHomeDir;
            mAppDataDir /= mUsername;
            mAppDataDir /= scDocumentsDirName;
            mAppDataDir /= scAppDataDirName;

            mInputBackupDir = mAppDataDir;
            mInputBackupDir /= scInputBackupDirName;
            mOutputBackupDir = mAppDataDir;
            mOutputBackupDir /= scOutputBackupDirName;
            mAppDataExamplesDir = mAppDataDir;
            mAppDataExamplesDir /= scAppDataExamplesDirName;

            mConnectionDefinitionsFile = mAppDataDir;
            mConnectionDefinitionsFile /= scConnectionDefinitionsFilename;
            mConnectionInputFile = mAppDataDir;
            mConnectionInputFile /= scConnectionInputFilename;
            mLabellingOutputFile = mAppDataDir;
            mLabellingOutputFile /= scLabellingTableFilename;
            mParsingErrorsFile /= mAppDataDir;
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
#elif defined (_WIN32)
    char buffer[scCharBufferLength / 8]; // for the username a smaller buffer can be used
    DWORD length{sizeof(buffer)};
    const BOOL c_IsValid{GetUserNameA(buffer, &length)};

    if (c_IsValid)
    {
        mUsername = std::string{buffer};
    }
#endif
}

void AppSettings::_retrieveAppDir()
{
    mAppDir.clear();
#if defined (__APPLE__) && defined (__MACH__)
    char path[scCharBufferLength];
    uint32_t pathSize = scCharBufferLength;
    if (0 == _NSGetExecutablePath(path, &pathSize))
    {
        mAppDir = Core::Path_t{path}.parent_path();
    }
#elif defined (__unix__)
    const Core::Path_t c_AppExecFilePath{std::filesystem::canonical("/proc/self/exe")};
    if (!c_AppExecFilePath.empty())
    {
        mAppDir = c_AppExecFilePath.parent_path();
    }
#elif defined (_WIN32)
    char path[scCharBufferLength];
    DWORD pathSize{scCharBufferLength};
    const DWORD c_Result{GetModuleFileNameA(NULL, path, pathSize)};

    if (c_Result > 0 && c_Result < scCharBufferLength)
    {
        mAppDir = Core::Path_t{path}.parent_path();
    }
#endif
    if (mAppDir.empty())
    {
        fprintf(stderr, "Warning! Application directory could not be retrieved. The buffer might be too small or another error occurred.");
    }
}

void AppSettings::_retrieveAppExamplesDir()
{
    mAppExamplesDir.clear();

    if (!mAppDir.empty())
    {
        for (const auto& dirEntry : std::filesystem::directory_iterator{mAppDir})
        {
            const Core::Path_t c_CurrentPath{dirEntry.path()};

            if (std::filesystem::is_directory(c_CurrentPath))
            {
                std::string dirName{c_CurrentPath.filename().string()};
                Core::convertStringCase(dirName, false);

                if (std::string::npos != dirName.find(scExamplesDirSearchKeyword, 0))
                {
                    mAppExamplesDir = c_CurrentPath;
                    break;
                }
            }
        }
    }
}

std::shared_ptr<AppSettings> AppSettings::s_pAppSettings{nullptr};
