#ifndef DEVICEFACTORY_H
#define DEVICEFACTORY_H

#include <string>
#include <set>
#include <map>

#include "labelutils.h"
#include "device.h"

class DeviceFactory
{
public:
    DeviceFactory();

    /* This function is responsible for creating the actual device objects which are then used for generating the connection output data (descriptions and labels)
       It implements the factory design pattern and returns a null pointer if the device cannot be created (unknown device)
    */
    DevicePtr createDevice(DeviceTypeID deviceTypeID, bool isSourceDevice);

    int getCreatedDevicesCount() const;

private:
    int mCreatedDevicesCount;
};

#endif // DEVICEFACTORY_H
