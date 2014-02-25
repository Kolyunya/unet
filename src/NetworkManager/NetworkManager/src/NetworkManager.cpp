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

namespace Unet
{

            template <typename AddressType>
            NetworkManager<AddressType>::NetworkManager ( const std::string& ip , const std::string& port )
    {
        this->initialize(ip,port);
    }

            template <typename AddressType>
    void    NetworkManager<AddressType>::initialize ( const std::string& ip , const std::string& port )
    {
        this->initializeTcpServer(ip,port);
        this->initializeUdpServer(ip,port);
    }

            template <typename AddressType>
    void    NetworkManager<AddressType>::initializeTcpServer ( const std::string& ip , const std::string& port )
    {
        this->tcpServer.setAddress(Unet::AddressUniPtr(new AddressType(ip,port)));
        this->tcpServer.setConnectionsLimit(128);
        this->tcpServer.setReceiveMode(Unet::TCP_RECEIVE_MODE_BY_DELIMITER);
        this->tcpServer.setKeepAliveTimeout(1);
        this->tcpServer.setDisconnectTimeout(1);
        this->tcpServer.setMessageDelimiter("@");
        this->tcpServer.clientConnectedEvent.add_listener(this,&NetworkManager::clientConnectHandler);
        this->tcpServer.clientDisconnectedEvent.add_listener(this,&NetworkManager::clientDisconnectHandler);
        this->tcpServer.messageReceivedEvent.add_listener(this,&NetworkManager::clientTcpRequestHandler);
        this->tcpServer.start();
    }

            template <typename AddressType>
    void    NetworkManager<AddressType>::initializeUdpServer ( const std::string& ip , const std::string& port )
    {
        this->udpServer.setAddress(Unet::AddressUniPtr(new AddressType(ip,port)));
        this->udpServer.datagramReceivedEvent.add_listener(this,&NetworkManager::clientUdpRequestHandler);
        this->udpServer.start();
    }

            template <typename AddressType>
    void    NetworkManager<AddressType>::clientConnectHandler ( Unet::TcpSocket& clientSocket )
    {

    }

            template <typename AddressType>
    void    NetworkManager<AddressType>::clientDisconnectHandler ( Unet::TcpSocket& clientSocket )
    {

    }

            template <typename AddressType>
    void    NetworkManager<AddressType>::clientTcpRequestHandler ( Unet::TcpSocket& clientSocket , const std::string& clientRequest )
    {

    }

            template <typename AddressType>
    void    NetworkManager<AddressType>::clientUdpRequestHandler ( const Unet::Datagram& clientRequest )
    {

    }

}