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

#ifndef _IPV6_ADDRESS_HPP_
#define _IPV6_ADDRESS_HPP_

#include <netinet/in.h>                 //  sockaddr_in6
#include <arpa/inet.h>                  //  inet_pton
#include <string.hpp>                   //  std::string_to_int
                                        //  std::number_to_string
#include <Unet/Address.hpp>             //  Unet::PosixAddress

namespace Unet
{
    class Ipv6Address
        :
            public Address
    {
        public:
                                        Ipv6Address ( void ) = default;
                                        Ipv6Address ( const Address& address );
                                        Ipv6Address ( const sockaddr_in6& ipv6Address );
                                        Ipv6Address ( const std::string& ipv6 , const std::string& port );
                                        Ipv6Address ( const std::string& ipv6 , unsigned short int port );
            virtual                     ~Ipv6Address ( void ) noexcept override = default;
            Ipv6Address&                operator= ( const sockaddr_in6& ipv6Address );
            virtual AddressUniPtr       getCopyByUniPtr ( void ) const override;
            virtual AddressShrPtr       getCopyByShrPtr ( void ) const override;
            std::string                 toString ( void ) const override;
        protected:
            static in6_addr             stringToIpv6Address ( const std::string& ipv6 );
            static sockaddr_in6         stringToIpv6Address ( const std::string& ipv6 , const std::string& port );
            static sockaddr_in6         stringToIpv6Address ( const std::string& ipv6 , unsigned short int port );
            static std::string          ipv6AddressToString ( const in6_addr& ipv6Address );
            static std::string          ipv6AddressToString ( const sockaddr_in6& ipv6Address );

    };
}

#endif  //  _IPV6_ADDRESS_HPP_