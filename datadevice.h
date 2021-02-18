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
    string mPortNumber;
    string mPlaceholder1;
    string mPlaceholder2;
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
    string mPortNumber;
    string mPlaceholder1;
    string mPlaceholder2;
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
    string mPortNumber;
    string mPlaceholder1;
    string mPlaceholder2;
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
    string mPortNumber;
    string mPlaceholder1;
    string mPlaceholder2;
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
    string mPortType;
    string mPortNumber;
    string mPlaceholder;
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
    string mControllerNr; // can be the controller number (for FC storage) or IO module number (for JBODs)
    string mPortNr;
    string mPlaceholder;
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
    string mModuleType; // blade system module type: interconnect, management module etc.
    string mModuleNumber; // data module number
    string mPortNumber;
};

#endif // DATADEVICE_H
