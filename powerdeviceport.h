#ifndef POWERDEVICEPORT_H
#define POWERDEVICEPORT_H

#include "deviceport.h"

// PDU ports
class PDUPort : public DevicePort
{
public:
    PDUPort() = delete;
    PDUPort(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mDevicePlacementType; // rack placement (vertical left/right, horizontal)
    std::string mLoadSegmentNumber;
};

// PDU extension bar ports
class ExtensionBarPort : public DevicePort
{
public:
    ExtensionBarPort() = delete;
    ExtensionBarPort(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mDevicePlacementType; // rack placement (left/right)
};

// UPS ports
class UPSPort : public DevicePort
{
public:
    UPSPort() = delete;
    UPSPort(bool isSourceDevice);

    void computeDescriptionAndLabel() override;

protected:
    std::string mLoadSegmentNumber;
};

#endif // POWERDEVICEPORT_H
