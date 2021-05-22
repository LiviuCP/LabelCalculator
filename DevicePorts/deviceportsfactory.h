#ifndef DEVICEPORTSFACTORY_H
#define DEVICEPORTSFACTORY_H

#include <string>
#include <set>
#include <map>

#include "labelutils.h"
#include "deviceport.h"

class DevicePortsFactory
{
public:
    DevicePortsFactory();

    /* This function is used for resetting the factory functionality (currently only the counter of created device ports)
    */
    void reset();

    /* This function is responsible for creating the actual device objects which are then used for generating the connection output data (descriptions and labels)
       It implements the factory design pattern and returns a null pointer if the device cannot be created (unknown device)
    */
    DevicePortPtr createDevicePort(const DeviceTypeID deviceTypeID, const std::string& deviceUPosition, const size_t fileRowNumber, const size_t fileColumnNumber, const bool isSourceDevice);

    size_t getCreatedDevicePortsCount() const;

private:
    size_t mCreatedDevicePortsCount;
};

#endif // DEVICEPORTSFACTORY_H
