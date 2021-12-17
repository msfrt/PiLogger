/**
 * \file interfaces.cpp
 *
 * \author Dave Yonkers
 * 
 */


#include <string>
#include <exception>

#include "interfaces.hpp"


/**
 * This function converts a string interface name to an enumerated name.
 * If there is no enumerated name available, the program will throw an
 * error, since that interface name is not supported.
 * \param str The name of the interface (ex. vcan0, vcan1, can0, can1)
 * \returns The enumerated interface name
 */ 
Interface string_to_iface(const std::string &str){

    if (str == std::string("vcan0"))
        return vcan0;
    else if (str == std::string("vcan1"))
        return vcan1;
    else if (str == std::string("vcan2"))
        return vcan2;
    else if (str == std::string("vcan3"))
        return vcan3;
    else if (str == std::string("vcan4"))
        return vcan4;
    else if (str == std::string("vcan5"))
        return vcan5;
    else if (str == std::string("vcan6"))
        return vcan6;
    else if (str == std::string("vcan7"))
        return vcan7;
    else if (str == std::string("can0"))
        return can0;
    else if (str == std::string("can1"))
        return can1;
    else if (str == std::string("can2"))
        return can2;
    else if (str == std::string("can3"))
        return can3;
    else if (str == std::string("can4"))
        return can4;
    else if (str == std::string("can5"))
        return can5;
    else if (str == std::string("can6"))
        return can6;
    else if (str == std::string("can7"))
        return can7;

    throw InterfaceNameException();

}


/**
 * This function gets the string representation of an iface enum
 * \param iface The enum of the interface (ex. vcan0, vcan1, can0, can1)
 * \returns The string interface name
 */ 
std::string iface_to_string(Interface iface){

    switch (iface)
    {
    case vcan0:
        return std::string("vcan0");
        break;
    case vcan1:
        return std::string("vcan1");
        break;
    case vcan2:
        return std::string("vcan2");
        break;
    case vcan3:
        return std::string("vcan3");
        break;
    case vcan4:
        return std::string("vcan4");
        break;
    case vcan5:
        return std::string("vcan5");
        break;
    case vcan6:
        return std::string("vcan6");
        break;
    case vcan7:
        return std::string("vcan7");
        break;
    case can0:
        return std::string("can0");
        break;
    case can1:
        return std::string("can1");
        break;
    case can2:
        return std::string("can2");
        break;
    case can3:
        return std::string("can3");
        break;
    case can4:
        return std::string("can4");
        break;
    case can5:
        return std::string("can5");
        break;
    case can6:
        return std::string("can6");
        break;
    case can7:
        return std::string("can7");
        break;
    
    }

    throw InterfaceNameException();

}