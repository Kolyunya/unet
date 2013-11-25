#include "UdpServer.hpp"

namespace Unet
{

            UdpServer::UdpServer ( void )
    {

        std::cout << "___1___"<< std::endl;

    }

            UdpServer::~UdpServer ( void ) noexcept
    {
        this->destructRoutine();
        this->destructSocket();
    }

    void    UdpServer::sendDatagram ( const Unet::Datagram& datagram )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->mutex);
        this->socket.sendDatagram(datagram);
    }

    void    UdpServer::launch ( void )
    {
        this->configureSocket();
        this->launchRoutine();
    }

    void    UdpServer::stop ( void )
    {
        this->socket.close();
    }

    void    UdpServer::routine ( UdpServer* udpServerPtr ) noexcept
    {
        while ( true )
        {
            udpServerPtr->recieveDatagram();
        }
    }

    void    UdpServer::configureSocket ( void )
    {
        this->socket.open();
        this->socket.setOption(SO_REUSEADDR,1);
        this->socket.bind(*this->addressShrPtr.get());
    }

    void    UdpServer::launchRoutine ( void )
    {
        this->thread = std::move(std::thread(UdpServer::routine,this));
    }

    void    UdpServer::destructRoutine ( void ) noexcept
    {
        try
        {
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

    void    UdpServer::destructSocket ( void ) noexcept
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

    void    UdpServer::recieveDatagram ( void ) noexcept
    {
        try
        {
            std::lock_guard<std::recursive_mutex> lockGuard(this->mutex);

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

}