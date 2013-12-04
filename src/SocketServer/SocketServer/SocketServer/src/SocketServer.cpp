#include "SocketServer.hpp"

namespace Unet
{

							SocketServer::~SocketServer ( void ) noexcept
	{

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

    void                    SocketServer::checkIsLaunched ( void ) const
    {
        if ( this->getLaunched() == false )
        {
            throw -1;
        }
    }

    void                    SocketServer::checkIsNotLaunched ( void ) const
    {
        if ( this->getLaunched() == true )
        {
            throw -1;
        }
    }

}