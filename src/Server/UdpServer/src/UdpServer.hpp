#ifndef _UDP_SERVER_HPP_
#define _UDP_SERVER_HPP_

#include <thread>
#include <mutex>
#include <Util/Event.hpp>
#include <Util/SocketServer.hpp>
#include <Unet/SocketServerEvent.hpp>
#include <Unet/UdpSocket.hpp>

namespace Unet
{
    class UdpServer
        :
            public SocketServer,
            public Util::EventDispatcher,
    {
        public:
            virtual                 ~UdpServer ( void ) override noexcept;
            void                    sendDatagram ( const Unet::Datagram& datagram );
            void                    launch ( void ) override final;
            void                    stop ( void ) override final;
        protected:
            static void             routine ( UdpServer* udpServerPtr ) noexcept;
            void                    configureSocket ( void );
            void                    launchRoutine ( void );
            void                    destructRoutine ( void ) noexcept;
            void                    destructSocket ( void ) noexcept;
            void                    recieveDatagram ( void ) noexcept;
            AddressShrPtr           addressShrPtr;
            UdpSocket               socket;
            std::thread             thread;
            std::recursive_mutex    mutex;
        private:
                                    UdpServer ( const UdpServer& udpServer );
            UdpServer&              operator= ( const UdpServer& udpServer );
    };
}

#endif // _UDP_SERVER_HPP_