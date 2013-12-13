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
                                Socket(domain,SOCK_STREAM,protocol,descriptor),
                                connectionsLimit(128),
                                messageSize(8),
                                messageDelimiter("\0")
    {

    }

                        TcpSocket::TcpSocket ( TcpSocket&& tcpSocket )
                            :
                                TcpSocket(AF_INET)
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
        std::swap(this->messageSize,tcpSocket.messageSize);
        std::swap(this->messageDelimiter,tcpSocket.messageDelimiter);
    }

    int                 TcpSocket::getConnectionsLimit ( void ) const
    {
        return this->connectionsLimit;
    }

    void                TcpSocket::setConnectionsLimit ( int connectionsLimit )
    {
        if ( connectionsLimit < 0 )
        {
            throw EXCEPTION(InvalidConnectionsLimitNumber);
        }
        this->connectionsLimit = connectionsLimit;
    }

    size_t              TcpSocket::getMessageSize ( void ) const
    {
        return this->messageSize;
    }

    void                TcpSocket::setMessageSize ( size_t messageSize )
    {
        if ( messageSize == 0 )
        {
            throw EXCEPTION(InvalidMessageSize);
        }
        this->messageSize = messageSize;
    }

    std::string         TcpSocket::getMessageDelimiter ( void ) const
    {
        return this->messageDelimiter;
    }

    void                TcpSocket::setMessageDelimiter ( const std::string& messageDelimiter )
    {

        this->messageDelimiter = messageDelimiter;

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
            throw SYSTEM_EXCEPTION(SocketCouldNotBeListened);
        }
    }

    bool                TcpSocket::getKeepAliveEnabled ( void ) const
    {
        return this->getOptionValue<int>(SO_KEEPALIVE);
    }

    void                TcpSocket::setKeepAliveEnabled ( bool keepAliveEnabled )
    {

        //  "setsockopt" expects an instance of "int"
        int keepAliveEnabledInt = static_cast<int>(keepAliveEnabled);

        this->setOption(SO_KEEPALIVE,keepAliveEnabledInt);

    }

    int                 TcpSocket::getKeepAliveParameter ( int parameter )
    {
        return this->getOptionValue<int>(parameter,SOL_TCP);
    }

    void                TcpSocket::setKeepAliveParameters ( unsigned int time , unsigned int interval , unsigned int probes )
    {

        if ( time > 0 )
        {
            this->setOption(TCP_KEEPIDLE,time,SOL_TCP);
        }

        if ( interval > 0 )
        {
            this->setOption(TCP_KEEPINTVL,interval,SOL_TCP);
        }

        if ( probes > 0 )
        {
            this->setOption(TCP_KEEPCNT,probes,SOL_TCP);
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
            throw SYSTEM_EXCEPTION(SocketCouldNotAcceptConnection);
        }

        //  Construct the socket which will manage accepted descriptor
        return TcpSocket    (
                                this->domain,
                                this->protocol,
                                acceptedSocketDescriptor
                            );
    }

    void                TcpSocket::checkDisconnect ( void ) const
    {
        if
        (
            this->isReadyForRead() == true
                &&
            this->getUnreadDataSize() == 0
        )
        {
            throw EXCEPTION(PeerDisconnected);
        }
    }

    std::string         TcpSocket::receiveDataBySize ( size_t dataSize , int receiveOptions )
    {

        std::string receivedData;
        size_t unreadDataSize = this->getUnreadDataSize();

        //  If requested to read all unread data
        if ( dataSize == 0 )
        {

            //  And there is no unread data
            if ( unreadDataSize == 0 )
            {

                //  return empty string
                throw EXCEPTION(IncommingDataIsNotAvailableNow);

            }

            //  Resize the buffer to hold all unread data
            receivedData.resize(unreadDataSize);

        }

        //  If requested to read a specific amount of data
        //  And that amount didn't arrive yet
        else if ( dataSize > unreadDataSize )
        {

            receivedData.resize(dataSize);
            this->waitForUnreadData(dataSize);

        }

        else
        {

            receivedData.resize(dataSize);

        }

        ssize_t receivedDataBytesRead = recv    (
                                                    this->getDescriptor(),
                                                    &receivedData[0],
                                                    receivedData.size(),
                                                    receiveOptions
                                                );

        if ( receivedDataBytesRead < 0 )
        {
            throw SYSTEM_EXCEPTION(IncommingDataCouldNotBeRetrieved);
        }

        return receivedData;

    }

    std::string         TcpSocket::receiveMessageBySize ( int receiveOptions )
    {
        return this->receiveDataBySize(this->messageSize,receiveOptions);
    }

    std::string         TcpSocket::receiveMessageByDelimiter ( int receiveOptions )
    {

        std::string unreadData;
        size_t messageTerminatorPosition=0;

        while ( true )
        {

            //  Peek all unread data
            unreadData = this->peekDataBySize();

            messageTerminatorPosition = unreadData.find_first_of(this->getMessageDelimiter());

            if ( messageTerminatorPosition != std::string::npos )
            {
                //  "messageDelimiter" found in the "unreadData"
                std::string messageReceived = this->receiveDataBySize(messageTerminatorPosition,receiveOptions);
                this->receiveDataBySize(this->getMessageDelimiter().size(),receiveOptions);
                return messageReceived;
            }

            if ( this->isNonBlocking() )
            {
                throw EXCEPTION(MessageHasNotBeenDelieveredYet);
            }

        }
    }

    std::string         TcpSocket::peekDataBySize ( size_t dataSize , int peekOptions )
    {
        //  Add "MSG_PEEK" to "peekOptions". Otherwise data will be removed from socket input buffer
        peekOptions |= MSG_PEEK;
        return this->receiveDataBySize(dataSize,peekOptions);
    }

    std::string         TcpSocket::peekMessageBySize ( int peekOptions )
    {
        //  Add "MSG_PEEK" to "peekOptions". Otherwise data will be removed from socket input buffer
        peekOptions |= MSG_PEEK;
        return this->receiveMessageBySize(peekOptions);
    }

    std::string         TcpSocket::peekMessageByDelimiter ( int peekOptions )
    {
        //  Add "MSG_PEEK" to "peekOptions". Otherwise data will be removed from socket input buffer
        peekOptions |= MSG_PEEK;
        return this->receiveMessageByDelimiter(peekOptions);
    }

    void                TcpSocket::sendMessage ( const std::string& message , int sendOptions ) const
    {

        //  @no_throw_guarantee        Strong no-throw guarantee

        size_t messageSize = message.size();

        ssize_t messageBytesSent = ::send   (
                                                this->getDescriptor(),
                                                message.data(),
                                                messageSize,
                                                sendOptions
                                            );

        if ( messageBytesSent < 0 )
        {
            throw SYSTEM_EXCEPTION(OutgoingDataCouldNotBeSent);
        }

        //  "sendMessage" may actually sendMessage less bytes than "data" contains. It's also an exceptional situation, thus must be checked for.
        //  "messageBytesSent" and "messageSize" are signed and unsigned types respectively. Explicit conversion is required to compare them.
        //  "messageBytesSent" is guaranteed to be non-negative at this point, thus it can be correctly cast to "size_t".
        else if ( static_cast<size_t>(messageBytesSent) < messageSize )
        {
            throw EXCEPTION(OutgoingDataCouldNotBeSentCompletely);
        }

    }

    void                TcpSocket::sendMessageWithDelimiter ( const std::string& message , int sendOptions ) const
    {
        std::string messageWithDelimiter = message + std::string(this->messageDelimiter);
        this->sendMessage(messageWithDelimiter,sendOptions);
    }


}