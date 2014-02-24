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

#ifndef _UNET_TCP_SOCKET_INCLUDED_
#define _UNET_TCP_SOCKET_INCLUDED_

#define TCP_USER_TIMEOUT 18

#include <utility>                          //  std::swap
#include <netinet/in.h>                     //  IPPROTO_TCP
#include <netinet/tcp.h>                    //  TCP_KEEPIDLE
                                            //  TCP_KEEPINTVL
                                            //  TCP_KEEPCNT
#include <Unet/Socket.hpp>                  //  Unet::Socket
#include <Unet/TcpSocketExceptions.hpp>

namespace Unet
{

    class TcpSocket;

    using TcpSocketUniPtr = std::unique_ptr<TcpSocket>;

    using TcpSocketShrPtr = std::shared_ptr<TcpSocket>;

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
            int                 getConnectionsLimit ( void ) const;
            void                setConnectionsLimit ( int connectionsLimit );
            size_t              getMessageSize ( void ) const;
            void                setMessageSize ( size_t messageSize );
            std::string         getMessageDelimiter ( void ) const;
            void                setMessageDelimiter ( const std::string& messageDelimiter );
            bool                getKeepAliveEnabled ( void ) const;
            void                setKeepAliveEnabled ( bool keepAliveEnabled );
            int                 getKeepAliveParameter ( int parameter );
            void                setKeepAliveParameters ( unsigned int time = 0 , unsigned int interval = 0 , unsigned int probes = 0 );
            int                 getUserTimeout ( void ) const;
            void                setUserTimeout ( int userTimeout );
            void                listen ( void );
            TcpSocket           accept ( void );
            TcpSocketShrPtr     _accept ( void );
            void                checkDisconnect ( void ) const;
            std::string         receiveDataBySize ( size_t dataSize = 0 , int receiveOptions = 0 );
            std::string         receiveMessageBySize ( int receiveOptions = 0 );
            std::string         receiveMessageByDelimiter ( int receiveOptions = 0 );
            std::string         peekDataBySize ( size_t dataSize = 0 , int peekOptions = 0 );
            std::string         peekMessageBySize ( int peekOptions = 0 );
            std::string         peekMessageByDelimiter ( int peekOptions = 0 );
            void                sendMessage ( const std::string& message , int sendOptions = 0 ) const;
            void                sendMessageWithDelimiter ( const std::string& message , int sendOptions = 0 ) const;
        protected:
            int                 connectionsLimit;
            size_t              messageSize;
            std::string         messageDelimiter;
    };
}

#endif  //  _UNET_TCP_SOCKET_INCLUDED_