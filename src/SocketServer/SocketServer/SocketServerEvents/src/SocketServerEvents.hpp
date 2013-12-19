#ifndef _UNET_SOCKET_SERVER_EVENTS_HPP_
#define _UNET_SOCKET_SERVER_EVENTS_HPP_

#include <event.hpp>

namespace Unet
{

    class SocketServer;

    class SocketServerEventStarted
        :
            public std::event<>
    {
        friend class UdpServer;
    };

    class SocketServerEventStopped
        :
            public std::event<>
    {
        friend class UdpServer;
    };

}

#endif // _UNET_SOCKET_SERVER_EVENTS_HPP_