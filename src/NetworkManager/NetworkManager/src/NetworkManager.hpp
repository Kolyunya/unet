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

#ifndef _UNET_NETWORK_MANAGER_HPP_
#define _UNET_NETWORK_MANAGER_HPP_

namespace Unet
{

    template <typename AddressType>
    class NetworkManager
    {
        public:
                                    NetworkManager ( const std::string& ip , const std::string& port );
            virtual                 ~NetworkManager ( void ) noexcept = default;
        protected:
            void                    initialize ( const std::string& ip , const std::string& port );
            void                    initializeTcpServer ( const std::string& ip , const std::string& port );
            void                    initializeUdpServer ( const std::string& ip , const std::string& port );
            void                    clientConnectHandler ( Unet::TcpSocket& clientSocket );
            void                    clientDisconnectHandler ( Unet::TcpSocket& clientSocket );
            void                    clientTcpRequestHandler ( Unet::TcpSocket& clientSocket , const std::string& clientRequest );
            void                    clientUdpRequestHandler ( const Unet::Datagram& clientRequest );
            Unet::TcpServer         tcpServer;
            Unet::UdpServer         udpServer;
    };

}

#include "NetworkManager.tpp"

#endif  //  _UNET_NETWORK_MANAGER_HPP_