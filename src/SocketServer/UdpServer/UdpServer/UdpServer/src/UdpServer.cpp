#include "UdpServer.hpp"

namespace Unet
{

            UdpServer::UdpServer ( void )
                :
                    recieveThread(std::bind(UdpServer::routine,this))
    {

    }

            UdpServer::~UdpServer ( void ) noexcept
    {
        try
        {
            this->stop();
        }
        catch ( ... )
        {
            return;
        }

    }

    bool    UdpServer::getLaunched ( void ) const
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        return this->recieveThread.isActive();
    }

    void    UdpServer::sendDatagram ( const Unet::Datagram& datagram )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->checkIsLaunched();
        this->socket.sendDatagram(datagram);
        this->datagramSentEvent.dispatch(datagram);
    }

    void    UdpServer::startProcedure ( void )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->checkIsNotLaunched();
        this->launchSocket();
        this->launchRoutine();
    }

    void    UdpServer::stopProcedure ( void )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->checkIsLaunched();
        this->stopRoutine();
        this->stopSocket();
    }

    void    UdpServer::launchSocket ( void )
    {
        this->socket.open();
        this->socket.setOption(SO_REUSEADDR,1);
        this->socket.bind(this->addressUniPtr);
    }

    void    UdpServer::launchRoutine ( void )
    {
        this->recieveThread.start();
    }

    void    UdpServer::stopRoutine ( void )
    {
        this->recieveThread.stop();
    }

    void    UdpServer::stopSocket ( void )
    {
        try
        {
            //  Will throw e. g. if the socket is not opened yet.
            this->socket.close();
        }
        catch ( ... )
        {
			// Ignore errors
        }
    }

    void    UdpServer::routine ( UdpServer* UdpServerPtr )
    {

        //  Try to lock "serverMutex"
        std::unique_lock<std::recursive_mutex> serverUniqueLock(UdpServerPtr->serverMutex,std::defer_lock);
        if ( serverUniqueLock.try_lock() )
        {
            //  If the socket has unread data
            if ( UdpServerPtr->socket.hasUnreadData() )
            {
                //  Receive the data
                Unet::Datagram recievedDatagram = UdpServerPtr->socket.recieveDatagram();

                //  Emit respective event
                UdpServerPtr->datagramReceivedEvent.dispatch(recievedDatagram);
            }
        }

    }

}