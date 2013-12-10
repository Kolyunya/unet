#include "TcpServer.hpp"

namespace Unet
{

                        TcpServer::TcpServer ( void )
                            :
                                threadAccept(std::bind(TcpServer::routineAccept,this)),
                                threadRecieve(std::bind(TcpServer::routineRecieve,this))
    {

    }

                        TcpServer::~TcpServer ( void ) noexcept
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

    bool                TcpServer::getLaunched ( void ) const
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        return this->threadAccept.isActive();
    }

    unsigned char       TcpServer::getMessageDelimiter ( void ) const
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        return this->serverSocket.getMessageDelimiter();
    }

    void                TcpServer::setMessageDelimiter ( unsigned char messageDelimiter )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->serverSocket.setMessageDelimiter(messageDelimiter);
    }

    unsigned char       TcpServer::getConnectionsLimit ( void ) const
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        return this->serverSocket.getConnectionsLimit();
    }

    void                TcpServer::setConnectionsLimit ( unsigned char connectionsLimit )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->serverSocket.setConnectionsLimit(connectionsLimit);
    }

    void                TcpServer::launch ( void )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->checkIsNotLaunched();
        this->launchSocket();
        this->launchRoutines();
    }

    void                TcpServer::stop ( void )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->checkIsLaunched();
        this->stopRoutines();
        this->stopSocket();
    }

    void                TcpServer::sendMessage ( const TcpSocket& tcpSocket , const std::string& message )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->checkIsLaunched();
        tcpSocket.sendMessage(message);
        this->messageSentEvent.dispatch(tcpSocket,message);
    }

    void                TcpServer::launchSocket ( void )
    {
        this->serverSocket.open();
        this->serverSocket.setNonBlocking();
        this->serverSocket.setOption(SO_REUSEADDR,1);
        this->serverSocket.bind(*this->addressUniPtr.get());
        this->serverSocket.listen();
    }

    void                TcpServer::launchRoutines ( void )
    {
        this->threadAccept.launch();
        this->threadRecieve.launch();
    }

    void                TcpServer::stopRoutines ( void )
    {
        this->threadAccept.stop();
        this->threadRecieve.stop();
    }

    void                TcpServer::stopSocket ( void )
    {
        this->serverSocket.close();
    }

    void                TcpServer::routineAccept ( TcpServer* tcpServerPtr )
    {
        //  Try to lock "serverMutex"
        std::unique_lock<std::recursive_mutex> serverUniqueLock(tcpServerPtr->serverMutex,std::defer_lock);
        if ( serverUniqueLock.try_lock() )
        {
            TcpSocket tcpSocket = tcpServerPtr->serverSocket.accept();
            tcpSocket.setNonBlocking();
            tcpServerPtr->clientConnectedEvent.dispatch(tcpSocket);
            tcpServerPtr->clientSockets.push_back(std::move(tcpSocket));
        }
    }

    void                TcpServer::routineRecieve ( TcpServer* tcpServerPtr )
    {
        std::unique_lock<std::recursive_mutex> serverUniqueLock(tcpServerPtr->serverMutex,std::defer_lock);
        if ( serverUniqueLock.try_lock() )
        {
            tcpServerPtr->clientSockets.erase
            (
                std::remove_if
                (
                    tcpServerPtr->clientSockets.begin(),
                    tcpServerPtr->clientSockets.end(),
                    [tcpServerPtr](TcpSocket& clientSocket)
                    {
                        try
                        {
                            std::string recievedMessage = clientSocket.recieveMessageByDelimiter(tcpServerPtr->serverSocket.getMessageDelimiter());
                            if ( recievedMessage == "" )
                            {
                                tcpServerPtr->clientDisconnectedEvent.dispatch(clientSocket);
                                return true;
                            }
                            tcpServerPtr->messageReceivedEvent.dispatch(clientSocket,recievedMessage);
                            return false;
                        }
                        catch ( ... )
                        {
                            return false;
                        }
                    }
                ),
                tcpServerPtr->clientSockets.end()
            );
        }
    }

}