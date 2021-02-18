#ifndef ERROR_H
#define ERROR_H

#include "device.h"

//*************************************************************************************************************************************************//

// error 3: unknown device
class UnknownDeviceError : public Device
{
public:
    UnknownDeviceError();

    void execute(ofstream& err);


    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const string&, int&, bool&, ofstream&);
};

//*************************************************************************************************************************************************//

// error 4: fewer input cells than required (some are empty)
class FewerCellsError : public Device
{
public:
    FewerCellsError();

    void execute(ofstream& err);

    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const string&, int&, bool&, ofstream&);
};

//*************************************************************************************************************************************************//

// error 5: wrong connection format
class WrongFormatError : public Device
{
public:
    WrongFormatError();

    void execute(ofstream &err);

    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const string&, int&, bool&, ofstream&);

};

//*************************************************************************************************************************************************//

// error 6: invalid device placement U number
class WrongUNumberError : public Device
{
public:
    WrongUNumberError();

    void execute(ofstream &err);

    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const string&, int&, bool&, ofstream&);
};

//*************************************************************************************************************************************************//

// error 7: device not contained in the mapping table
class NoDevicePresentError : public Device
{
public:
    NoDevicePresentError();

    void execute(ofstream &err);

    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const string&, int&, bool&, ofstream&);
};

//*************************************************************************************************************************************************//

// error 8: device connected to itself
class DeviceConnectedToItselfError : public Device
{
public:
    DeviceConnectedToItselfError();

    void execute(ofstream &err);

    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const string&, int&, bool&, ofstream&);

};

// empty methods, not used


//*************************************************************************************************************************************************//

// error 9: device marked as connected, number of connections mentioned is 0
class NoConnectionsError:public Device
{
public:
    NoConnectionsError();

    void execute(ofstream &err);

    void buildDescriptionText();
    void buildLabelText();
    void parseInputData(const string&, int&, bool&, ofstream&);
};

//*************************************************************************************************************************************************//



#endif // ERROR_H
