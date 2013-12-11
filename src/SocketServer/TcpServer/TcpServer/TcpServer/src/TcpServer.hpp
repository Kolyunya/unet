#ifndef _TCP_SERVER_HPP_
#define _TCP_SERVER_HPP_

#include <vector>
#include <algorithm>
#include <thread.hpp>
#include <event.hpp>
#include <Unet/SocketServer.hpp>
#include <Unet/TcpSocket.hpp>
#include <Unet/Datagram.hpp>
#include <Unet/TcpServerEvents.hpp>

namespace Unet
{

    typedef std::vector<TcpSocket>              TcpSocketsVec;

    typedef TcpSocketsVec::const_iterator       TcpSocketsVecCitr;

    class TcpServer
        :
            public SocketServer
    {
        public:
            explicit                            TcpServer ( void );
            virtual                             ~TcpServer ( void ) noexcept override;
            bool                                getLaunched ( void ) const override;
            unsigned char                       getMessageDelimiter ( void ) const;
            void                                setMessageDelimiter ( unsigned char messageDelimiter );
            unsigned char                       getConnectionsLimit ( void ) const;
            void                                setConnectionsLimit ( unsigned char connectionsLimit );
            void                                launch ( void ) override;
            void                                stop ( void ) override;
            void                                sendMessage ( const TcpSocket& tcpSocket , const std::string& message );
        protected:
            void                                launchSocket ( void );
            void                                launchRoutines ( void );
            void                                stopRoutines ( void );
            void                                stopSocket ( void );
            static void                         routineAccept ( TcpServer* tcpServerPtr );
            static void                         routineRecieve ( TcpServer* tcpServerPtr );
            TcpSocket                           serverSocket;
            TcpSocketsVec                       clientSockets;
            std::raii_thread_manual             threadAccept;
            std::raii_thread_manual             threadRecieve;
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