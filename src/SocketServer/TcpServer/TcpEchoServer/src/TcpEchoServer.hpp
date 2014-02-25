#ifndef _UNET_TCP_ECHO_SERVER_INCLUDED_
#define _UNET_TCP_ECHO_SERVER_INCLUDED_

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
        private:
                                TcpEchoServer ( const TcpEchoServer& TcpEchoServer ) = delete;
            TcpEchoServer&      operator= ( const TcpEchoServer& TcpEchoServer ) = delete;
            void                registerEventListeneter ( void );
            void                serverStartedHandler ( void );
            void                serverStoppedHandler ( void );
            void                clientConnectedEventHandler ( TcpSocketShrPtr& tcpSocketShrPtr );
            void                clientDisconnectedEventHandler ( TcpSocketShrPtr& tcpSocketShrPtr );
            void                messageReceievedEventHandler ( TcpSocketShrPtr& tcpSocketShrPtr , const std::string& message );
            void                messageSentEventHandler ( TcpSocketShrPtr& tcpSocketShrPtr , const std::string& message );
    };
}

#endif  //  _UNET_TCP_ECHO_SERVER_INCLUDED_