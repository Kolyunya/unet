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

#ifndef _ADDRESS_HPP_
#define _ADDRESS_HPP_

#include <sys/socket.h>             //  socklen_t
                                    //  sa_family_t
                                    //  sockaddr_storage
#include <cstring>                  //  memset
                                    //  memcpy
                                    //  memcmp
#include <string>                   //  std::string
#include <memory>                   //  std::shared_ptr
#include <Unet/Exception.hpp>       //  Unet::Exception

namespace Unet
{

    class Address;

    typedef std::shared_ptr<Address> AddressShrPtr;

    typedef std::unique_ptr<Address> AddressUniPtr;

    class Address
    {
        public:
                                        Address ( void );
                                        Address ( const Address& address );
            virtual                     ~Address ( void ) noexcept = default;
            Address&                    operator= ( const Address& address );
            bool                        operator== ( const Address& address ) const;
            sa_family_t                 getFamily ( void ) const;
            const sockaddr*             getDataPtr ( void ) const;
            sockaddr*                   getDataPtr ( void );
            const socklen_t&            getSizeRef ( void ) const;
            socklen_t&                  getSizeRef ( void );
            const socklen_t*            getSizePtr ( void ) const;
            socklen_t*                  getSizePtr ( void );
            static socklen_t            getSizeLimit ( void );
            bool                        isEmpty ( void ) const;
            bool                        isNotEmpty ( void ) const;
            void                        clear ( void );
            virtual AddressUniPtr       getCopyByUniPtr ( void ) const = 0;
            virtual AddressShrPtr       getCopyByShrPtr ( void ) const = 0;
            virtual std::string         toString  ( void ) const = 0;
        protected:
            void                        setFamily ( sa_family_t family );
                                        template <typename AddressType>
            void                        copyAddress ( const AddressType& address);
            void                        copyAddress ( const Address& address );
            sockaddr_storage            data;
            socklen_t                   size;
    };

}

#include "Address.tpp"

#endif  //  _ADDRESS_HPP_