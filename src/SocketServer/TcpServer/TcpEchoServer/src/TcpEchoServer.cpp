#include "TcpEchoServer.hpp"

namespace Unet
{

            TcpEchoServer::TcpEchoServer ( void )
    {
        this->registerEventListeneter();
    }

    void    TcpEchoServer::registerEventListeneter ( void )
    {
        this->startedEvent.add_listener(this,&TcpEchoServer::serverStartedHandler);
        this->stoppedEvent.add_listener(this,&TcpEchoServer::serverStoppedHandler);
        this->clientConnectedEvent.add_listener(this,&TcpEchoServer::clientConnectedEventHandler);
        this->clientDisconnectedEvent.add_listener(this,&TcpEchoServer::clientDisconnectedEventHandler);
        this->messageReceivedEvent.add_listener(this,&TcpEchoServer::messageReceievedEventHandler);
        this->messageSentEvent.add_listener(this,&TcpEchoServer::messageSentEventHandler);
    }

    void    TcpEchoServer::serverStartedHandler ( void )
    {
        std::string startedEventMessage = "TCP server has started successfully";
        this->log(startedEventMessage);
    }

    void    TcpEchoServer::serverStoppedHandler ( void )
    {
        std::string stopEventMessage = "TCP server has stopped successfully";
        this->log(stopEventMessage);
    }

    void    TcpEchoServer::clientConnectedEventHandler ( TcpSocketShrPtr& tcpSocketShrPtr )
    {
        std::string clientConnectedEventMessage = tcpSocketShrPtr->getPeerAddress()->toString();
        clientConnectedEventMessage += " - connected";
        this->log(clientConnectedEventMessage);
    }

    void    TcpEchoServer::clientDisconnectedEventHandler ( TcpSocketShrPtr& )
    {
        std::string clientDisconnectedEventMessage = "client disconnected";
        this->log(clientDisconnectedEventMessage);
    }

    void    TcpEchoServer::messageReceievedEventHandler ( TcpSocketShrPtr& tcpSocketShrPtr , const std::string& message )
    {
        std::string messageReceievedEventMessage = tcpSocketShrPtr->getPeerAddress()->toString();
        messageReceievedEventMessage += " -> ";
        messageReceievedEventMessage += message;
        this->log(messageReceievedEventMessage);
        this->sendMessage(tcpSocketShrPtr,message);
    }

    void    TcpEchoServer::messageSentEventHandler ( TcpSocketShrPtr& tcpSocketShrPtr , const std::string& message )
    {
        std::string messageSentEventMessage = tcpSocketShrPtr->getPeerAddress()->toString();
        messageSentEventMessage += " <- ";
        messageSentEventMessage += message;
        this->log(messageSentEventMessage);
    }

}