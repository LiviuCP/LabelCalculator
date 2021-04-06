#ifndef POWERDEVICE_H
#define POWERDEVICE_H

#include "device.h"

// PDU ports
class PDU : public Device
{
public:
    PDU() = delete;
    PDU(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPlacementType; // rack placement (vertical left/right, horizontal)
    std::string mLoadSegmentNumber;
};

// PDU extension bar ports
class ExtensionBar : public Device
{
public:
    ExtensionBar() = delete;
    ExtensionBar(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPlacementType; // rack placement (left/right)
    std::string mPlaceholder;
};

// UPS ports
class UPS : public Device
{
public:
    UPS() = delete;
    UPS(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mLoadSegmentNumber;
    std::string mPlaceholder;
};

// power supply ports (any power supply no matter the device type)
class PowerSupply : public Device
{
public:
    PowerSupply() = delete;
    PowerSupply(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mPlaceholder1;
    std::string mPlaceholder2;
};

#endif // POWERDEVICE_H
