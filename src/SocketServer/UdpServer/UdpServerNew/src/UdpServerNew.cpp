#include "UdpServerNew.hpp"

namespace Unet
{

            UdpServerNew::UdpServerNew ( void )
                :
                    thread(std::bind(UdpServerNew::routine,this))
    {

    }

            UdpServerNew::~UdpServerNew ( void ) noexcept
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

    void    UdpServerNew::launch ( void )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->mutex);
        this->checkIsNotLaunched();
        this->launchSocket();
        this->launchRoutine();
    }

    void    UdpServerNew::stop ( void )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->mutex);
        this->checkIsLaunched();
        this->stopRoutine();
        this->stopSocket();
    }

    void    UdpServerNew::sendDatagram ( const Unet::Datagram& datagram )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->mutex);
        this->checkIsLaunched();
        this->socket.sendDatagram(datagram);
//        this->dispatchEvent(SocketServerEvent::MESSAGE_SENT,static_cast<void*>(&datagram));
    }

    void    UdpServerNew::launchSocket ( void )
    {
        this->socket.open();
        this->socket.setOption(SO_REUSEADDR,1);
        this->socket.bind(*(this->addressShrPtr.get()));
    }

    void    UdpServerNew::launchRoutine ( void )
    {
        this->thread.launch();
    }

    void    UdpServerNew::stopRoutine ( void )
    {
        this->thread.stop();
    }

    void    UdpServerNew::stopSocket ( void )
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

    void    UdpServerNew::checkIsLaunched ( void ) const
    {/*
        if ( this->thread_ptr == nullptr )
        {
            throw -1;
        }*/
    }

    void    UdpServerNew::checkIsNotLaunched ( void ) const
    {/*
        if ( this->thread_ptr != nullptr )
        {
            throw -1;
        }*/
    }

    void    UdpServerNew::routine ( UdpServerNew* udpServerNewPtr )
    {

        //  Try to lock "mutex"
        std::unique_lock<std::recursive_mutex> masterUniqueLock(udpServerNewPtr->mutex,std::defer_lock);
        if ( masterUniqueLock.try_lock() )
        {
            //  Make sure the server is in launched state
            udpServerNewPtr->checkIsLaunched();

            //  If the socket has unread data
            if ( udpServerNewPtr->socket.hasUnreadData() )
            {

                std::cout << "DATA!\n";

                //  Recieve the data
                Unet::Datagram recievedDatagram = udpServerNewPtr->socket.recieveDatagram();

                //  Emit respective event
                udpServerNewPtr->dispatchEvent(SocketServerEvent::MESSAGE_RECIEVED,&recievedDatagram);
            }
        }

    }

}