#ifndef _UNET_UDP_ECHO_SERVER_HPP_
#define _UNET_UDP_ECHO_SERVER_HPP_

#include <Unet/UdpServer.hpp>

namespace Unet
{
    class UdpEchoServer
        :
            public UdpServer
    {
        public:
            explicit            UdpEchoServer ( void );
            virtual             ~UdpEchoServer ( void ) noexcept override;
        protected:
            void                datagramRecievedHandler ( Datagram* recievedDatagramPtr );
        private:
                                UdpEchoServer ( const UdpEchoServer& udpEchoServer );
            UdpEchoServer&      operator= ( const UdpEchoServer& udpEchoServer );
    };
}

#endif // _UNET_UDP_ECHO_SERVER_HPP_