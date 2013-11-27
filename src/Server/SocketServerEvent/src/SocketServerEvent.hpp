#ifndef _UNET_SOCKET_SERVER_EVENT_HPP_
#define _UNET_SOCKET_SERVER_EVENT_HPP_

namespace Unet
{
    class SocketServerEvent
    {
        public:
            static const int MESSAGE_SENT = 0;
            static const int MESSAGE_RECIEVED = 0;
            static const int COULD_NOT_TERMINATE_THREAD = 1;

    };
}

#endif // _UNET_SOCKET_SERVER_EVENT_HPP_