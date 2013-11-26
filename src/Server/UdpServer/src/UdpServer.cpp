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
        this->stop();
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
        this->socket.bind(*(this->addressShrPtr.get()));
    }

    void    UdpServer::launchRoutine ( void )
    {
        this->thread = std::thread(UdpServer::routine,this);
        this->launched = true;
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
        if ( this->launched == false )
        {
            throw -1;
        }
    }

    void    UdpServer::checkIsNotLaunched ( void ) const
    {
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
				//	Lock "UdpServer::launched" leave an ability to unlock manually
				std::unique_lock<std::mutex> launchedUniqueLock(udpServerPtr->launchedMutex);

				if  ( udpServerPtr->launched == false )
				{
					return;
				}

				//	Release "UdpServer::launched" so that the child thread could read it
				launchedUniqueLock.unlock();

				udpServerPtr->recieveDatagram();
			}
			catch ( ... )
			{
				// Ignore errors
			}
        }
    }

}