//	Unix-network (unet) - Object-oriented high-level C++ Unix network framework.
//	Copyright (C) 2013 Oleynikov Nikolay
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//	Author email: OleynikovNY@mail.ru

#include <iostream>
#include <Unet/UdpSocket.hpp>
#include <Unet/Ipv4Address.hpp>

int main ( int argc , char** argv )
{

	//	Exactly four command line parameters are required
	if ( argc != 5 )
	{
		std::cout << "Usage: UdpEchoClient_000 $(SERVER_IP) $(SERVER_PORT) $(CLIENT_IP) $(CLIENT_PORT)" << std::endl;
		return -1;
	}

	//	Create and open UDP socket with Internet family address
	Unet::UdpSocket udpSocket;
	udpSocket.open();
	std::cout << "Client socket was opened successfully" << std::endl;

	//	Set socket to reuse address
	udpSocket.setOption(SO_REUSEADDR,1);
	std::cout << "Client socket option \"SO_REUSEADDR\" was set to \"1\"" << std::endl;

	//	Bind socket to the specified local address
	udpSocket.bind(Unet::Ipv4Address(argv[3],argv[4]));
	std::cout << "Client socket was bound successfully" << std::endl;

	//	Construct server address
	Unet::AddressShrPtr serverAddressShrPtr(new Unet::Ipv4Address(argv[1],argv[2]));

	//	Datagram which will be recieved and sent back
	Unet::Datagram outgoingDatagram("",serverAddressShrPtr);
	Unet::Datagram incommingDatagram;

	while ( true )
	{
		try
		{
			//	Request a message from user
			outgoingDatagram.message.clear();
			std::cout << "Enter message: ";
			std::cin >> outgoingDatagram.message;

			//	Send the message to the server
			udpSocket.sendDatagram(outgoingDatagram);
			std::cout << "[ --> ] [" << outgoingDatagram.addressShrPtr->toString() << "] - \"" << outgoingDatagram.message << "\"" << std::endl;

			//	Retrieve all pending datagrams
			while ( udpSocket.hasUnreadData() )
			{
				incommingDatagram = udpSocket.recieveDatagram();
				std::cout << "[ <-- ] ["  << incommingDatagram.addressShrPtr->toString() << "] - \"" << incommingDatagram.message << "\"" << std::endl;
			}
		}

		//	Catch and print all exceptions
		catch ( Unet::Exception exception )
		{
			std::cout << "Unet exception: " << exception.getMessage() << std::endl;
		}
		catch ( std::exception exception )
		{
			std::cout << "Standard exception: " << exception.what() << std::endl;
		}
		catch ( ... )
		{
			std::cout << "Unknown exception" << std::endl;
		}
	}
}