#include "UdpServer.hpp"

namespace Unet
{

            UdpServer::UdpServer ( void )
                :
                    thread(std::bind(UdpServer::routine,this))
    {

    }

            UdpServer::~UdpServer ( void ) noexcept
    {
        try
        {
            //  May throw e. g. if the server is not in launched state
            this->stop();
        }
        catch ( ... )
        {
            //  Destructor must never throw.
            //  http://stackoverflow.com/questions/130117/throwing-exceptions-out-of-a-destructor
        }
    }

    void    UdpServer::launch ( void )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->mutex);
        this->checkIsNotLaunched();
        this->launchSocket();
        this->launchRoutine();
    }

    void    UdpServer::stop ( void )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->mutex);
        this->checkIsLaunched();
        this->stopRoutine();
        this->stopSocket();
    }

    void    UdpServer::sendDatagram ( const Unet::Datagram& datagram )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->mutex);
        this->checkIsLaunched();
        this->socket.sendDatagram(datagram);
        //this->dispatchEvent(SocketServerEvent::MESSAGE_SENT,static_cast<void*>(&datagram));
    }

    bool    UdpServer::getLaunched ( void ) const
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->mutex);
        return this->thread.isActive();
    }

    void    UdpServer::launchSocket ( void )
    {
        this->socket.open();
        this->socket.setOption(SO_REUSEADDR,1);
        this->socket.bind(*(this->addressShrPtr.get()));
    }

    void    UdpServer::launchRoutine ( void )
    {
        this->thread.launch();
    }

    void    UdpServer::stopRoutine ( void )
    {
        this->thread.stop();
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

    void    UdpServer::checkIsLaunched ( void ) const
    {
        if ( this->getLaunched() == false )
        {
            throw -1;
        }
    }

    void    UdpServer::checkIsNotLaunched ( void ) const
    {
        if ( this->getLaunched() == true )
        {
            throw -1;
        }
    }

    void    UdpServer::routine ( UdpServer* UdpServerPtr )
    {

        //  Try to lock "mutex"
        std::unique_lock<std::recursive_mutex> masterUniqueLock(UdpServerPtr->mutex,std::defer_lock);
        if ( masterUniqueLock.try_lock() )
        {
            //  Make sure the server is in launched state
            UdpServerPtr->checkIsLaunched();

            //  If the socket has unread data
            if ( UdpServerPtr->socket.hasUnreadData() )
            {

                std::cout << "DATA!\n";

                //  Recieve the data
                Unet::Datagram recievedDatagram = UdpServerPtr->socket.recieveDatagram();

                //  Emit respective event
                UdpServerPtr->dispatchEvent(SocketServerEvent::MESSAGE_RECIEVED,&recievedDatagram);
            }
        }

    }

}