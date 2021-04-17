#ifndef DATADEVICEPORT_H
#define DATADEVICEPORT_H

#include "deviceport.h"

// Ethernet switch ports
class LANSwitchPort : public DevicePort
{
public:
    LANSwitchPort() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    LANSwitchPort(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPortType;
};

// FC (SAN) switch ports
class SANSwitchPort : public DevicePort
{
public:
    SANSwitchPort() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    SANSwitchPort(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPortType;
};

// Infiniband switch ports
class InfinibandSwitchPort : public DevicePort
{
public:
    InfinibandSwitchPort() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    InfinibandSwitchPort(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPortType;
};

// KVM switch ports
class KVMSwitchPort : public DevicePort
{
public:
    KVMSwitchPort() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    KVMSwitchPort(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPortType;
};

// server ports (embedded (incl. management), Ethernet, FC, IB, etc)
class ServerPort : public DevicePort
{
public:
    ServerPort() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    ServerPort(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPortType;
};

// storage FC ports
class StoragePort : public DevicePort
{
public:
    StoragePort() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    StoragePort(bool isSourceDevice);

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

    // first = 0: second device on the csv row; first = 1: first device on the row
    BladeServerPort(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mModuleType; // blade system module type: interconnect, management module, power supply etc.
    std::string mModuleNumber; // data module number
};

#endif // DATADEVICEPORT_H
