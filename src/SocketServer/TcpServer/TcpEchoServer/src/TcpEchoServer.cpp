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
        std::string clientConnectedEventMessage = "[";
        clientConnectedEventMessage += tcpSocket.getPeerAddress()->toString();
        clientConnectedEventMessage += "] - connected\n";
        std::coutmt << clientConnectedEventMessage;
    }

    void    TcpEchoServer::clientDisconnectedEventHandler ( const TcpSocket& tcpSocket )
    {
        std::string clientDisconnectedEventMessage = "[";
        clientDisconnectedEventMessage += tcpSocket.getPeerAddress()->toString();
        clientDisconnectedEventMessage += "] - disconnected\n";
        std::coutmt << clientDisconnectedEventMessage;
    }

    void    TcpEchoServer::messageReceievedEventHandler ( const TcpSocket& tcpSocket , const std::string& message )
    {
        std::string messageReceievedEventMessage = "[";
        messageReceievedEventMessage += tcpSocket.getPeerAddress()->toString();
        messageReceievedEventMessage += "] -> ";
        messageReceievedEventMessage += message;
        messageReceievedEventMessage += "\n";
        std::coutmt << messageReceievedEventMessage;
        this->sendMessage(tcpSocket,message);
    }

    void    TcpEchoServer::messageSentEventHandler ( const TcpSocket& tcpSocket , const std::string& message )
    {
        std::string messageSentEventMessage = "[";
        messageSentEventMessage += tcpSocket.getPeerAddress()->toString();
        messageSentEventMessage += "] <- ";
        messageSentEventMessage += message;
        messageSentEventMessage += "\n";
        std::coutmt << messageSentEventMessage;
    }

}