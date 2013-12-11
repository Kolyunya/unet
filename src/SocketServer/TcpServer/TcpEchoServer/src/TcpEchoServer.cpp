#include "TcpEchoServer.hpp"

namespace Unet
{

            TcpEchoServer::TcpEchoServer ( void )
    {
        this->clientConnectedEvent.add_listener(this,&TcpEchoServer::clientConnectedEventHandler);
        this->clientDisconnectedEvent.add_listener(this,&TcpEchoServer::clientDisconnectedEventHandler);
        this->messageReceivedEvent.add_listener(this,&TcpEchoServer::messageReceievedEventHandler);
        this->messageSentEvent.add_listener(this,&TcpEchoServer::messageSentEventHandler);
    }

            TcpEchoServer::~TcpEchoServer ( void ) noexcept
    {

    }

    void    TcpEchoServer::clientConnectedEventHandler ( const TcpSocket& tcpSocket )
    {
        std::string clientConnectedEventMessage = tcpSocket.getPeerAddress()->toString();
        clientConnectedEventMessage += " - connected";
        this->log(clientConnectedEventMessage);
    }

    void    TcpEchoServer::clientDisconnectedEventHandler ( const TcpSocket& tcpSocket )
    {
        std::string clientDisconnectedEventMessage = tcpSocket.getPeerAddress()->toString();
        clientDisconnectedEventMessage += " - disconnected";
        this->log(clientDisconnectedEventMessage);
    }

    void    TcpEchoServer::messageReceievedEventHandler ( const TcpSocket& tcpSocket , const std::string& message )
    {
        std::string messageReceievedEventMessage = tcpSocket.getPeerAddress()->toString();
        messageReceievedEventMessage += " -> ";
        messageReceievedEventMessage += message;
        this->log(messageReceievedEventMessage);
        this->sendMessage(tcpSocket,message);
    }

    void    TcpEchoServer::messageSentEventHandler ( const TcpSocket& tcpSocket , const std::string& message )
    {
        std::string messageSentEventMessage = tcpSocket.getPeerAddress()->toString();
        messageSentEventMessage += " <- ";
        messageSentEventMessage += message;
        this->log(messageSentEventMessage);
    }

}