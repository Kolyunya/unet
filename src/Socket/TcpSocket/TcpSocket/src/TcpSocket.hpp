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

#ifndef _UNET_TCP_SOCKET_HPP_
#define _UNET_TCP_SOCKET_HPP_

#include <utility>                          //  std::swap
#include <netinet/in.h>                     //  IPPROTO_TCP
#include <Unet/Socket.hpp>                  //  Unet::Socket
#include <Unet/TcpSocketExceptions.hpp>

namespace Unet
{
    class TcpSocket
        :
            public Socket
    {
        public:
            explicit            TcpSocket ( int domain = AF_INET , int protocol = IPPROTO_TCP , int descriptor = -1 );
                                TcpSocket ( TcpSocket&& tcpSocket );
            TcpSocket&          operator= ( TcpSocket&& tcpSocket );
            void                swap ( TcpSocket& tcpSocket );
            virtual             ~TcpSocket ( void ) noexcept override = default;
            unsigned char       getMessageDelimiter ( void ) const;
            void                setMessageDelimiter ( unsigned char messageDelimiter );
            void                unsetMessageDelimiter ( void );
            unsigned char       getConnectionsLimit ( void ) const;
            void                setConnectionsLimit ( unsigned char connectionsLimit );
            void                unsetConnectionsLimit ( void );
            void                listen ( void );
            TcpSocket           accept ( void );
            std::string         peekMessage ( size_t messageSize = 0 , int options = 0 );
            std::string         peekMessageByDelimiter ( char messageDelimiter , int options = 0 );
            std::string         recieveMessage ( size_t messageSize = 0 , int options = 0 );
            std::string         recieveMessageByDelimiter ( char messageDelimiter = 0 , int options = 0 );
            void                sendMessage ( const std::string& message , int options = 0 ) const;
        protected:
            char                messageDelimiter;
            char                connectionsLimit;
    };
}

#endif  //  _UNET_TCP_SOCKET_HPP_