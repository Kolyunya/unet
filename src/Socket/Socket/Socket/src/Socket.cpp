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

#include "Socket.hpp"

namespace Unet
{

                        Socket::Socket ( int domain , int type , int protocol , int descriptor )
                            :
                                File(descriptor),
                                domain(domain),
                                type(type),
                                protocol(protocol)
    {

    }

    bool                Socket::operator== ( const Socket& socket ) const
    {
        return this->getDescriptor() == socket.getDescriptor();
    }

    void                Socket::bind ( const AddressUniPtr& addressUniPtr )
    {

        int socketDescriptor = this->getDescriptor();
        const sockaddr* addressDataPtr = addressUniPtr->getDataPtr();
        socklen_t addressSize = addressUniPtr->getSizeRef();

        int socketBindResult = ::bind(socketDescriptor,addressDataPtr,addressSize);

        if ( socketBindResult != 0 )
        {
            throw SYSTEM_EXCEPTION(SocketCouldNotBeBound);
        }

    }

    void                Socket::connect ( const AddressUniPtr& addressUniPtr )
    {

        /*
            Linux Programmer's Manual - CONNECT(2)

            Connects the socket to the address specified by "addressUniPtr"
            The format of the address in addr is determined by the address space of
            the socket.

            If  the socket is of type SOCK_DGRAM then "addressUniPtr" is the
            address to which datagrams are sent by default, and the only  address
            from  which datagrams  are  received.

            If the socket is of type SOCK_STREAM or SOCK_SEQPACKET, this call
            attempts to make a connection to  the  socket that is bound to the
            address specified by addr.

            Generally, connection-based protocol sockets may successfully connect()
            only once; connectionless protocol sockets may use  connect()  multiple
            times to change their association.  Connectionless sockets may dissolve
            the association by connecting to an address with the  sa_family  member
            of sockaddr set to AF_UNSPEC (supported on Linux since kernel 2.2).
        */

        int connectResult = ::connect   (
                                            this->getDescriptor(),
                                            addressUniPtr->getDataPtr(),
                                            addressUniPtr->getSizeRef()
                                        );

        if ( connectResult != 0 )
        {

            throw SYSTEM_EXCEPTION(SocketCouldNotBeConnected);

        }

    }

    void                Socket::disconnect ( void )
    {

        /*
            Linux Programmer's Manual - CONNECT(2)

            Connects the socket to the address specified by "addressUniPtr"
            The format of the address in addr is determined by the address space of
            the socket.

            If  the socket is of type SOCK_DGRAM then "addressUniPtr" is the
            address to which datagrams are sent by default, and the only  address
            from  which datagrams  are  received.

            If the socket is of type SOCK_STREAM or SOCK_SEQPACKET, this call
            attempts to make a connection to  the  socket that is bound to the
            address specified by addr.

            Generally, connection-based protocol sockets may successfully connect()
            only once; connectionless protocol sockets may use  connect()  multiple
            times to change their association.  Connectionless sockets may dissolve
            the association by connecting to an address with the  sa_family  member
            of sockaddr set to AF_UNSPEC (supported on Linux since kernel 2.2).
        */

        this->connect(AddressUniPtr(new NullAddress()));

    }

    void                Socket::shutdown ( int options )
    {

        /*
            Linux Programmer's Manual - SHUTDOWN(2)

            The  shutdown()  call causes all or part of a full-duplex connection on
            the socket associated with sockfd to be shut down.  If how is  SHUT_RD,
            further  receptions  will  be  disallowed.   If how is SHUT_WR, further
            transmissions will be disallowed.  If how is SHUT_RDWR, further  recep-
            tions and transmissions will be disallowed.
        */

        int socketDescriptor = this->getDescriptor();

        int socketShutdownResult = ::shutdown(socketDescriptor,options);

        if ( socketShutdownResult != 0 )
        {

            throw SYSTEM_EXCEPTION(SocketCouldNotBeShutdown);

        }

    }

    bool                Socket::isBlocking ( void ) const
    {

        return ! this->isNonBlocking();

    }

    bool                Socket::isNonBlocking ( void ) const
    {

        return this->checkOptions(O_NONBLOCK);

    }

