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

#ifndef _UNET_DATAGRAM_HPP_
#define _UNET_DATAGRAM_HPP_

#include <string>               //  std::string
#include <memory>               //  std::shared_ptr
#include <utility>              //  std::swap
#include <Unet/Address.hpp>     //  Unet::Address

namespace Unet
{
    class Datagram4
    {
        public:
                                Datagram ( void ) = default;
                                Datagram ( const Datagram& datagram );
                                Datagram ( Datagram&& datagram );
            explicit            Datagram ( const std::string& message , AddressUniPtr addressUniPtr );
            virtual             ~Datagram ( void ) noexcept = default;
            Datagram&           operator= ( Datagram datagram );
            void                swap ( Datagram& datagram );
            bool                operator== ( const Datagram& datagram ) const;
            bool                hasAddress ( void ) const;
            void                clear ( void );
            std::string         message;
            AddressUniPtr       addressUniPtr;
        protected:
            void                deepCopy ( const Datagram& datagram );
            void                deepCopyMessage ( const Datagram& datagram );
            void                deepCopyAddress ( const Datagram& datagram );
    };
}

#endif  //  _UNET_DATAGRAM_HPP_