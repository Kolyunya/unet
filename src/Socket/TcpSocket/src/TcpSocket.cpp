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

#include "TcpSocket.hpp"

namespace Unet
{

                        TcpSocket::TcpSocket ( int domain , int protocol , int descriptor )
                            :
                                Socket(domain,SOCK_STREAM,protocol,descriptor)
    {

    }

                        TcpSocket::TcpSocket ( TcpSocket&& tcpSocket )
                            :
                                TcpSocket()
    {
        this->swap(tcpSocket);
    }

    TcpSocket&          TcpSocket::operator= ( TcpSocket&& tcpSocket )
    {
        this->swap(tcpSocket);
        return *this;
    }

    void                TcpSocket::swap ( TcpSocket& tcpSocket )
    {
        std::swap(this->descriptor,tcpSocket.descriptor);
        std::swap(this->messageDelimiter,tcpSocket.messageDelimiter);
        std::swap(this->connectionsLimit,tcpSocket.connectionsLimit);
    }

    unsigned char       TcpSocket::getMessageDelimiter ( void ) const
    {
        if ( this->messageDelimiter < 0 )
        {
            throw Exception(ExcMessageDelimiterIsNotDefined);
        }
        return static_cast<unsigned int>(this->messageDelimiter);
    }

    void                TcpSocket::setMessageDelimiter ( unsigned char messageDelimiter )
    {
        this->messageDelimiter = static_cast<int>(messageDelimiter);
    }

    void                TcpSocket::unsetMessageDelimiter ( void )
    {
        this->messageDelimiter = -1;
    }

    unsigned char       TcpSocket::getConnectionsLimit ( void ) const
    {
        if ( this->connectionsLimit < 0 )
        {
            throw Exception(ExcConnectionsLimitIsNotDefined);
        }
        return static_cast<unsigned int>(this->connectionsLimit);
    }

    void                TcpSocket::setConnectionsLimit ( unsigned char connectionsLimit )
    {
        this->connectionsLimit = static_cast<int>(connectionsLimit);
    }

    void                TcpSocket::unsetConnectionsLimit ( void )
    {
        this->connectionsLimit = -1;
    }

    void                TcpSocket::listen ( void )
    {
        //  @description    Marks  the  socket as a passive socket, that is,
        //                  as a socket that will be used to  accept  incoming  connection requests.
        //  @throws         ExcSocketCouldNotBeListened

        int socketDescriptor = this->getDescriptor();
        int connectionsLimit = this->getConnectionsLimit();
        int socketListenResult = ::listen(socketDescriptor,connectionsLimit);
        if ( socketListenResult != 0 )
        {
            throw Exception(ExcSocketCouldNotBeListened,true);
        }
    }

    TcpSocket           TcpSocket::accept ( void )
    {
        //  @description    Extracts the first connection request on the queue of pending connections for the listening socket.
        //                  Creates a new connected socket, and returns a new file descriptor  referring  to that socket.
        //                  The newly created socket is not in the listening state.
        //                  The original socket  sockfd  is  unaffected  by  this call.

        //  Try to accept client socket
        int acceptedSocketDescriptor = ::accept (
                                                    this->getDescriptor(),
                                                    nullptr,
                                                    nullptr
                                                );

        //  Check if the socket was accepted successfully
        if ( acceptedSocketDescriptor < 0 )
        {
            throw Exception(ExcSocketCouldNotAcceptConnection,true);
        }

        //  Construct the socket which will manage accepted descriptor
        return TcpSocket    (
                                this->domain,
                                this->protocol,
                                acceptedSocketDescriptor
                            );
    }

    std::string         TcpSocket::peekMessage ( size_t messageSize , int options )
    {

        //  Add "MSG_PEEK" to "recieve_flags". Otherwise data will be removed from socket input buffer
        options |= MSG_PEEK;

        return this->recieveMessage(messageSize,options);

    }

    std::string         TcpSocket::peekMessage ( char messageTerminator , int options )
    {

        //  Add "MSG_PEEK" to "recieve_flags". Otherwise data will be removed from socket input buffer
        options |= MSG_PEEK;

        return this->recieveMessage(messageTerminator,options);

    }

    std::string         TcpSocket::recieveMessage ( size_t messageSize , int options )
    {

        this->waitForUnreadData(messageSize);

        std::string messageData;

        if ( messageSize == 0 )
        {

            messageData.resize(this->getUnreadDataSize(),'\0');

        }

        else
        {

            messageData.resize(messageSize,'\0');

        }

        ssize_t messageBytesRead = recv (
                                            this->getDescriptor(),
                                            &messageData[0],
                                            messageData.size(),
                                            options
                                        );

        if ( messageBytesRead < 0 )
        {

            throw Exception(ExcIncommingDataCouldNotBeRetrieved,true);

        }

        return messageData;

    }

    std::string         TcpSocket::recieveMessage ( char messageTerminator , int options )
    {

        std::string messageAvailable;
        size_t messageTerminatorPosition;

        while ( true )
        {

            messageAvailable = this->peekMessage();
            messageTerminatorPosition = messageAvailable.find_first_of(messageTerminator);

            if ( messageTerminatorPosition != std::string::npos )
            {

                //  "messageTerminatorPosition" found in the "messageAvailable"
                return this->recieveMessage(++messageTerminatorPosition,options);

            }

            if ( this->isNonBlocking() )
            {

                throw Exception(ExcMessageHasNotBeenDelieveredYet);

            }

        }

        return messageAvailable;

    }

    void                TcpSocket::sendMessage ( const std::string& message , int options )
    {

        //  @no_throw_guarantee        Strong no-throw guarantee



        size_t messageSize = message.size();

        ssize_t messageBytesSent = ::send   (
                                                this->getDescriptor(),
                                                message.data(),
                                                messageSize,
                                                options
                                            );

        if ( messageBytesSent < 0 )
        {

            throw Exception(ExcOutgoingDataCouldNotBeSent,true);

        }

        //  "sendMessage" may actually sendMessage less bytes than "data" contains. It's also an exceptional situation, thus must be checked for.
        //  "messageBytesSent" and "messageSize" are signed and unsigned types respectively. Explicit conversion is required to compare them.
        //  "messageBytesSent" is guaranteed to be non-negative at this point, thus it can be correctly cast to "size_t".
        else if ( static_cast<size_t>(messageBytesSent) < messageSize )
        {

            throw Exception(ExcOutgoingDataCouldNotBeSentCompletely);

        }

    }

}