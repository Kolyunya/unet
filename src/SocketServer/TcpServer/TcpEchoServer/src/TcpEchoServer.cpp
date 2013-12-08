#include "TcpEchoServer.hpp"

namespace Unet
{

            TcpEchoServer::TcpEchoServer ( void )
    {
        this->addEventListener(SocketServerEvent::MESSAGE_RECIEVED,this,&TcpEchoServer::datagramRecievedHandler);
    }

            TcpEchoServer::~TcpEchoServer ( void ) noexcept
    {

    }

    void    TcpEchoServer::datagramRecievedHandler ( Datagram* recievedDatagramPtr )
    {
        this->sendDatagram(*recievedDatagramPtr);
    }

}