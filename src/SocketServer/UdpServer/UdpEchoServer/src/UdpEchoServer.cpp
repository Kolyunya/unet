#include "UdpEchoServer.hpp"

namespace Unet
{

            UdpEchoServer::UdpEchoServer ( void )
    {
        this->startedEvent.add_listener(this,&UdpEchoServer::startedHandler);
        this->stoppedEvent.add_listener(this,&UdpEchoServer::stoppedHandler);
        this->datagramReceivedEvent.add_listener(this,&UdpEchoServer::datagramRecievedHandler);
        this->datagramSentEvent.add_listener(this,&UdpEchoServer::datagramSentHandler);

    }

    void    UdpEchoServer::startedHandler ( void )
    {
        std::string startedEventMessage = "UDP server has started successfully";
        this->log(startedEventMessage);
    }

    void    UdpEchoServer::stoppedHandler ( void )
    {
        std::string stopEventMessage = "UDP server has stopped successfully";
        this->log(stopEventMessage);
    }

    void    UdpEchoServer::datagramRecievedHandler ( const Datagram& recievedDatagram )
    {
        std::string datagramReceievedEventMessage = recievedDatagram.addressUniPtr->toString();
        datagramReceievedEventMessage += " -> ";
        datagramReceievedEventMessage += recievedDatagram.message;
        this->log(datagramReceievedEventMessage);
        this->sendDatagram(recievedDatagram);
    }

    void    UdpEchoServer::datagramSentHandler ( const Datagram& sentDatagram )
    {
        std::string datagramSentEventMessage = sentDatagram.addressUniPtr->toString();
        datagramSentEventMessage += " <- ";
        datagramSentEventMessage += sentDatagram.message;
        this->log(datagramSentEventMessage);
    }

}