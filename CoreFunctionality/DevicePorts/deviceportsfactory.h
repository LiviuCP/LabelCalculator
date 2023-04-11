#ifndef DEVICEPORTSFACTORY_H
#define DEVICEPORTSFACTORY_H

#include "applicationdata.h"

class DevicePort;

class DevicePortsFactory
{
public:
    /* This function is responsible for creating the actual device objects which are then used for generating the connection output data (descriptions and labels)
       It implements the factory design pattern and returns a null pointer if the device cannot be created (unknown device)
    */
    static DevicePort* createDevicePort(const Data::DeviceTypeID deviceTypeID,
                                        const std::string_view deviceUPosition,
                                        const size_t fileRowNumber,
                                        const bool isSourceDevice);
private:
    DevicePortsFactory() = default;
};

#endif // DEVICEPORTSFACTORY_H
