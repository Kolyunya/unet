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

#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include <sys/ioctl.h>                  //  ioctl
                                        //  FIONREAD
#include <fcntl.h>                      //  O_NONBLOCK
#include <Unet/File.hpp>                //  Unet::File
#include <Unet/Address.hpp>             //  Unet::Address
#include <Unet/AddressFactory.hpp>      //  Unet::AddressFactory
#include <Unet/SocketExceptions.hpp>    //  Unet::Exception
                                        //  Unet::Address
#include <Unet/NullAddress.hpp>         //  Unet::NullAddress
#include <iostream>
namespace Unet
{
    class Socket
        :
            public File
    {
        public:
            explicit            Socket ( int domain , int type , int protocol = 0 , int descriptor = -1 );
            virtual             ~Socket ( void ) noexcept override = default;
            bool                operator== ( const Socket& socket ) const;
            void                bind ( const AddressUniPtr& addressUniPtr );
            void                connect ( const AddressUniPtr& addressUniPtr );
            void                disconnect ( void );
            void                shutdown ( int options );
            bool                isBlocking ( void ) const;
            bool                isNonBlocking ( void ) const;
            bool                hasUnreadData ( size_t unreadDataSizeRequired = 0 ) const;
            bool                hasNoUnreadData ( size_t unreadDataSizeRequired = 0 ) const;
            void                waitForUnreadData ( size_t unreadDataSizeRequired = 0 ) const;
            AddressUniPtr       getLocalAddress ( void ) const;
            AddressUniPtr       getPeerAddress ( void ) const;
            size_t              getUnreadDataSize ( void ) const;
                                template <typename OptionType>
            void                setOption ( int optionName , const OptionType& optionValue , int protocolLevel = SOL_SOCKET );
                                template <typename OptionType>
            OptionType          getOptionValue ( int optionName , int protocolLevel = SOL_SOCKET ) const;
            void                setBlocking ( void );
            void                setNonBlocking ( void );
            std::string         read ( int readOptions = 0 );
            std::string         peek ( int peekOptions = 0 );
            void                write ( const std::string& data , int writeOptions = 0 );
        protected:
            virtual int         allocate ( void ) override;
            int                 domain;
            int                 type;
            int                 protocol;
    };
}

#include "Socket.tpp"

#endif  //  _SOCKET_HPP_