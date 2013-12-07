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

#ifndef _UNET_ADDRESS_FACTORY_HPP_
#define _UNET_ADDRESS_FACTORY_HPP_

#include <Unet/Address.hpp>
#include <Unet/Ipv4Address.hpp>
#include <Unet/Ipv6Address.hpp>

namespace Unet
{
    class AddressFactory
    {
        public:
            explicit                    AddressFactory ( void );
            virtual                     ~AddressFactory ( void ) noexcept = default;
            sockaddr*                   getDataPtr ( void ) const;
            socklen_t*                  getSizePtr ( void ) const;
            bool                        hasProduct ( void ) const;
            void                        reset ( void );
            void                        makeProduct ( void );
            AddressUniPtr               getProductByUniPtr ( void );
            AddressShrPtr               getProductByShrPtr ( void );
        protected:
            void                        checkHasProduct ( void ) const;
            void                        resetAddressUniPtr ( void );
            void                        resetAddressData ( void );
            void                        resetAddressSize ( void );
            void                        copyAddress ( Address* addressPtr ) const;
            void                        copyAddressData ( Address* addressPtr ) const;
            void                        copyAddressSize ( Address* addressPtr ) const;
            AddressUniPtr               getAddressCopy ( void ) const;
            AddressUniPtr               addressUniPtr;
            mutable sockaddr_storage    addressData;
            mutable socklen_t           addressSize;
    };
}

#endif  //  _UNET_ADDRESS_FACTORY_HPP_