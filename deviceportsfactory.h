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

    /* This function is responsible for creating the actual device objects which are then used for generating the connection output data (descriptions and labels)
       It implements the factory design pattern and returns a null pointer if the device cannot be created (unknown device)
    */
    DevicePortPtr createDevicePort(DeviceTypeID deviceTypeID, bool isSourceDevice);

    int getCreatedDevicePortsCount() const;

private:
    int mCreatedDevicePortsCount;
};

#endif // DEVICEPORTSFACTORY_H