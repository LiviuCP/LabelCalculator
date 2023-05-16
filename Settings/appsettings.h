#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include "coreutils.h"

namespace Core = Utilities::Core;

class AppSettings
{
public:
    static std::shared_ptr<AppSettings> getInstance();

    bool areSettingsValid() const;

    std::string getUsername() const;

    Core::Path_t getAppDataDir() const;
    Core::Path_t getInputBackupDir() const;
    Core::Path_t getOutputBackupDir() const;
    Core::Path_t getAppExamplesDir() const;
    Core::Path_t getAppDataExamplesDir() const;
    Core::Path_t getConnectionDefinitionsFile() const;
    Core::Path_t getConnectionInputFile() const;
    Core::Path_t getLabellingOutputFile() const;
    Core::Path_t getParsingErrorsFile() const;

private:
    /* Private constructor (singleton)
    */
    AppSettings();

    /* This function is responsible for building the application settings object
       It creates the paths of all essential files and dirs
    */
    void _init();

    /* Cross-platform function that retrieves the username from OS system settings
    */
    void _retrieveUsername();

    /* Cross-platform function that retrieves the application directory from OS system settings and converts it into a canonical path
    */
    void _retrieveAppDir();

    /* This function retrieves the examples directory located within build folder (source examples directory) - if any
    */
    void _retrieveAppExamplesDir();

    static std::shared_ptr<AppSettings> s_pAppSettings;

#if defined (__APPLE__) && defined (__MACH__)
    static constexpr std::string_view scCentralHomeDir{"/Users"};
#elif defined (__unix__)
    static constexpr std::string_view scCentralHomeDir{"/home"};
#else
    static constexpr std::string_view scCentralHomeDir{"C:\\Users"};
#endif
    static constexpr std::string_view scDocumentsDirName{"Documents"};
    static constexpr std::string_view scAppDataDirName{"LabelCalculatorData"};
    static constexpr std::string_view scInputBackupDirName{"LabellingInputBackup"};
    static constexpr std::string_view scOutputBackupDirName{"LabellingOutputBackup"};
    static constexpr std::string_view scAppDataExamplesDirName{"ExampleFiles"};

    static constexpr std::string_view scConnectionDefinitionsFilename{"connectiondefinitions.csv"};
    static constexpr std::string_view scConnectionInputFilename{"connectioninput.csv"};
    static constexpr std::string_view scLabellingTableFilename{"labellingtable.csv"};
    static constexpr std::string_view scParsingErrorsFilename{"error.txt"};

    static constexpr std::string_view scExamplesDirSearchKeyword{"example"};
    static constexpr size_t scCharBufferLength{1024}; // used for reading various strings of previously unknown size (e.g. application dir path)

    bool mIsInitialized;

    Core::Path_t mAppDir;             // build folder, directory where the application executable resides
    Core::Path_t mAppDataDir;         // data folder, directory where the input/output files processed by application reside
    Core::Path_t mInputBackupDir;     // backup for input files
    Core::Path_t mOutputBackupDir;    // backup for output files
    Core::Path_t mAppExamplesDir;     // examples dir from build folder
    Core::Path_t mAppDataExamplesDir; // examples dir copied from build folder to app data directory

    Core::Path_t mConnectionDefinitionsFile;
    Core::Path_t mConnectionInputFile;
    Core::Path_t mLabellingOutputFile;
    Core::Path_t mParsingErrorsFile;

    std::string mUsername;
};

#endif // APPSETTINGS_H
