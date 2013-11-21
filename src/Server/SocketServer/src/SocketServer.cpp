#include "SocketServer.hpp"

namespace Unet
{

    Unet::AddressShrPtr     SocketServer::getAddress ( void ) const
    {
        return this->addressShrPtr;
    }

    void                    SocketServer::setAddress ( const Unet::AddressShrPtr addressShrPtr )
    {
        this->addressShrPtr = addressShrPtr;
    }
        
}