/**
 * \file interfaces.hpp
 *
 * \author Dave Yonkers
 * 
 * This file defines all possible CAN interfaces and provides
 * a function to convert strings to their interface enumerations.
 */

#pragma once

#include <string>
#include <exception>


/// An enumeration for the interface name, since we won't 
/// want to compare strings all the time
enum Interface { vcan0, vcan1, vcan2, vcan3, vcan4, vcan5, vcan6, vcan7, 
                  can0,  can1,  can2,  can3,  can4,  can5,  can6,  can7};


struct InterfaceNameException : public std::exception
{
    const char * what () const throw ()
    {
        return "Error turning a string into an inface enumeration or vice versa.";
    }
};


Interface string_to_iface(const std::string &str);
std::string iface_to_string(Interface iface);