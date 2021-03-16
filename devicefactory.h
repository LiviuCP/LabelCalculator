#ifndef DEVICEFACTORY_H
#define DEVICEFACTORY_H

#include <string>
#include <set>

class Device;

class DeviceFactory
{
public:
    static bool isDeviceTypeValid(const std::string& deviceType);

    DeviceFactory();

    /* This function is responsible for creating the actual device objects which are then used for generating the connection output data (descriptions and labels)
       It implements the factory design pattern and returns a null pointer if the device cannot be created (unknown device)
    */
    Device* createDevice(const std::string& deviceType, bool isSourceDevice);

    int getCreatedDevicesCount() const;

private:
    static const std::set<std::string> scDeviceTypes;

    int mCreatedDevicesCount;
};

#endif // DEVICEFACTORY_H
