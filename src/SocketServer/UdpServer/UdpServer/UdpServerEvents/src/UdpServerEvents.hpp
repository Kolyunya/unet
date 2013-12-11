#ifndef _UNET_UDP_SERVER_EVENTS_HPP_
#define _UNET_UDP_SERVER_EVENTS_HPP_

#include <event.hpp>

namespace Unet
{

    class UdpServer;

    class UdpServerEventDatagramReceived
        :
            public std::event<const Datagram&>
    {
        friend class UdpServer;
    };

    class UdpServerEventDatagramSent
        :
            public std::event<const Datagram&>
    {
        friend class UdpServer;
    };

}

#endif  //  _UNET_UDP_SERVER_EVENTS_HPP_