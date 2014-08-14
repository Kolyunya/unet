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

#ifndef _UNET_NULL_ADDRESS_HPP_
#define _UNET_NULL_ADDRESS_HPP_

#include <netinet/in.h>                 //  sockaddr_in
#include <arpa/inet.h>                  //  inet_pton
#include <Unet/Address.hpp>             //  Unet::Address

namespace Unet
{
    class NullAddress
        :
            public Address
    {
        public:
                                        NullAddress ( void );
            virtual                     ~NullAddress ( void ) noexcept override = default;
            virtual AddressUniPtr       getCopyByUniPtr ( void ) const override;
            virtual AddressShrPtr       getCopyByShrPtr ( void ) const override;
            virtual std::string         toString ( void ) const override;
    };
}

#endif  //  _UNET_NULL_ADDRESS_HPP_