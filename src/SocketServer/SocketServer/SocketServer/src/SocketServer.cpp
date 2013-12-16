#include "SocketServer.hpp"

namespace Unet
{

                            SocketServer::SocketServer ( void )
                                :
                                    keepAliveTimeout(5),
                                    disconnectTimeout(5)
    {

    }

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

    unsigned int            SocketServer::getKeepAliveTimeout ( void ) const
    {
        return this->keepAliveTimeout;
    }

    void                    SocketServer::setKeepAliveTimeout ( unsigned int keepAliveTimeout )
    {
        this->keepAliveTimeout = keepAliveTimeout;
    }

    unsigned int            SocketServer::getDisconnectTimeout ( void ) const
    {
        return this->disconnectTimeout;
    }

    void                    SocketServer::setDisconnectTimeout ( unsigned int disconnectTimeout )
    {
        this->disconnectTimeout = disconnectTimeout;
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