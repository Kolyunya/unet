#ifndef _UNET_SOCKET_SERVER_EVENTS_HPP_
#define _UNET_SOCKET_SERVER_EVENTS_HPP_

#include <xtd/pubsub.hpp>

namespace Unet
{

    class SocketServer;

    class SocketServerEventStarted
        :
            public xtd::pubsub::event<>
    {
        friend class UdpServer;
    };

    class SocketServerEventStopped
        :
            public xtd::pubsub::event<>
    {
        friend class UdpServer;
    };

}

#endif // _UNET_SOCKET_SERVER_EVENTS_HPP_