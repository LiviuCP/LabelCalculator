#include <cassert>

#include "labelutils.h"

int readDataField(const std::string& src, std::string& dest, const int index)
{
    const int c_Length{static_cast<int>(src.size())}; // no issue in converting to int as the size is reasonable (same for other similar situations in this project)

    int nextIndex{-1};

    if(c_Length > 0)
    {
        assert(index < c_Length);

        dest.clear();

        int currentIndex{index};

        if (',' == src[currentIndex])
        {
            ++currentIndex;
        }

        while(currentIndex < c_Length)
        {
            if(src[currentIndex] != ',')
            {
                dest += src[currentIndex];
            }
            else
            {
                break;
            }

            ++currentIndex;
        }

        if(currentIndex != c_Length)
        {
            nextIndex = currentIndex;
        }
    }

    return nextIndex;
}

bool parseConnectionFormatting(const std::string& source, int& secondDevice, int& connectionsCount)
{
    bool isFormattingValid{true};

    const int sourceLength{static_cast<int>(source.size())};
    int slashIndex{-1}; // index of '/'

    if (sourceLength > 0)
    {
        for (int index{0}; index < sourceLength; ++index)
        {
            if (isdigit(source[index]))
            {
                continue;
            }
            else if ('/' == source[index])
            {
                if (-1 != slashIndex) // more than one slash
                {
                    isFormattingValid = false;
                    break;
                }
                else
                {
                    slashIndex = index;
                    continue;
                }
            }
            else
            {
                isFormattingValid = false;
                break;
            }
        }

        // there should be exactly one slash character and it should not be located in the first/last string character position
        isFormattingValid = isFormattingValid && (slashIndex > 0 && slashIndex < sourceLength-1);

        if (isFormattingValid)
        {
            secondDevice = stoi(source.substr(0, slashIndex));
            connectionsCount = stoi(source.substr(slashIndex+1));
        }
    }
    else
    {
        isFormattingValid = false;
    }

    return isFormattingValid;
}

void createPlaceholders(const std::string& deviceType, std::string& dest)
{

    if ("_pdu" == deviceType) //pdu
    {
        dest += "PLACEMENT,LOAD SEGMENT NUMBER,PORT NUMBER";
    }
    else if ("_ext" == deviceType) //extension bar
    {
        dest += "PLACEMENT,PORT NUMBER,-";
    }
    else if ("_ups" == deviceType) //ups
    {
        dest += "LOAD SEGMENT NUMBER,PORT NUMBER,-";
    }
    else if ("_ps" == deviceType) //power supply
    {
        dest += "POWER SUPPLY NUMBER,-,-";
    }
    else if ("svr" == deviceType) //server
    {
        dest += "PORT TYPE,PORT NUMBER,-";
    }
    else if ("bld" == deviceType) //blade system
    {
        dest += "MODULE TYPE,MODULE NUMBER,PORT NUMBER";
    }
    else if ("sto" == deviceType) //storage device
    {
        dest += "CONTROLLER NUMBER,PORT NUMBER,-";
    }
    else if ("san" == deviceType) //SAN (FC) switch
    {
        dest += "PORT NUMBER,-,-";
    }
    else if ("lan" == deviceType) //LAN (Ethernet) switch
    {
        dest += "PORT NUMBER,-,-";
    }
    else if ("ib" == deviceType) //Infiniband switch
    {
        dest += "PORT NUMBER,-,-";
    }
    else if ("kvm" == deviceType) //KVM switch
    {
        dest += "PORT NUMBER,-,-";
    }
}
