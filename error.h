#ifndef ERROR_H
#define ERROR_H

#include "device.h"

// unknown device entered in connection definitions/input file
class UnknownDeviceError : public Device
{
public:
    UnknownDeviceError();

    void execute(std::ofstream& err);


    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const std::string&, int&, bool&, std::ofstream&);
};

// fewer input cells than required (some are empty) in connection input file
class FewerCellsError : public Device
{
public:
    FewerCellsError();

    void execute(std::ofstream& err);

    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const std::string&, int&, bool&, std::ofstream&);
};

// wrong connection format in connection definitions file
class WrongFormatError : public Device
{
public:
    WrongFormatError();

    void execute(std::ofstream &err);

    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const std::string&, int&, bool&, std::ofstream&);

};

// invalid device placement U number in connection definitions file
class WrongUNumberError : public Device
{
public:
    WrongUNumberError();

    void execute(std::ofstream &err);

    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const std::string&, int&, bool&, std::ofstream&);
};

// device mentioned in connection definitions file not contained in the mapping table (connection to nothing)
class NoDevicePresentError : public Device
{
public:
    NoDevicePresentError();

    void execute(std::ofstream &err);

    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const std::string&, int&, bool&, std::ofstream&);
};

// device mentioned in connection definitions file is connected to itself
class DeviceConnectedToItselfError : public Device
{
public:
    DeviceConnectedToItselfError();

    void execute(std::ofstream &err);

    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const std::string&, int&, bool&, std::ofstream&);

};

// device marked as connected, number of connections mentioned in connection definitions file is 0
class NoConnectionsError:public Device
{
public:
    NoConnectionsError();

    void execute(std::ofstream &err);

    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const std::string&, int&, bool&, std::ofstream&);
};

#endif // ERROR_H
