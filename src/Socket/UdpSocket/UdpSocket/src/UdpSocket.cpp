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

#include "UdpSocket.hpp"

namespace Unet
{

                    UdpSocket::UdpSocket ( int domain , int protocol )
                        :
                            Socket(domain,SOCK_DGRAM,protocol)
    {

    }

                    UdpSocket::UdpSocket ( UdpSocket&& udpSocket )
                        :
                            Socket(udpSocket.domain,SOCK_DGRAM,udpSocket.protocol)
    {
        this->swap(udpSocket);
    }

    UdpSocket&      UdpSocket::operator= ( UdpSocket&& udpSocket )
    {
        this->File::swap(udpSocket);
        return *this;
    }

    Datagram        UdpSocket::recieveDatagram ( int options )
    {

        //  If socket is in blocking mode it will wait for datagram to arrive, otherwise an exception will be thrown.
        this->waitForUnreadData();

        //  Get pending datagram size
        size_t pendingDatagramMessageSize = this->getUnreadDataSize();

        //  Construct a string which will hold the datagram message
        std::string pendingDatagramMessage(pendingDatagramMessageSize,0);

        // Get a pointer to datagram message
        char* pendingDatagramMessagePtr = &(pendingDatagramMessage[0]);

        // Prepare address factory to make an address
        AddressFactory addressFactory;
        sockaddr* pendingDatagramSenderAddressDataPtr = addressFactory.getDataPtr();
        socklen_t* pendingDatagramSenderAddressSizePtr = addressFactory.getSizePtr();

        // Receiving datagram from socket
        // Datagram message will be written directly in the "Datagram.message"
        // Datagram address will be written into the address factory which will make an address object a bit later
        ssize_t pendingDatagramBytesRecieved = recvfrom (
                                                            this->getDescriptor(),
                                                            pendingDatagramMessagePtr,
                                                            pendingDatagramMessageSize,
                                                            options,
                                                            pendingDatagramSenderAddressDataPtr,
                                                            pendingDatagramSenderAddressSizePtr
                                                        );

        // Checking for receive error
        if ( pendingDatagramBytesRecieved < 0 )
        {
            throw SYSTEM_EXCEPTION(IncommingDataCouldNotBeRetrieved);
        }

        //  Construct and return received datagram object
        return Datagram(pendingDatagramMessage,addressFactory.getProduct());

    }

    void            UdpSocket::sendDatagram ( const Datagram& datagram , int options )
    {

        // Define receiver address
        const sockaddr* recieverAddressPtr = datagram.addressUniPtr->isNotEmpty() ? datagram.addressUniPtr->getDataPtr() : nullptr;
        socklen_t recieverAddressSize = datagram.addressUniPtr->isNotEmpty() ? datagram.addressUniPtr->getSizeRef() : 0;

        // Send datagram
        ssize_t datagramMessageBytesSent = sendto   (
                                                        this->getDescriptor(),
                                                        datagram.message.data(),
                                                        datagram.message.size(),
                                                        options,
                                                        recieverAddressPtr,
                                                        recieverAddressSize
                                                    );

        // Checking for errors
        if ( datagramMessageBytesSent < 0 )
        {

            throw SYSTEM_EXCEPTION(OutgoingDataCouldNotBeSent);

        }

    }

}