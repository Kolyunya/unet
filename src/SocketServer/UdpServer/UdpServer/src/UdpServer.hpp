#ifndef _UDP_SERVER_HPP_
#define _UDP_SERVER_HPP_

#include <thread>
#include <mutex>
#include <Util/Event.hpp>
#include <Unet/SocketServer.hpp>
#include <Unet/SocketServerEvent.hpp>
#include <Unet/UdpSocket.hpp>
#include <iostream>

namespace Unet
{
    class UdpServer
        :
            public SocketServer,
            public Util::EventDispatcher
    {
        public:
            explicit                        UdpServer ( void );
            virtual                         ~UdpServer ( void ) noexcept override;
            void                            launch ( void ) override final;
            void                            stop ( void ) override final;
            void                            sendDatagram ( const Unet::Datagram& datagram );
            bool                            getLaunched ( void ) const;
        protected:
            void                            launchSocket ( void );
            void                            launchRoutine ( void );
            void                            stopRoutine ( void );
            void                            stopSocket ( void );
            void                            checkIsLaunched ( void ) const;
            void                            checkIsNotLaunched ( void ) const;
            static void                     routine ( UdpServer* udpServerPtr );
            UdpSocket                       socket;
            std::thread                     thread;
            bool                            launched;
            mutable std::recursive_mutex    masterMutex;
            mutable std::recursive_mutex    launchedMutex;
        private:
                                            UdpServer ( const UdpServer& udpServer );
            UdpServer&                      operator= ( const UdpServer& udpServer );
    };
}

#endif // _UDP_SERVER_HPP_