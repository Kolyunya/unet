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

#ifndef _UNET_PROTOCOL_HPP_
#define _UNET_PROTOCOL_HPP_

#include <Unet/ProtocolType.hpp>
#include <Unet/ProtocolExceptions.hpp>

namespace Unet
{
    class Protocol
    {
        public:
            explicit            Protocol ( ProtocolType type );
            virtual             ~Protocol ( void ) = default;
            bool                operator== ( const Protocol& protocol ) const;
            bool                operator!= ( const Protocol& protocol ) const;
            ProtocolType        getType ( void ) const;
            void                setType ( ProtocolType type );
            static Protocol     tcp ( void );
            static Protocol     udp ( void );
        private:
            static bool         getTypeIsValid ( ProtocolType type );
            static bool         getTypeIsNotValid ( ProtocolType type );
            static void         validateType ( ProtocolType type );
            ProtocolType        type;
    };
}

#endif  //  _UNET_PROTOCOL_HPP_