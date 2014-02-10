#ifndef _TCP_SERVER_HPP_
#define _TCP_SERVER_HPP_

#include <list>
#include <algorithm>
#include <chrono>
#include <thread.hpp>
#include <event.hpp>
#include <Unet/SocketServer.hpp>
#include <Unet/TcpSocket.hpp>
#include <Unet/Datagram.hpp>
#include <Unet/TcpServerEvents.hpp>

namespace Unet
{

    typedef std::list<TcpSocket>                TcpSocketsList;

    typedef TcpSocketsList::iterator            TcpSocketsListItr;

    typedef TcpSocketsList::const_iterator      TcpSocketsListCitr;

    enum TcpReceiveMode
    {
        TCP_RECEIVE_MODE_DEFAULT,
        TCP_RECEIVE_MODE_BY_SIZE,
        TCP_RECEIVE_MODE_BY_DELIMITER
    };

    class TcpServer
        :
            public SocketServer
    {
        public:
            explicit                            TcpServer ( void );
            virtual                             ~TcpServer ( void ) noexcept override;
            bool                                getLaunched ( void ) const override;
            int                                 getConnectionsLimit ( void ) const;
            void                                setConnectionsLimit ( int connectionsLimit );
            TcpReceiveMode                      getReceiveMode ( void ) const;
            void                                setReceiveMode ( TcpReceiveMode receiveMode );
            unsigned int                        getKeepAliveTimeout ( void ) const;
            void                                setKeepAliveTimeout ( unsigned int keepAliveTimeout );
            unsigned int                        getDisconnectTimeout ( void ) const;
            void                                setDisconnectTimeout ( unsigned int disconnectTimeout );
            size_t                              getMessageSize ( void ) const;
            void                                setMessageSize ( size_t messageSize );
            std::string                         getMessageDelimiter ( void ) const;
            void                                setMessageDelimiter ( const std::string& messageDelimiter );
            void                                sendMessage ( TcpSocket& tcpSocket , const std::string& message );
            TcpServerEventClientConnected       clientConnectedEvent;
            TcpServerEventClientDisconnected    clientDisconnectedEvent;
            TcpServerEventMessageReceived       messageReceivedEvent;
            TcpServerEventMessageSent           messageSentEvent;
        protected:
            void                                startProcedure ( void ) override;
            void                                stopProcedure ( void ) override;
            void                                launchSocket ( void );
            void                                launchRoutines ( void );
            void                                stopRoutines ( void );
            void                                stopSocket ( void );
            void                                removeClient ( TcpSocket& clientSocket );
            static void                         routineAccept ( TcpServer* tcpServerPtr );
            static void                         routineReceive ( TcpServer* tcpServerPtr );
            static void                         routineKeepAlive ( TcpServer* tcpServerPtr );
            TcpSocket                           serverSocket;
            TcpSocketsList                      clientSockets;
            std::raii_thread_manual             threadAccept;
            std::raii_thread_manual             threadReceive;
            std::raii_thread_manual             threadKeepAlive;
            TcpReceiveMode                      receiveMode;
            unsigned int                        keepAliveTimeout;
            unsigned int                        disconnectTimeout;
        private:
                                                TcpServer ( const TcpServer& tcpServer );
            TcpServer&                          operator= ( const TcpServer& tcpServer );
    };

}

#endif // _TCP_SERVER_HPP_
