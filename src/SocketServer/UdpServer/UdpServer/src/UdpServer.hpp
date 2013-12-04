#ifndef _UDP_SERVER_HPP_
#define _UDP_SERVER_HPP_

#include <thread.hpp>
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
            explicit                    UdpServer ( void );
            virtual                     ~UdpServer ( void ) noexcept override;
            void                        launch ( void ) override final;
            void                        stop ( void ) override final;
            void                        sendDatagram ( const Unet::Datagram& datagram );
        protected:
            void                        launchSocket ( void );
            void                        launchRoutine ( void );
            void                        stopRoutine ( void );
            void                        stopSocket ( void );
            void                        checkIsLaunched ( void ) const;
            void                        checkIsNotLaunched ( void ) const;
            static void                 routine ( UdpServer* UdpServerPtr );
            std::recursive_mutex        mutex;
            std::raii_thread_manual     thread;
            UdpSocket                   socket;
        private:
                                        UdpServer ( const UdpServer& UdpServer );
            UdpServer&                  operator= ( const UdpServer& UdpServer );
    };
}

#endif // _UDP_SERVER_HPP_