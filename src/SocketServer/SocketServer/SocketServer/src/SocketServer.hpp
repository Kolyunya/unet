#ifndef _UNET_SOCKET_SERVER_HPP_
#define _UNET_SOCKET_SERVER_HPP_

#include <thread>
#include <mutex>
#include <Unet/Address.hpp>

namespace Unet
{
    class SocketServer
    {
        public:
            explicit                SocketServer ( void ) = default;
            virtual                 ~SocketServer ( void ) noexcept;
            virtual void            launch ( void ) = 0;
            virtual void            stop ( void ) = 0;
            Unet::AddressShrPtr     getAddress ( void ) const;
            void                    setAddress ( const Unet::AddressShrPtr addressShrPtr );
        protected:
            Unet::AddressShrPtr     addressShrPtr;
            std::thread             thread;
            std::recursive_mutex    mutex;
        private:
            explicit                SocketServer ( const SocketServer& socketServer );
            SocketServer&           operator= ( const SocketServer& socketServer );
    };
}

#endif  //  _UNET_SOCKET_SERVER_HPP_