#ifndef _UNET_SOCKET_SERVER_HPP_
#define _UNET_SOCKET_SERVER_HPP_

#include <mutex>
#include <thread>
#include <Unet/Address.hpp>

namespace Unet
{
    class SocketServer
    {
        public:
            explicit                        SocketServer ( void ) = default;
            virtual                         ~SocketServer ( void ) noexcept;
            Unet::AddressUniPtr             getAddress ( void ) const;
            void                            setAddress ( Unet::AddressUniPtr addressUniPtr );
            virtual bool                    getLaunched ( void ) const = 0;
            virtual void                    start ( void ) = 0;
            virtual void                    stop ( void ) = 0;
            void                            restart ( void );
        protected:
            void                            checkIsLaunched ( void ) const;
            void                            checkIsNotLaunched ( void ) const;
            mutable std::recursive_mutex    serverMutex;
            Unet::AddressUniPtr             addressUniPtr;
        private:
            explicit                        SocketServer ( const SocketServer& socketServer );
            SocketServer&                   operator= ( const SocketServer& socketServer );
    };
}

#endif  //  _UNET_SOCKET_SERVER_HPP_