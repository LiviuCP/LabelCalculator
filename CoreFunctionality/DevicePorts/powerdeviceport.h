#ifndef POWERDEVICEPORT_H
#define POWERDEVICEPORT_H

#include "deviceport.h"

// PDU ports
class PDUPort : public DevicePort
{
public:
    PDUPort() = delete;
    PDUPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice);

    virtual void updateDescriptionAndLabel() override;

protected:
    virtual void _registerRequiredParameters() override;
    virtual size_t _getInputParametersCount() const override;
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const override;

private:
    std::string mDevicePlacementType; // rack placement (vertical left/right, horizontal)
    std::string mLoadSegmentNumber;
    std::string mPortNumber;
};

// PDU extension bar ports
class ExtensionBarPort : public DevicePort
{
public:
    ExtensionBarPort() = delete;
    ExtensionBarPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice);

    virtual void updateDescriptionAndLabel() override;

protected:
    virtual void _registerRequiredParameters() override;
    virtual size_t _getInputParametersCount() const override;
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const override;

private:
    std::string mDevicePlacementType; // rack placement (left/right)
    std::string mPortNumber;
};

// UPS ports
class UPSPort : public DevicePort
{
public:
    UPSPort() = delete;
    UPSPort(const std::string_view deviceUPosition, const size_t fileRowNumber, const bool isSourceDevice);

    virtual void updateDescriptionAndLabel() override;

protected:
    virtual void _registerRequiredParameters() override;
    virtual size_t _getInputParametersCount() const override;
    virtual std::pair<std::string, std::string> _getDeviceTypeDescriptionAndLabel() const override;

private:
    std::string mLoadSegmentNumber;
    std::string mPortNumber;
};

#endif // POWERDEVICEPORT_H
