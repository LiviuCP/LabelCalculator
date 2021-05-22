#ifndef DATADEVICEPORT_H
#define DATADEVICEPORT_H

#include "deviceport.h"

// Ethernet switch ports
class LANSwitchPort : public DevicePort
{
public:
    LANSwitchPort() = delete;
    LANSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPortType;
};

// FC (SAN) switch ports
class SANSwitchPort : public DevicePort
{
public:
    SANSwitchPort() = delete;
    SANSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPortType;
};

// Infiniband switch ports
class InfinibandSwitchPort : public DevicePort
{
public:
    InfinibandSwitchPort() = delete;
    InfinibandSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPortType;
};

// KVM switch ports
class KVMSwitchPort : public DevicePort
{
public:
    KVMSwitchPort() = delete;
    KVMSwitchPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPortType;
};

// server ports (embedded (incl. management), Ethernet, FC, IB, etc)
class ServerPort : public DevicePort
{
public:
    ServerPort() = delete;
    ServerPort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPortType;
};

// storage FC ports
class StoragePort : public DevicePort
{
public:
    StoragePort() = delete;
    StoragePort(const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);

    void computeDescriptionAndLabel() override;

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

    void computeDescriptionAndLabel() override;

protected:
    std::string mModuleType; // blade system module type: interconnect, management module, power supply etc.
    std::string mModuleNumber; // data module number
};

#endif // DATADEVICEPORT_H
