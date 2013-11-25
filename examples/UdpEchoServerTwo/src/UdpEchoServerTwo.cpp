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

#include <iostream>
#include <Unet/UdpServer.hpp>
#include <Unet/Ipv4Address.hpp>

namespace Unet
{
    class UdpEchoServer
    {
        public:
            explicit            UdpEchoServer ( AddressShrPtr addressShrPtr )
            {
                this->udpServer.addEventListener(SocketServerEvent::MESSAGE_RECIEVED,this,&UdpEchoServer::datagramRecievedHandler);
                this->udpServer.setAddress(addressShrPtr);
                this->udpServer.launch();
                    std::cout << "___4___\n";

            }
            void                datagramRecievedHandler ( Datagram* recievedDatagramPtr )
            {
                std::cout << recievedDatagramPtr->message << std::endl;
                this->udpServer.sendDatagram(*recievedDatagramPtr);
            }
        protected:
            explicit            UdpEchoServer ( const UdpEchoServer& udpServerUser );
            UdpEchoServer&      operator= ( const UdpEchoServer& udpServerUser );
            UdpServer           udpServer;
    };
}

int main ( int argc , char** argv )
{

    //  Check if program parameters are provided
    if ( argc != 3 )
    {
        std::cout << "Usage: UdpEchoServerTwo $(SERVER_IP) $(SERVER_PORT)" << std::endl;
        return -1;
    }

    try
    {
        //  Create an UDP echo server
        Unet::AddressShrPtr udpEchoServerAddressShrPtr(new Unet::Ipv4Address(argv[1],argv[2]));
        Unet::UdpEchoServer udpEchoServer(udpEchoServerAddressShrPtr);
            std::cout << "___5___\n";

    }
    catch ( Unet::Exception exception )
    {
        std::cout << "UNET EXCEPTION" << std::endl;
        std::cout << exception.what() << std::endl;
        std::cout << exception.getMessage() << std::endl;
    }
    catch ( std::exception )
    {
        std::cout << "STD EXCEPTION" << std::endl;
    }
    catch ( ... )
    {
        std::cout << "WTF EXCEPTION" << std::endl;
    }

    return -1;
    std::cout << "___6___\n";

    std::cout << "DONE" << std::endl;
    //  Run forever
    while ( false )
    {

    }

    return 0;

}