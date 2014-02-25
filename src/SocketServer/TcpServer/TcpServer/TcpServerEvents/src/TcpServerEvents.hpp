#ifndef _UNET_TCP_SERVER_EVENTS_HPP_
#define _UNET_TCP_SERVER_EVENTS_HPP_

#include <event.hpp>

namespace Unet
{

    class TcpServer;

    class TcpServerEventClientConnected
        :
            public std::event<TcpSocketShrPtr&>
    {
        friend class TcpServer;
    };

    class TcpServerEventClientDisconnected
        :
            public std::event<TcpSocketShrPtr&>
    {
        friend class TcpServer;
    };

    class TcpServerEventMessageReceived
        :
            public std::event<TcpSocketShrPtr&,const std::string&>
    {
        friend class TcpServer;
    };

    class TcpServerEventMessageSent
        :
            public std::event<TcpSocketShrPtr&,const std::string&>
    {
        friend class TcpServer;
    };

}

#endif  //  _UNET_TCP_SERVER_EVENTS_HPP_