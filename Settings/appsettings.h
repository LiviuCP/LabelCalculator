#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <memory>
#include <string>

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

    static const std::string scCentralHomeDir;
    static const std::string scDocumentsDirName;
    static const std::string scConnectionDefinitionsFilename;
    static const std::string scConnectionInputFilename;
    static const std::string scLabellingTableFilename;
    static const std::string scParsingErrorsFilename;

    static const char scPathSeparator;

    bool mIsInitialized;

    std::string mConnectionDefinitionsFile;
    std::string mConnectionInputFile;
    std::string mLabellingOutputFile;
    std::string mParsingErrorsFile;
    std::string mUsername;
};

#endif // APPSETTINGS_H
