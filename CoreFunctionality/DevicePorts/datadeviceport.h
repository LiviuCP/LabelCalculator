#ifndef DATADEVICEPORT_H
#define DATADEVICEPORT_H

#include "deviceport.h"
#include "deviceporttypes.h"

// Generic switch ports
class SwitchPort : public DevicePort
{
public:
    SwitchPort() = delete;

    virtual void updateDescriptionAndLabel() override;

protected:
    // constructor needs to be protected as this class is a switch abstraction (derived classes are the concrete switches for which labels are being created)
    SwitchPort(const std::string_view deviceUPosition, const DevicePortTypesInfo_t& switchPortTypesInfo, const size_t fileRowNumber, const bool isSourceDevice);

    virtual void _registerRequiredParameters() override;
    std::string _getPortType() const;

private:
    void _handleNumberedPortType();

    const AllowedDataPortTypes_t mAllowedDataPortTypes;
    const bool mIsManagementPortAllowed;

    std::string mPortType;
    std::string mPortNumber;
};

// Generic director (multiple blades switch) ports
class DirectorPort : public SwitchPort
{
public:
    DirectorPort() = delete;

    virtual void updateDescriptionAndLabel() override;

protected:
    // constructor needs to be protected as this class is a director abstraction (derived classes are the concrete directors for which labels are being created)
    DirectorPort(const std::string_view deviceUPosition, const AllowedDataPortTypes_t& directorDataPortTypesInfo, const size_t fileRowNumber, const bool isSourceDevice);

    virtual void _registerRequiredParameters() override;

private:
    std::string mBladeNumber;
};

// LAN (Ethernet) switch ports
class LANSwitchPort final : public SwitchPort
{
public:
    LANSwitchPort() = delete;
    LANSwitchPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice);

protected:
    virtual size_t _getInputParametersCount() const override;
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const override;
};

// LAN (Ethernet) director ports
class LANDirectorPort final : public DirectorPort
{
public:
    LANDirectorPort() = delete;
    LANDirectorPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice);

protected:
    virtual size_t _getInputParametersCount() const override;
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const override;
};

// SAN (FC) switch ports
class SANSwitchPort final : public SwitchPort
{
public:
    SANSwitchPort() = delete;
    SANSwitchPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice);

protected:
    virtual size_t _getInputParametersCount() const override;
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const override;
};

// SAN (FC, FCIP, iSCSI) director ports
class SANDirectorPort final : public DirectorPort
{
public:
    SANDirectorPort() = delete;
    SANDirectorPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice);

protected:
    virtual size_t _getInputParametersCount() const override;
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const override;
};

// Infiniband switch ports
class InfinibandSwitchPort final : public SwitchPort
{
public:
    InfinibandSwitchPort() = delete;
    InfinibandSwitchPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice);

protected:
    virtual size_t _getInputParametersCount() const override;
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const override;
};

// KVM switch ports
class KVMSwitchPort final : public SwitchPort
{
public:
    KVMSwitchPort() = delete;
    KVMSwitchPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice);

protected:
    virtual size_t _getInputParametersCount() const override;
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const override;
};

// server ports (embedded (incl. management), Ethernet, FC, IB, etc)
class ServerPort : public DevicePort
{
public:
    ServerPort() = delete;
    ServerPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice);

    virtual void updateDescriptionAndLabel() override;

protected:
    virtual void _registerRequiredParameters() override;
    virtual size_t _getInputParametersCount() const override;
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const override;
    void _handleNumberedPortType();

private:
    std::string mPortType;
    std::string mPortNumber;
};

// scalable server ports; a scalable server contains several slots within chassis, each slot being an independent server; one server might also occupy more slots
class ScalableServerPort : public ServerPort
{
public:
    ScalableServerPort() = delete;
    ScalableServerPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice);

    virtual void updateDescriptionAndLabel() override;

protected:
    virtual void _registerRequiredParameters() override;
    virtual size_t _getInputParametersCount() const override;
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const override;

    std::string mSlotNumber;
};

// storage ports (FC, SAS, etc)
class StoragePort : public DevicePort
{
public:
    StoragePort() = delete;
    StoragePort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice);

    virtual void updateDescriptionAndLabel() override;

protected:
    virtual void _registerRequiredParameters() override;
    virtual size_t _getInputParametersCount() const override;
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const override;

    void _handleNumberedPortType();
    void _handleManagementPort();

private:
    std::string mControllerNr; // can be the controller number (for FC storage) or IO module number (for JBODs)
    std::string mPortType;
    std::string mPortNumber;
};

// blade system interconnect/management/uplink/downlink ports
class BladeServerPort : public DevicePort
{
public:
    BladeServerPort() = delete;
    BladeServerPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice);

    virtual void updateDescriptionAndLabel() override;

protected:
    virtual void _registerRequiredParameters() override;
    virtual size_t _getInputParametersCount() const override;
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const override;
    void _handleNumberedModuleType();

private:
    std::string mModuleType; // blade system module type: interconnect, management module, power supply etc.
    std::string mModuleNumber; // data module number
    std::string mPortNumber;
};

#endif // DATADEVICEPORT_H
