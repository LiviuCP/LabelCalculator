#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <memory>
#include <string>
#include <string_view>

class AppSettings
{
public:
    static std::shared_ptr<AppSettings> getInstance();

    bool areSettingsValid() const;

    std::string getUsername() const;
    std::string getConnectionDefinitionsFile() const;
    std::string getConnectionInputFile() const;
    std::string getLabellingOutputFile() const;
    std::string getParsingErrorsFile() const;

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

    static std::shared_ptr<AppSettings> s_pAppSettings;

#if defined (__APPLE__) && defined (__MACH__)
    static constexpr std::string_view scCentralHomeDir{"/Users"};
#elif defined (__unix__)
    static constexpr std::string_view scCentralHomeDir{"/home"};
#else
    static constexpr std::string_view scCentralHomeDir{"C:\\Users"};
#endif
    static constexpr std::string_view scDocumentsDirName{"Documents"};
    static constexpr std::string_view scConnectionDefinitionsFilename{"connectiondefinitions.csv"};
    static constexpr std::string_view scConnectionInputFilename{"connectioninput.csv"};
    static constexpr std::string_view scLabellingTableFilename{"labellingtable.csv"};
    static constexpr std::string_view scParsingErrorsFilename{"error.txt"};

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
    static constexpr char scPathSeparator{'/'};
#else
    static constexpr char scPathSeparator{'\\'};
#endif

    bool mIsInitialized;

    std::string mConnectionDefinitionsFile;
    std::string mConnectionInputFile;
    std::string mLabellingOutputFile;
    std::string mParsingErrorsFile;
    std::string mUsername;
};

#endif // APPSETTINGS_H
