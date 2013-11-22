#ifndef _TCP_SERVER_HPP_
#define _TCP_SERVER_HPP_

#include <thread>
#include <mutex>
#include <vector>
#include <Util/Event.hpp>
#include <Unet/SocketServer.hpp>
#include <Unet/SocketServerEvent.hpp>
#include <Unet/TcpSocket.hpp>

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
            virtual                 ~TcpServer ( void ) noexcept;
            unsigned char           getMessageDelimiter ( void ) const;
            void                    setMessageDelimiter ( unsigned char messageDelimiter );
            unsigned char           getConnectionsLimit ( void ) const;
            void                    setConnectionsLimit ( unsigned char connectionsLimit );
            void                    launch ( void ) override final;
            void                    stop ( void ) override final;
        protected:
            static void             routineAccept ( TcpServer* serverPtr ) noexcept;
            static void             routineRecieve ( TcpServer* serverPtr ) noexcept;



            void                    configureSocket ( void );


            AddressShrPtr           serverAddressShrPtr;
            TcpSocket               serverSocket;
            std::recursive_mutex    serverMutex;
            TcpSocketsVec           clientSockets;
            std::thread             threadAccept;
            std::thread             threadRecieve;








            void                    destructRoutine ( void ) noexcept;
            void                    destructSocket ( void ) noexcept;
        private:
                                    TcpServer ( const TcpServer& tcpServer );
            TcpServer&              operator= ( const TcpServer& tcpServer );
    };

}

#endif // _TCP_SERVER_HPP_