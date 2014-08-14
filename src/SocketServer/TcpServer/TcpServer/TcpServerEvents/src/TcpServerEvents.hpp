#ifndef _UNET_TCP_SERVER_EVENTS_HPP_
#define _UNET_TCP_SERVER_EVENTS_HPP_

#include <xtd/pubsub.hpp>

namespace Unet
{

    class TcpServer;

    class TcpServerEventClientConnected
        :
            public xtd::pubsub::event<TcpSocketShrPtr&>
    {
        friend class TcpServer;
    };

    class TcpServerEventClientDisconnected
        :
            public xtd::pubsub::event<TcpSocketShrPtr&>
    {
        friend class TcpServer;
    };

    class TcpServerEventMessageReceived
        :
            public xtd::pubsub::event<TcpSocketShrPtr&,const std::string&>
    {
        friend class TcpServer;
    };

    class TcpServerEventMessageSent
        :
            public xtd::pubsub::event<TcpSocketShrPtr&,const std::string&>
    {
        friend class TcpServer;
    };

}

#endif  //  _UNET_TCP_SERVER_EVENTS_HPP_