#include "UdpEchoServer.hpp"

namespace Unet
{

            UdpEchoServer::UdpEchoServer ( void )
    {
        this->addEventListener(SocketServerEvent::MESSAGE_RECIEVED,this,&UdpEchoServer::datagramRecievedHandler);

    }

            UdpEchoServer::~UdpEchoServer ( void ) noexcept
    {

    }

    void    UdpEchoServer::datagramRecievedHandler ( Datagram* recievedDatagramPtr )
    {
        this->sendDatagram(*recievedDatagramPtr);
    }

}