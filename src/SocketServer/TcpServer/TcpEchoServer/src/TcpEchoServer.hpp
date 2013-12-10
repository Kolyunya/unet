#ifndef _UNET_TCP_ECHO_SERVER_HPP_
#define _UNET_TCP_ECHO_SERVER_HPP_

#include <iostream.hpp>
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
            void                clientConnectedEventHandler ( const TcpSocket& tcpSocket );
            void                clientDisconnectedEventHandler ( const TcpSocket& tcpSocket );
            void                messageReceievedEventHandler ( const TcpSocket& tcpSocket , const std::string& message );
            void                messageSentEventHandler ( const TcpSocket& tcpSocket , const std::string& message );
        private:
                                TcpEchoServer ( const TcpEchoServer& TcpEchoServer );
            TcpEchoServer&      operator= ( const TcpEchoServer& TcpEchoServer );
    };
}

#endif // _UNET_TCP_ECHO_SERVER_HPP_