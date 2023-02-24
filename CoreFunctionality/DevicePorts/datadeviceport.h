#ifndef DATADEVICEPORT_H
#define DATADEVICEPORT_H

#include "deviceport.h"
#include "deviceporttypes.h"

// Generic switch ports
class SwitchPort : public DevicePort
{
public:
    SwitchPort() = delete;

    void computeDescriptionAndLabel() override;

protected:
    // constructor needs to be protected as this class is a switch abstraction (derived classes are the concrete switches for which labels are being created)
    SwitchPort(const std::string& deviceUPosition, const DevicePortTypesInfo_t& switchPortTypesInfo, const size_t fileRowNumber, const size_t fileColumnNumber, const size_t requiredNumberOfParameters, const bool isSourceDevice);

    std::string mPortType;

private:
    void _handleNumberedPortType();

    const AllowedDataPortTypes_t mAllowedDataPortTypes;
    const bool mHasManagementPort;
};

// Ethernet switch ports
class LANSwitchPort : public SwitchPort
{
public:
    LANSwitchPort() = delete;
    LANSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);
};

// FC (SAN) switch ports
class SANSwitchPort : public SwitchPort
{
public:
    SANSwitchPort() = delete;
    SANSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);
};

// Infiniband switch ports
class InfinibandSwitchPort : public SwitchPort
{
public:
    InfinibandSwitchPort() = delete;
    InfinibandSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);
};

// KVM switch ports
class KVMSwitchPort : public SwitchPort
{
public:
    KVMSwitchPort() = delete;
    KVMSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);
};

// server ports (embedded (incl. management), Ethernet, FC, IB, etc)
class ServerPort : public DevicePort
{
public:
    ServerPort() = delete;
    ServerPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);

    virtual void computeDescriptionAndLabel() override;

protected:
    void _handleNumberedPortType();

    std::string mPortType;
};

// storage FC ports
class StoragePort : public DevicePort
{
public:
    StoragePort() = delete;
    StoragePort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);

    virtual void computeDescriptionAndLabel() override;

protected:
    std::string mControllerNr; // can be the controller number (for FC storage) or IO module number (for JBODs)
    std::string mPortType;
};

// blade system interconnect/management/uplink/downlink ports
class BladeServerPort : public DevicePort
{
public:
    BladeServerPort() = delete;
    BladeServerPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);

    virtual void computeDescriptionAndLabel() override;

protected:
    void _handleNumberedModuleType();

    std::string mModuleType; // blade system module type: interconnect, management module, power supply etc.
    std::string mModuleNumber; // data module number
};

#endif // DATADEVICEPORT_H
