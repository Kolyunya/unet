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

#ifndef _IPV4_ADDRESS_HPP_
#define _IPV4_ADDRESS_HPP_

#include <netinet/in.h>                 //  sockaddr_in
#include <arpa/inet.h>                  //  inet_pton
#include <string.hpp>                   //  std::string_to_int
                                        //  std::number_to_string
#include <Unet/Address.hpp>             //  Unet::Address
#include <Unet/SocketException.hpp>     //  Unet::Exception

namespace Unet
{
    class Ipv4Address
        :
            public Address
    {
        public:
                                        Ipv4Address ( void ) = default;
                                        Ipv4Address ( const Address& address );
                                        Ipv4Address ( const sockaddr_in& ipv4Address );
                                        Ipv4Address ( const std::string& ipv4 , const std::string& port );
                                        Ipv4Address ( const std::string& ipv4 , unsigned short int port );
            virtual                     ~Ipv4Address ( void ) noexcept override = default;
            Ipv4Address&                operator= ( const sockaddr_in& ipv4Address );
            virtual AddressUniPtr       getCopyByUniPtr ( void ) const override;
            virtual AddressShrPtr       getCopyByShrPtr ( void ) const override;
            virtual std::string         toString ( void ) const override;
        protected:
            static in_addr              stringToIpv4Address ( const std::string& ipv4 );
            static sockaddr_in          stringToIpv4Address ( const std::string& ipv4 , const std::string& port );
            static sockaddr_in          stringToIpv4Address ( const std::string& ipv4 , unsigned short int port );
            static std::string          ipv4AddressToString ( const in_addr& ipv4Address );
            static std::string          ipv4AddressToString ( const sockaddr_in& ipv4Address );
    };
}

#endif  //  _IPV4_ADDRESS_HPP_