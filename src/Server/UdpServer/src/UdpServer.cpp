#include "UdpServer.hpp"

namespace Unet
{

            UdpServer::UdpServer ( void )
                :
                    active(false)
    {

    }

            UdpServer::~UdpServer ( void )
    {
        this->stop();
    }

    bool    UdpServer::getIsActive ( void ) const
    {
        std::lock_guard<std::mutex> launchedLockGuard(this->launchedMutex);
        return this->active;
    }

    void    UdpServer::launch ( void )
    {
        //  Protect against "stop" and "sendDatagram"
        std::lock_guard<std::mutex> masterLockGuard(this->masterMutex);

        this->launchSocket();
        this->launchRoutine();
    }

    void    UdpServer::stop ( void )
    {
        //  Protect against "launch" and "sendDatagram"
        std::lock_guard<std::mutex> masterLockGuard(this->masterMutex);

        this->stopSocket();
        this->stopRoutine();
    }

    void    UdpServer::sendDatagram ( const Unet::Datagram& datagram )
    {
        //  Protect against "launch" and "stop"
        std::lock_guard<std::mutex> masterLockGuard(this->masterMutex);

        //  Server could not send datagrams if it is not in launched state
        this->checkIsLaunched();

        //  Server is guaranteed to be in a launched state if control reaches this line
        this->socket.sendDatagram(datagram);
    }

    void    UdpServer::launchSocket ( void )
    {
        this->socket.open();
        this->socket.setOption(SO_REUSEADDR,1);
        this->socket.bind(*this->addressShrPtr.get());
    }

    void    UdpServer::launchRoutine ( void )
    {
        this->thread = std::move(std::thread(UdpServer::routine,this));
        this->active = true;
    }

    void    UdpServer::stopSocket ( void )
    {
        try
        {
            //  Will throw e. g. if the socket is not opened yet.
            this->stop();
        }
        catch ( Unet::Exception )
        {
        }
        catch ( ... )
        {
        }
    }

    void    UdpServer::stopRoutine ( void )
    {
        try
        {
            //  This flag will signal the child thread to stop
    //        std::unique_lock<std::mutex> uniqueLock(this->masterMutex);
  //          uniqueLock.lock();
            this->active = false;
//            uniqueLock.unlock();

            //  Will throw "std::system_error" is the thread is not in joinable state or if some error occurs.
            //  http://en.cppreference.com/w/cpp/thread/thread/join
            this->thread.join();
        }
        catch ( std::system_error )
        {
            this->dispatchEvent(SocketServerEvent::COULD_NOT_TERMINATE_THREAD,nullptr);
        }
        catch ( ... )
        {
            //  Extra security
        }
    }

    void    UdpServer::recieveDatagram ( void )
    {
        try
        {
            std::lock_guard<std::mutex> lockGuard(this->masterMutex);

            //  No need to check if the socket has unread data since if it does not then
            //  an exception will be thrown by "UdpSocket::recieveDatagram" which will be caught
            //  by "UdpServer::recieveDatagramSafely". This will save some machine time
            //  when the socket does have unread data.
            Unet::Datagram recievedDatagram = this->socket.recieveDatagram();
            this->dispatchEvent(SocketServerEvent::MESSAGE_RECIEVED,&recievedDatagram);
        }
        catch ( ... )
        {
            // All exceptions must be caught. Otherwise the server thread will be terminated.
        }
    }

    void    UdpServer::checkIsLaunched ( void ) const
    {
        if ( this->active == false )
        {
            throw -1;
        }
    }

    void    UdpServer::checkIsNotLaunched ( void ) const
    {
        if ( this->active == true )
        {
            throw -1;
        }
    }

    void    UdpServer::routine ( UdpServer* udpServerPtr )
    {
        while ( true )
        {
            std::lock_guard<std::mutex> lockGuard(udpServerPtr->masterMutex);
            if  ( udpServerPtr->active == false )
            {
                return;
            }
            udpServerPtr->recieveDatagram();
        }
    }

}