#ifndef DATADEVICE_H
#define DATADEVICE_H

#include "device.h"

// Ethernet switch ports
class LANSwitch : public Device
{
public:
    LANSwitch() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    LANSwitch(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPlaceholder1;
    std::string mPlaceholder2;
};

// FC (SAN) switch ports
class SANSwitch : public Device
{
public:
    SANSwitch() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    SANSwitch(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPlaceholder1;
    std::string mPlaceholder2;
};

// Infiniband switch ports
class InfinibandSwitch : public Device
{
public:
    InfinibandSwitch() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    InfinibandSwitch(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPlaceholder1;
    std::string mPlaceholder2;
};

// KVM switch ports
class KVMSwitch : public Device
{
public:
    KVMSwitch() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    KVMSwitch(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPlaceholder1;
    std::string mPlaceholder2;
};

// server ports (embedded (incl. management), Ethernet, FC, IB, etc)
class Server : public Device
{
public:
    Server() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    Server(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPortType;
    std::string mPlaceholder;
};

// storage FC ports
class Storage : public Device
{
public:
    Storage() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    Storage(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mControllerNr; // can be the controller number (for FC storage) or IO module number (for JBODs)
    std::string mPlaceholder;
};

// blade system interconnect/management/uplink/downlink ports
class BladeServer : public Device
{
public:
    BladeServer() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    BladeServer(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mModuleType; // blade system module type: interconnect, management module etc.
    std::string mModuleNumber; // data module number
};

#endif // DATADEVICE_H
