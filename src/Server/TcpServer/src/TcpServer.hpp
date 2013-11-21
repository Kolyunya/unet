#ifndef _TCP_SERVER_HPP_
#define _TCP_SERVER_HPP_

#include <thread>
#include <mutex>
#include <vector>
#include <Util/Event.hpp>
#include <Util/SocketServer.hpp>
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
            void                    setMessageDelimiter ( usigned char messageDelimiter );
            unsigned char           getConnectionsLimit ( void ) const;
            void                    setConnectionsLimit ( unsigned char connectionsLimit ) const;
            void                    launch ( void ) override final;
            void                    stop ( void ) override final;
        protected:
            static void             routineAccept ( TcpServer* tcpServerPtr ) noexcept;
            static void             routineRecieve ( TcpServer* tcpServerPtr ) noexcept;


            
            void                    configureSocket ( void );
            
            
            AddressShrPtr           serverAddressShrPtr;
            UdpSocket               serverSocket;
            std::recursive_mutex    serverMutex;
            TcpSocketsVec           clientSockets;
            std::thread             threadAccept;
            std::thread             threadRecieve;






            
            
            void                    destructRoutine ( void ) noexcept;
            void                    destructSocket ( void ) noexcept;
        private:
                                    UdpServer ( const UdpServer& udpServer );
            UdpServer&              operator= ( const UdpServer& udpServer );
    };
    
}

#endif // _TCP_SERVER_HPP_