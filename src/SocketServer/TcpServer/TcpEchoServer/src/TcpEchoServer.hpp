#ifndef _UNET_TCP_ECHO_SERVER_HPP_
#define _UNET_TCP_ECHO_SERVER_HPP_

#include <Unet/TcpServer.hpp>

namespace Unet
{
    class TcpEchoServer
        :
            public TcpServer
    {
        public:
            explicit            TcpEchoServer ( void );
            virtual             ~TcpEchoServer ( void ) noexcept override;
        protected:
            void                datagramRecievedHandler ( Datagram* recievedDatagramPtr );
        private:
                                TcpEchoServer ( const TcpEchoServer& TcpEchoServer );
            TcpEchoServer&      operator= ( const TcpEchoServer& TcpEchoServer );
    };
}

#endif // _UNET_TCP_ECHO_SERVER_HPP_