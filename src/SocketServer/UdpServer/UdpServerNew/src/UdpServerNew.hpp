#ifndef _UDP_SERVER_NEW_HPP_
#define _UDP_SERVER_NEW_HPP_

#include <thread>
#include <mutex>
#include <Util/Event.hpp>
#include <Unet/SocketServer.hpp>
#include <Unet/SocketServerEvent.hpp>
#include <Unet/UdpSocket.hpp>
#include <thread.hpp>
#include <iostream>

namespace Unet
{
    class UdpServerNew
        :
            public SocketServer,
            public Util::EventDispatcher
    {
        public:
            explicit                    UdpServerNew ( void );
            virtual                     ~UdpServerNew ( void ) noexcept override;
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
            static void                 routine ( UdpServerNew* udpServerNewPtr );
            std::recursive_mutex        mutex;
            std::raii_thread_manual     thread;
            UdpSocket                   socket;
        private:
                                        UdpServerNew ( const UdpServerNew& UdpServerNew );
            UdpServerNew&               operator= ( const UdpServerNew& UdpServerNew );
    };
}

#endif // _UDP_SERVER_NEW_HPP_