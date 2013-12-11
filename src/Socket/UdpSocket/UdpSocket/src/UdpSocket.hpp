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

#ifndef _UENT_UDP_SOCKET_HPP_
#define _UENT_UDP_SOCKET_HPP_

#include <netinet/in.h>                     //  IPPROTO_UDP
#include <Unet/Socket.hpp>                  //  Unet::Socket
#include <Unet/Datagram.hpp>                //  Unet::Datagram
#include <Unet/AddressFactory.hpp>          //  Unet::AddressFactory
#include <Unet/UdpSocketExceptions.hpp>     //  Unet::Exception

namespace Unet
{
    class UdpSocket
        :
            public Socket
    {
        public:
            explicit        UdpSocket ( int domain = AF_INET , int protocol = IPPROTO_UDP );
            explicit        UdpSocket ( UdpSocket&& udpSocket );
            UdpSocket&      operator= ( UdpSocket&& udpSocket );
            virtual         ~UdpSocket ( void ) noexcept override = default;
            Datagram        recieveDatagram ( int options = 0 );
            void            sendDatagram ( const Datagram& datagram , int options = 0 );
    };
}

#endif  //  _UENT_UDP_SOCKET_HPP_