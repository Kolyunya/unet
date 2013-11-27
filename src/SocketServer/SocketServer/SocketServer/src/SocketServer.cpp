#include "SocketServer.hpp"

namespace Unet
{

							SocketServer::~SocketServer ( void ) noexcept
	{

	}

    Unet::AddressShrPtr     SocketServer::getAddress ( void ) const
    {
        return this->addressShrPtr;
    }

    void                    SocketServer::setAddress ( const Unet::AddressShrPtr addressShrPtr )
    {
        this->addressShrPtr = addressShrPtr;
    }

}