#ifndef _UNET_UDP_ECHO_SERVER_HPP_
#define _UNET_UDP_ECHO_SERVER_HPP_

#include <log.hpp>
#include <Unet/UdpServer.hpp>

namespace Unet
{
    class UdpEchoServer
        :
            public UdpServer,
            public std::console_logger
    {
        public:
            explicit            UdpEchoServer ( void );
            virtual             ~UdpEchoServer ( void ) noexcept override = default;
        protected:
            void                startedHandler ( void );
            void                stoppedHandler ( void );
            void                datagramRecievedHandler ( const Datagram& recievedDatagram );
            void                datagramSentHandler ( const Datagram& sentDatagram );
        private:
                                UdpEchoServer ( const UdpEchoServer& udpEchoServer );
            UdpEchoServer&      operator= ( const UdpEchoServer& udpEchoServer );
    };
}

#endif // _UNET_UDP_ECHO_SERVER_HPP_