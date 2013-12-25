#ifndef _UNET_TCP_ECHO_SERVER_HPP_
#define _UNET_TCP_ECHO_SERVER_HPP_

#include <log.hpp>
#include <Unet/TcpServer.hpp>

namespace Unet
{
    class TcpEchoServer
        :
            public TcpServer,
            public std::console_logger
    {
        public:
            explicit            TcpEchoServer ( void );
            virtual             ~TcpEchoServer ( void ) noexcept override = default;
        protected:
            void                startedHandler ( void );
            void                stoppedHandler ( void );
            void                clientConnectedEventHandler ( TcpSocket& tcpSocket );
            void                clientDisconnectedEventHandler ( TcpSocket& tcpSocket );
            void                messageReceievedEventHandler ( TcpSocket& tcpSocket , const std::string& message );
            void                messageSentEventHandler ( TcpSocket& tcpSocket , const std::string& message );
        private:
                                TcpEchoServer ( const TcpEchoServer& TcpEchoServer );
            TcpEchoServer&      operator= ( const TcpEchoServer& TcpEchoServer );
    };
}

#endif // _UNET_TCP_ECHO_SERVER_HPP_