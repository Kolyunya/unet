//  Unix-network (unet) - Object-oriented high-level C++ Unix network framework.
//  Copyright (C) 2013 Oleynikov Nikolay
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  Author email: OleynikovNY@mail.ru

#include "Ipv4Address.hpp"

namespace Unet
{

                        Ipv4Address::Ipv4Address ( const Address& address )
                            :
                                Address(address)
    {

    }

                        Ipv4Address::Ipv4Address ( const sockaddr_in& ipv4Address )
    {

        this->copyAddress(ipv4Address);

    }

                        Ipv4Address::Ipv4Address ( const std::string& ipv4 , const std::string& port )
    {

        this->copyAddress(Ipv4Address::stringToIpv4Address(ipv4,port));

    }

                        Ipv4Address::Ipv4Address ( const std::string& ipv4 , unsigned short int port )
    {

        this->copyAddress(Ipv4Address::stringToIpv4Address(ipv4,port));

    }

    Ipv4Address&        Ipv4Address::operator= ( const sockaddr_in& ipv4Address )
    {

        this->copyAddress(ipv4Address);
        return *this;

    }

    AddressUniPtr       Ipv4Address::getCopyByUniPtr ( void ) const
    {

        return AddressUniPtr(new Ipv4Address(*this));

    }

    AddressShrPtr       Ipv4Address::getCopyByShrPtr ( void ) const
    {

        return AddressShrPtr(new Ipv4Address(*this));

    }

    std::string         Ipv4Address::toString ( void ) const
    {

        const sockaddr_in* addressPtr = reinterpret_cast <const sockaddr_in*> (this->getDataPtr());
        return Ipv4Address::ipv4AddressToString(*addressPtr);

    }

    in_addr             Ipv4Address::stringToIpv4Address ( const std::string& ipv4 )
    {

        in_addr ipv4Address;
        memset(&ipv4Address,0,sizeof(ipv4Address));

        int ipv4AddressConversionResult = inet_pton(AF_INET,ipv4.data(),&ipv4Address);

        if ( ipv4AddressConversionResult == -1 )
        {
            throw SYSTEM_EXCEPTION(InvalidNetworkAddressFamily);
        }

        if ( ipv4AddressConversionResult == 0 )
        {
            throw EXCEPTION(StringDoesNotRepresentValidNetworkAddress);
        }

        return ipv4Address;

    }

    sockaddr_in         Ipv4Address::stringToIpv4Address ( const std::string& ipv4 , const std::string& port )
    {

        return stringToIpv4Address(ipv4,std::string_to_int(port));

    }

    sockaddr_in         Ipv4Address::stringToIpv4Address ( const std::string& ipv4 , unsigned short int port )
    {

        sockaddr_in ipv4Address;
        memset(&ipv4Address,0,sizeof(ipv4Address));

        ipv4Address.sin_family = AF_INET;
        ipv4Address.sin_port = htons(port);
        ipv4Address.sin_addr = stringToIpv4Address(ipv4);

        return ipv4Address;

    }

    std::string         Ipv4Address::ipv4AddressToString ( const in_addr& ipv4Address )
    {

        char ipv4AddressBuffer[INET_ADDRSTRLEN];

        const char* ipv4AddressString = inet_ntop   (
                                                        AF_INET,
                                                        &ipv4Address,
                                                        ipv4AddressBuffer,
                                                        INET_ADDRSTRLEN
                                                    );

        if ( ipv4AddressString == nullptr )
        {
            throw SYSTEM_EXCEPTION(NetworkAddressCouldNotBeconvertedToString);
        }

        return ipv4AddressString;

    }

    std::string         Ipv4Address::ipv4AddressToString ( const sockaddr_in& ipv4Address )
    {

        std::string ipv4AddressIp = Ipv4Address::ipv4AddressToString(ipv4Address.sin_addr);
        std::string ipv4AddressPort = std::number_to_string(ntohs(ipv4Address.sin_port));

        return ipv4AddressIp + ":" + ipv4AddressPort;

    }

}