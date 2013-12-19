#include "SocketServer.hpp"

namespace Unet
{

							SocketServer::~SocketServer ( void ) noexcept
	{

	}

    Unet::AddressUniPtr     SocketServer::getAddress ( void ) const
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        //return std::move(this->addressUniPtr);
        return nullptr;
    }

    void                    SocketServer::setAddress ( Unet::AddressUniPtr addressUniPtr )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->addressUniPtr = std::move(addressUniPtr);
    }

    void                    SocketServer::start ( void )
    {
        this->startProcedure();
        this->startedEvent.dispatch();
    }

    void                    SocketServer::stop ( void )
    {
        this->stopProcedure();
        this->stoppedEvent.dispatch();
    }

    void                    SocketServer::restart ( void )
    {
        this->stop();
        this->start();
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