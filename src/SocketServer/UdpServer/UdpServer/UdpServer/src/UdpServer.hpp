#ifndef _UDP_SERVER_HPP_
#define _UDP_SERVER_HPP_

#include <thread.hpp>
#include <event.hpp>
#include <Unet/UdpSocket.hpp>
#include <Unet/SocketServer.hpp>
#include <Unet/UdpServerEvents.hpp>

namespace Unet
{
    class UdpServer
        :
            public SocketServer
    {
        public:
            explicit                            UdpServer ( void );
            virtual                             ~UdpServer ( void ) noexcept override;
            bool                                getLaunched ( void ) const override;
            void                                sendDatagram ( const Unet::Datagram& datagram );
            UdpServerEventDatagramReceived      datagramReceivedEvent;
            UdpServerEventDatagramSent          datagramSentEvent;
        protected:
            void                                startProcedure ( void ) override;
            void                                stopProcedure ( void ) override;
            void                                launchSocket ( void );
            void                                launchRoutine ( void );
            void                                stopRoutine ( void );
            void                                stopSocket ( void );
            static void                         routine ( UdpServer* UdpServerPtr );
            std::raii_thread_manual             recieveThread;
            UdpSocket                           socket;
        private:
                                                UdpServer ( const UdpServer& UdpServer );
            UdpServer&                          operator= ( const UdpServer& UdpServer );
    };
}

#endif // _UDP_SERVER_HPP_