    bool                Socket::hasUnreadData ( size_t unreadDataSizeRequired ) const
    {

        if ( this->isReadyForRead() == false )
        {

            //  If socket is not ready for read it does not have any unread data
            return false;

        }

        else if ( unreadDataSizeRequired == 0 )
        {

            //  "unread_data_size_required" equal to zero means "Does socket have ANY AMMOUNT of unread data to read?"
            //  At this point socket is guaranteed to be ready to read, thus it has SOME AMMOUNT of unread data to read.
            return true;

        }

        size_t unread_data_size_available = this->getUnreadDataSize();

        bool has_required_ammount_of_unread_data = unread_data_size_available >= unreadDataSizeRequired;

        return has_required_ammount_of_unread_data;

    }

    bool                Socket::hasNoUnreadData ( size_t unreadDataSizeRequired ) const
    {

        return ! this->hasUnreadData(unreadDataSizeRequired);

    }

    void                Socket::waitForUnreadData ( size_t unreadDataSizeRequired ) const
    {

        while ( this->hasNoUnreadData(unreadDataSizeRequired) )
        {

            //  Required ammount of data has not arrived yet
            if ( this->isNonBlocking() )
            {

                //  Socket is in non-blocking state
                throw EXCEPTION(IncommingDataIsNotAvailableNow);

            }

            //  Socket is in blocking state... Wait for data to arrive...

        }

        //  Required ammount of data has arrived!

    }

    AddressUniPtr       Socket::getLocalAddress ( void ) const
    {

        // Address will construct an address object from raw data
        AddressFactory addressFactory;

        // Get pointers to raw data from address factory
        sockaddr* addressDataPtr = addressFactory.getDataPtr();
        socklen_t* addressSizePtr = addressFactory.getSizePtr();

        // "addressSizePtr" must point to the size of buffer pointed by "addressDataPtr"
        *addressSizePtr = Address::getSizeLimit();

        // Get local socket address
        int getLocalAddressSuccess = getsockname    (
                                                        this->getDescriptor(),
                                                        addressDataPtr,
                                                        addressSizePtr
                                                    );

        // Check operation success
        if ( getLocalAddressSuccess != 0 )
        {
            throw SYSTEM_EXCEPTION(CouldNotRetrieveSocketLocalAddress);
        }

        return addressFactory.getProduct();

    }

    AddressUniPtr       Socket::getPeerAddress ( void ) const
    {
        // Address will construct an address object from raw data
        AddressFactory addressFactory;

        // Get pointers to raw data from address factory
        sockaddr* addressDataPtr = addressFactory.getDataPtr();
        socklen_t* addressSizePtr = addressFactory.getSizePtr();

        // "addressSizePtr" must point to the size of buffer pointed by "addressDataPtr"
        *addressSizePtr = Address::getSizeLimit();

        // Get local socket address
        int getPeerAddressSuccess = getpeername     (
                                                        this->getDescriptor(),
                                                        addressDataPtr,
                                                        addressSizePtr
                                                    );

        // Check operation success
        if ( getPeerAddressSuccess != 0 )
        {
            throw SYSTEM_EXCEPTION(CouldNotRetrieveSocketPeerAddress);
        }

        return addressFactory.getProduct();
    }

    size_t              Socket::getUnreadDataSize ( void ) const
    {

        //  @description    For UDP sockets returns the size of the first pending datagram. For TCP sockets returns the size of data available for read.



        int unread_data_size;

        int get_unread_data_size_success = ioctl(this->getDescriptor(),FIONREAD,&unread_data_size);

        if ( get_unread_data_size_success == -1 )
        {

            throw SYSTEM_EXCEPTION(IncommingDataSizeCouldNotBeRetrieved);

        }

        //  "unread_data_size" is of a signed interger type. Funtion returns an instance of `size_t`, thus an excplicit conversion is required
        //  "ioctl" with "FIONREAD" option is guaranteed to return a number greater or equal to zero in "unread_data_size",
        //  thus "unread_data_size" can be converted to "unsigned int"
        return static_cast<size_t>(unread_data_size);

    }

    void                Socket::setBlocking ( void )
    {
        this->disableOptions(O_NONBLOCK);
    }

    void                Socket::setNonBlocking ( void )
    {
        this->enableOptions(O_NONBLOCK);
    }

    int                 Socket::allocate ( void )
    {

        int socketDescriptor = ::socket(this->domain,this->type,this->protocol);

        if ( socketDescriptor == -1 )
        {

            throw SYSTEM_EXCEPTION(SocketCouldNotBeOpened);

        }

        return socketDescriptor;

    }

}