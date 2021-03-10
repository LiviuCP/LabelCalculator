#ifndef DATADEVICE_H
#define DATADEVICE_H

#include "device.h"

// Ethernet switch ports
class LANSwitch : public Device
{
public:
    LANSwitch() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    LANSwitch(int first);

    void buildDescriptionText();
    void buildLabelText();

protected:
    std::string mPortNumber;
    std::string mPlaceholder1;
    std::string mPlaceholder2;
};

// FC (SAN) switch ports
class SANSwitch : public Device
{
public:
    SANSwitch() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    SANSwitch(int first);

    void buildDescriptionText();
    void buildLabelText();

protected:
    std::string mPortNumber;
    std::string mPlaceholder1;
    std::string mPlaceholder2;
};

// Infiniband switch ports
class InfinibandSwitch : public Device
{
public:
    InfinibandSwitch() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    InfinibandSwitch(int first);

    void buildDescriptionText();
    void buildLabelText();

protected:
    std::string mPortNumber;
    std::string mPlaceholder1;
    std::string mPlaceholder2;
};

// KVM switch ports
class KVMSwitch : public Device
{
public:
    KVMSwitch() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    KVMSwitch(int first);

    void buildDescriptionText();
    void buildLabelText();

protected:
    std::string mPortNumber;
    std::string mPlaceholder1;
    std::string mPlaceholder2;
};

// server ports (embedded (incl. management), Ethernet, FC, IB, etc)
class Server : public Device
{
public:
    Server() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    Server(int first);

    void buildDescriptionText();
    void buildLabelText();

protected:
    std::string mPortType;
    std::string mPortNumber;
    std::string mPlaceholder;
};

// storage FC ports
class Storage : public Device
{
public:
    Storage() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    Storage(int first);

    void buildDescriptionText();
    void buildLabelText();

protected:
    std::string mControllerNr; // can be the controller number (for FC storage) or IO module number (for JBODs)
    std::string mPortNr;
    std::string mPlaceholder;
};

// blade system interconnect/management/uplink/downlink ports
class BladeServer : public Device
{
public:
    BladeServer() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    BladeServer(int first);

    void buildDescriptionText();
    void buildLabelText();

protected:
    std::string mModuleType; // blade system module type: interconnect, management module etc.
    std::string mModuleNumber; // data module number
    std::string mPortNumber;
};

#endif // DATADEVICE_H
