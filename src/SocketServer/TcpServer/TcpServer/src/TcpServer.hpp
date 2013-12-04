#ifndef _TCP_SERVER_HPP_
#define _TCP_SERVER_HPP_

#include <vector>
#include <thread.hpp>
#include <Util/Event.hpp>
#include <Unet/SocketServer.hpp>
#include <Unet/SocketServerEvent.hpp>
#include <Unet/TcpSocket.hpp>
#include <Unet/Datagram.hpp>

namespace Unet
{

    typedef std::vector<TcpSocket>              TcpSocketsVec;

    typedef TcpSocketsVec::const_iterator       TcpSocketsVecCitr;

    class TcpServer
        :
            public SocketServer,
            public Util::EventDispatcher
    {
        public:
            explicit                    TcpServer ( void );
            virtual                     ~TcpServer ( void ) noexcept override;
            bool                        getLaunched ( void ) const override;
            unsigned char               getMessageDelimiter ( void ) const;
            void                        setMessageDelimiter ( unsigned char messageDelimiter );
            unsigned char               getConnectionsLimit ( void ) const;
            void                        setConnectionsLimit ( unsigned char connectionsLimit );
            void                        launch ( void ) override;
            void                        stop ( void ) override;
        protected:
            void                        launchSocket ( void );
            void                        launchRoutines ( void );
            void                        stopRoutines ( void );
            void                        stopSocket ( void );
            static void                 routineAccept ( TcpServer* serverPtr );
            static void                 routineRecieve ( TcpServer* serverPtr );
            TcpSocket                   serverSocket;
            TcpSocketsVec               clientSockets;
            std::raii_thread_manual     threadAccept;
            std::raii_thread_manual     threadRecieve;
        private:
                                        TcpServer ( const TcpServer& tcpServer );
            TcpServer&                  operator= ( const TcpServer& tcpServer );
    };

}

#endif // _TCP_SERVER_HPP_