#ifndef _UDP_SERVER_HPP_
#define _UDP_SERVER_HPP_

#include <thread.hpp>
#include <Util/Event.hpp>
#include <Unet/SocketServer.hpp>
#include <Unet/SocketServerEvent.hpp>
#include <Unet/UdpSocket.hpp>

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
            bool                            getLaunched ( void ) const override;
            void                            launch ( void ) override;
            void                            stop ( void ) override;
            void                            sendDatagram ( const Unet::Datagram& datagram );
        protected:
            void                            launchSocket ( void );
            void                            launchRoutine ( void );
            void                            stopRoutine ( void );
            void                            stopSocket ( void );
            void                            checkIsLaunched ( void ) const;
            void                            checkIsNotLaunched ( void ) const;
            static void                     routine ( UdpServer* UdpServerPtr );
            std::raii_thread_manual         recieveThread;
            UdpSocket                       socket;
        private:
                                            UdpServer ( const UdpServer& UdpServer );
            UdpServer&                      operator= ( const UdpServer& UdpServer );
    };
}

#endif // _UDP_SERVER_HPP_