#include "TcpEchoServer.hpp"

namespace Unet
{

            TcpEchoServer::TcpEchoServer ( void )
    {
        this->startedEvent.add_listener(this,&TcpEchoServer::startedHandler);
        this->stoppedEvent.add_listener(this,&TcpEchoServer::stoppedHandler);
        this->clientConnectedEvent.add_listener(this,&TcpEchoServer::clientConnectedEventHandler);
        this->clientDisconnectedEvent.add_listener(this,&TcpEchoServer::clientDisconnectedEventHandler);
        this->messageReceivedEvent.add_listener(this,&TcpEchoServer::messageReceievedEventHandler);
        this->messageSentEvent.add_listener(this,&TcpEchoServer::messageSentEventHandler);
    }

    void    TcpEchoServer::startedHandler ( void )
    {
        std::string startedEventMessage = "TCP server has started successfully";
        this->log(startedEventMessage);
    }

    void    TcpEchoServer::stoppedHandler ( void )
    {
        std::string stopEventMessage = "TCP server has stopped successfully";
        this->log(stopEventMessage);
    }

    void    TcpEchoServer::clientConnectedEventHandler ( TcpSocket& tcpSocket )
    {
        std::string clientConnectedEventMessage = tcpSocket.getPeerAddress()->toString();
        clientConnectedEventMessage += " - connected";
        this->log(clientConnectedEventMessage);
    }

    void    TcpEchoServer::clientDisconnectedEventHandler ( TcpSocket& )
    {
        std::string clientDisconnectedEventMessage = "client disconnected";
        this->log(clientDisconnectedEventMessage);
    }

    void    TcpEchoServer::messageReceievedEventHandler ( TcpSocket& tcpSocket , const std::string& message )
    {
        std::string messageReceievedEventMessage = tcpSocket.getPeerAddress()->toString();
        messageReceievedEventMessage += " -> ";
        messageReceievedEventMessage += message;
        this->log(messageReceievedEventMessage);
        this->sendMessage(tcpSocket,message);
    }

    void    TcpEchoServer::messageSentEventHandler ( TcpSocket& tcpSocket , const std::string& message )
    {
        std::string messageSentEventMessage = tcpSocket.getPeerAddress()->toString();
        messageSentEventMessage += " <- ";
        messageSentEventMessage += message;
        this->log(messageSentEventMessage);
    }

}