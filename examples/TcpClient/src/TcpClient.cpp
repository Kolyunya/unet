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

#include <thread.hpp>
#include <iostream.hpp>
#include <Unet/TcpSocket.hpp>
#include <Unet/Ipv4Address.hpp>

void recieveDatagrams ( Unet::UdpSocket* udpSocketPtr )
{
    Unet::Datagram incommingDatagram;
    //	Retrieve all pending datagrams
    while ( udpSocketPtr->hasUnreadData() )
    {
        incommingDatagram = udpSocketPtr->recieveDatagram();
        std::coutmt << "[ <-- ] ["  << incommingDatagram.addressShrPtr->toString() << "] - \"" << incommingDatagram.message << "\"" << std::endl;
    }
}

int main ( int argc , char** argv )
{

    //  Check if server IP address and port command line parameters were provided
	if ( argc != 5 )
	{
		//	Show how the program is supposed to be launched
		std::cout << "Usage: TcpClient $(SERVER_IP) $(SERVER_PORT) $(CLIENT_IP) $(CLIENT_PORT)" << std::endl;
		return -1;
	}

	//	Create and open UDP socket with Internet family address
	Unet::TcpSocket tcpSocket;
	tcpSocket.open();
	std::cout << "Client socket was opened successfully" << std::endl;

	//	Set socket to reuse address
	tcpSocket.setOption(SO_REUSEADDR,1);
	std::cout << "Client socket option \"SO_REUSEADDR\" was set to \"1\"" << std::endl;

	//	Bind socket to the specified local address
	tcpSocket.bind(Unet::Ipv4Address(argv[3],argv[4]));
	std::cout << "Client socket was bound successfully" << std::endl;

	//	Construct server address
	Unet::AddressShrPtr serverAddressShrPtr(new Unet::Ipv4Address(argv[1],argv[2]));

    //  Connect to TCP server
    tcpSocket.connect(serverAddressShrPtr);

	//	Datagram which will be received and sent back
	Unet::Datagram outgoingDatagram("",serverAddressShrPtr);
	Unet::Datagram incommingDatagram;

    //  Launch thread receiving datagrams
    std::raii_thread recievingThread(std::bind(recieveDatagrams,&udpSocket));

	while ( true )
	{
		try
		{
			//	Request a message from user
			outgoingDatagram.message.clear();
			std::cin >> outgoingDatagram.message;

			//	Send the message to the server
			udpSocket.sendDatagram(outgoingDatagram);
			std::coutmt << "[ --> ] [" << outgoingDatagram.addressShrPtr->toString() << "] - \"" << outgoingDatagram.message << "\"" << std::endl;
		}
		catch ( ... )
		{
			//	Ignore all errors
		}
	}
}