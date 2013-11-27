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
#include <Unet/Ipv4Address.hpp>
#include <Unet/UdpEchoServer.hpp>

int main ( int argc , char** argv )
{

    //  Check if program parameters are provided
    if ( argc != 3 )
    {
        std::cout << "Usage: UdpEchoServerTwo $(SERVER_IP) $(SERVER_PORT)" << std::endl;
        return -1;
    }

    //  Create an UDP echo server
    Unet::AddressShrPtr udpEchoServerAddressShrPtr(new Unet::Ipv4Address(argv[1],argv[2]));
    Unet::UdpEchoServer udpEchoServer;
    udpEchoServer.setAddress(udpEchoServerAddressShrPtr);
    udpEchoServer.launch();

    while ( true )
    {
        // Run server forever
    }

    return 0;

}