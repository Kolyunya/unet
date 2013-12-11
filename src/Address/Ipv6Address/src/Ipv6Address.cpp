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

#include "Ipv6Address.hpp"

namespace Unet
{

                        Ipv6Address::Ipv6Address ( const Address& address )
                            :
                                Address(address)
    {

    }

                        Ipv6Address::Ipv6Address ( const sockaddr_in6& ipv6Address )
    {

        this->copyAddress(ipv6Address);

    }

                        Ipv6Address::Ipv6Address ( const std::string& ipv6 , const std::string& port )
    {

        this->copyAddress(Ipv6Address::stringToIpv6Address(ipv6,port));

    }

                        Ipv6Address::Ipv6Address ( const std::string& ipv6 , unsigned short int port )
    {

        this->copyAddress(Ipv6Address::stringToIpv6Address(ipv6,port));

    }

    Ipv6Address&        Ipv6Address::operator= ( const sockaddr_in6& ipv6Address )
    {

        this->copyAddress(ipv6Address);
        return *this;

    }

    AddressUniPtr       Ipv6Address::getCopyByUniPtr ( void ) const
    {

        return AddressUniPtr(new Ipv6Address(*this));

    }

    AddressShrPtr       Ipv6Address::getCopyByShrPtr ( void ) const
    {

        return AddressShrPtr(new Ipv6Address(*this));

    }

    std::string         Ipv6Address::toString ( void ) const
    {

        const sockaddr_in6* addressPtr = reinterpret_cast <const sockaddr_in6*> (this->getDataPtr());
        return Ipv6Address::ipv6AddressToString(*addressPtr);

    }

    in6_addr            Ipv6Address::stringToIpv6Address ( const std::string& ipv6 )
    {

        in6_addr ipv6Address;

        //  @review_required    Does "inet_pton" zero-initializes address? Remove next line if it does.
        memset(&ipv6Address,0,sizeof(ipv6Address));

        int ipv6AddressConversionResult = inet_pton(AF_INET6,ipv6.data(),&ipv6Address);

        if ( ipv6AddressConversionResult == -1 )
        {
            throw SYSTEM_EXCEPTION(InvalidNetworkAddressFamily);
        }

        else if ( ipv6AddressConversionResult == 0 )
        {
            throw EXCEPTION(StringDoesNotRepresentValidNetworkAddress);
        }

        return ipv6Address;

    }

    sockaddr_in6        Ipv6Address::stringToIpv6Address ( const std::string& ipv6 , const std::string& port )
    {

        return stringToIpv6Address(ipv6,std::string_to_int(port));

    }

    sockaddr_in6        Ipv6Address::stringToIpv6Address ( const std::string& ipv6 , unsigned short int port )
    {

        sockaddr_in6 ipv6Address;
        memset(&ipv6Address,0,sizeof(ipv6Address));

        //  @review_required    How do we fill in first two fields?
        ipv6Address.sin6_flowinfo = 0;
        ipv6Address.sin6_scope_id = 0;
        ipv6Address.sin6_family = AF_INET6;
        ipv6Address.sin6_port = htons(port);
        ipv6Address.sin6_addr = stringToIpv6Address(ipv6);

        return ipv6Address;

    }

    std::string         Ipv6Address::ipv6AddressToString ( const in6_addr& ipv6Address )
    {

        char ipv6AddressBuffer[INET6_ADDRSTRLEN];

        const char* ipv6AddressString = inet_ntop   (
                                                        AF_INET6,
                                                        &ipv6Address,
                                                        ipv6AddressBuffer,
                                                        INET6_ADDRSTRLEN
                                                    );

        if ( ipv6AddressString == nullptr )
        {
            throw SYSTEM_EXCEPTION(NetworkAddressCouldNotBeconvertedToString);
        }

        return ipv6AddressString;

    }

    std::string         Ipv6Address::ipv6AddressToString ( const sockaddr_in6& ipv6Address )
    {

        std::string ipv6AddressIp = Ipv6Address::ipv6AddressToString(ipv6Address.sin6_addr);
        std::string ipv6AddressPort = std::number_to_string(ntohs(ipv6Address.sin6_port));
        return    ipv6AddressIp + ":" + ipv6AddressPort;

    }

}