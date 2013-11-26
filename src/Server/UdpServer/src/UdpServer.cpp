#include "UdpServer.hpp"

namespace Unet
{

            UdpServer::UdpServer ( void )
                :
                    launched(false)
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
        std::lock_guard<std::mutex> masterLockGuard(this->masterMutex);
        this->checkIsNotLaunched();
        this->launchSocket();
        this->launchRoutine();
    }

    void    UdpServer::stop ( void )
    {
        std::lock_guard<std::mutex> masterLockGuard(this->masterMutex);
        this->checkIsLaunched();
        this->stopRoutine();
        this->stopSocket();
    }

    void    UdpServer::sendDatagram ( const Unet::Datagram& datagram )
    {
        std::lock_guard<std::mutex> masterLockGuard(this->masterMutex);
        this->checkIsLaunched();
        this->socket.sendDatagram(datagram);
    }

    bool    UdpServer::getLaunched ( void ) const
    {
        std::lock_guard<std::mutex> launchedLockGuard(this->launchedMutex);
        return this->launched;
    }

    void    UdpServer::launchSocket ( void )
    {
        this->socket.open();
        this->socket.setOption(SO_REUSEADDR,1);
        this->socket.bind(*(this->addressShrPtr.get()));
    }

    void    UdpServer::launchRoutine ( void )
    {
        std::lock_guard<std::mutex> launchedLockGuard(this->launchedMutex);
        this->launched = true;
        this->thread = std::thread(UdpServer::routine,this);
    }

    void    UdpServer::stopRoutine ( void )
    {
        try
        {

			//	Lock "UdpServer::launched" leave an ability to unlock manually
			std::unique_lock<std::mutex> launchedUniqueLock(this->launchedMutex);

            //  This flag will signal the child thread to stop
            this->launched = false;

			//	Release "UdpServer::launched" so that the child thread could read it
            launchedUniqueLock.unlock();

            //  Will throw "std::system_error" is the thread is not in joinable state or if some error occurs.
            this->thread.join();
        }
        catch ( ... )
        {
            this->dispatchEvent(SocketServerEvent::COULD_NOT_TERMINATE_THREAD,nullptr);
        }
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
        std::lock_guard<std::mutex> launchedLockGuard(this->launchedMutex);
        if ( this->launched == false )
        {
            throw -1;
        }
    }

    void    UdpServer::checkIsNotLaunched ( void ) const
    {
        std::lock_guard<std::mutex> launchedLockGuard(this->launchedMutex);
        if ( this->launched == true )
        {
            throw -1;
        }
    }

    void    UdpServer::routine ( UdpServer* udpServerPtr )
    {
        while ( true )
        {
			try
			{
                //  Try to lock "launchedMutex"
				std::unique_lock<std::mutex> launchedUniqueLock(udpServerPtr->launchedMutex,std::defer_lock);
                if ( launchedUniqueLock.try_lock() )
                {
                    //  Check if the routine is supposed to terminate
                    if  ( udpServerPtr->launched == false )
                    {
                        //  Terminate the child thread
                        return;
                    }
                }

                //  Try to lock "masterMutex"
                std::unique_lock<std::mutex> masterUniqueLock(udpServerPtr->masterMutex,std::defer_lock);
                if ( masterUniqueLock.try_lock() )
                {
                    //  Make sure the server is in launched state
                    udpServerPtr->checkIsLaunched();

                    //  No need to check if the socket has unread data since if it does not then
                    //  an exception will be thrown by "UdpSocket::recieveDatagram" which will be caught
                    //  by "UdpServer::recieveDatagramSafely". This will save some machine time
                    //  when the socket does have unread data.
                    Unet::Datagram recievedDatagram = udpServerPtr->socket.recieveDatagram();
                    udpServerPtr->dispatchEvent(SocketServerEvent::MESSAGE_RECIEVED,&recievedDatagram);
                }
			}
			catch ( ... )
			{
				// Ignore errors
			}
        }
    }

}