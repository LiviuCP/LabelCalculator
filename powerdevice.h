#ifndef POWERDEVICE_H
#define POWERDEVICE_H

#include <map>

#include "device.h"

// PDU ports
class PDU : public Device
{
public:
    PDU() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    PDU(int first);

    void buildDescriptionText();
    void buildLabelText();

protected:
    string mPlacementType; // rack placement (vertical left/right, horizontal)
    string mLoadSegmentNumber;
    string mPortNumber;
};

// PDU extension bar ports
class ExtensionBar : public Device
{
public:
    ExtensionBar() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    ExtensionBar(int first);

    void buildDescriptionText();
    void buildLabelText();

protected:
    string mPlacementType; // rack placement (left/right)
    string mPortNumber;
    string mPlaceholder;
};

// UPS ports
class UPS : public Device
{
public:
    UPS() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    UPS(int first);

    void buildDescriptionText();
    void buildLabelText();

protected:
    string mLoadSegmentNumber;
    string mPortNumber;
    string mPlaceholder;
};

// power supply ports (any power supply no matter the device type)
class PowerSupply : public Device
{
public:
    PowerSupply() = delete;

    // first = 0: second device on the csv row; first = 1: first device on the row
    PowerSupply(int first);

    void buildDescriptionText();
    void buildLabelText();

protected:
    string mPowerSupplyNumber;
    string mPlaceholder1;
    string mPlaceholder2;
};

#endif // POWERDEVICE_H
