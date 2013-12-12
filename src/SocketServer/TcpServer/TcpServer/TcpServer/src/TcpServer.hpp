#ifndef _TCP_SERVER_HPP_
#define _TCP_SERVER_HPP_

#include <vector>
#include <algorithm>
#include <chrono>
#include <thread.hpp>
#include <event.hpp>
#include <Unet/SocketServer.hpp>
#include <Unet/TcpSocket.hpp>
#include <Unet/Datagram.hpp>
#include <Unet/TcpServerEvents.hpp>
#include <iostream>
namespace Unet
{

    typedef std::vector<TcpSocket>              TcpSocketsVec;

    typedef TcpSocketsVec::const_iterator       TcpSocketsVecCitr;

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
            size_t                              getMessageSize ( void ) const;
            void                                setMessageSize ( size_t messageSize );
            std::string                         getMessageDelimiter ( void ) const;
            void                                setMessageDelimiter ( const std::string& messageDelimiter );
            TcpReceiveMode                      getReceiveMode ( void ) const;
            void                                setReceiveMode ( TcpReceiveMode receiveMode );
            void                                start ( void ) override;
            void                                stop ( void ) override;
            void                                sendMessage ( const TcpSocket& tcpSocket , const std::string& message );
            void                                sendMessageWithDelimiter ( const TcpSocket& tcpSocket , const std::string& message );
        protected:
            void                                launchSocket ( void );
            void                                launchRoutines ( void );
            void                                stopRoutines ( void );
            void                                stopSocket ( void );
            static void                         routineAccept ( TcpServer* tcpServerPtr );
            static void                         routineReceive ( TcpServer* tcpServerPtr );
            static void                         routinePing ( TcpServer* tcpServerPtr );
            TcpSocket                           serverSocket;
            TcpSocketsVec                       clientSockets;
            std::raii_thread_manual             threadAccept;
            std::raii_thread_manual             threadReceive;
            std::raii_thread_manual             threadPing;
            TcpReceiveMode                      receiveMode;
            TcpServerEventClientConnected       clientConnectedEvent;
            TcpServerEventClientDisconnected    clientDisconnectedEvent;
            TcpServerEventMessageReceived       messageReceivedEvent;
            TcpServerEventMessageSent           messageSentEvent;
        private:
                                                TcpServer ( const TcpServer& tcpServer );
            TcpServer&                          operator= ( const TcpServer& tcpServer );
    };

}

#endif // _TCP_SERVER_HPP_