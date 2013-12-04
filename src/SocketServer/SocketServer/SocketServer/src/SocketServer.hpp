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
            explicit                        SocketServer ( void ) = default;
            virtual                         ~SocketServer ( void ) noexcept;
            Unet::AddressShrPtr             getAddress ( void ) const;
            void                            setAddress ( const Unet::AddressShrPtr addressShrPtr );
            virtual bool                    getLaunched ( void ) const = 0;
            virtual void                    launch ( void ) = 0;
            virtual void                    stop ( void ) = 0;
        protected:
            mutable std::recursive_mutex    serverMutex;
            Unet::AddressShrPtr             addressShrPtr;
        private:
            explicit                        SocketServer ( const SocketServer& socketServer );
            SocketServer&                   operator= ( const SocketServer& socketServer );
    };
}

#endif  //  _UNET_SOCKET_SERVER_HPP_