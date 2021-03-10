#ifndef POWERDEVICE_H
#define POWERDEVICE_H

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
    std::string mPlacementType; // rack placement (vertical left/right, horizontal)
    std::string mLoadSegmentNumber;
    std::string mPortNumber;
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
    std::string mPlacementType; // rack placement (left/right)
    std::string mPortNumber;
    std::string mPlaceholder;
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
    std::string mLoadSegmentNumber;
    std::string mPortNumber;
    std::string mPlaceholder;
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
    std::string mPowerSupplyNumber;
    std::string mPlaceholder1;
    std::string mPlaceholder2;
};

#endif // POWERDEVICE_H
