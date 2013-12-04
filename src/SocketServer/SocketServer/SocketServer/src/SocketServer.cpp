#include "SocketServer.hpp"

namespace Unet
{

							SocketServer::~SocketServer ( void ) noexcept
	{
        try
        {
            this->stop();
        }
        catch ( ... )
        {
            return;
        }
	}

    Unet::AddressShrPtr     SocketServer::getAddress ( void ) const
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        return this->addressShrPtr;
    }

    void                    SocketServer::setAddress ( const Unet::AddressShrPtr addressShrPtr )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->addressShrPtr = addressShrPtr;
    }

}