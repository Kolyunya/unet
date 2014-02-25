#include "TcpServer.hpp"

namespace Unet
{

                        TcpServer::TcpServer ( void )
                            :
                                threadAccept(std::bind(TcpServer::routineAccept,this)),
                                threadReceive(std::bind(TcpServer::routineReceive,this)),
                                threadKeepAlive(std::bind(TcpServer::routineKeepAlive,this)),
                                receiveMode(TCP_RECEIVE_MODE_DEFAULT),
                                keepAliveTimeout(1),
                                disconnectTimeout(1)
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
        return this->threadAccept.get_is_initialized();
    }

    int                 TcpServer::getConnectionsLimit ( void ) const
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        return this->serverSocket.getConnectionsLimit();
    }

    void                TcpServer::setConnectionsLimit ( int connectionsLimit )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->serverSocket.setConnectionsLimit(connectionsLimit);
    }

    TcpReceiveMode      TcpServer::getReceiveMode ( void ) const
    {
        return this->receiveMode;
    }

    void                TcpServer::setReceiveMode ( TcpReceiveMode receiveMode )
    {
        this->receiveMode = receiveMode;
    }

    unsigned int        TcpServer::getKeepAliveTimeout ( void ) const
    {
        return this->keepAliveTimeout;
    }

    void                TcpServer::setKeepAliveTimeout ( unsigned int keepAliveTimeout )
    {
        this->keepAliveTimeout = keepAliveTimeout;
    }

    unsigned int        TcpServer::getDisconnectTimeout ( void ) const
    {
        return this->disconnectTimeout;
    }

    void                TcpServer::setDisconnectTimeout ( unsigned int disconnectTimeout )
    {
        this->disconnectTimeout = disconnectTimeout;
    }

    size_t              TcpServer::getMessageSize ( void ) const
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        return this->serverSocket.getMessageSize();
    }

    void                TcpServer::setMessageSize ( size_t messageSize )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->serverSocket.setMessageSize(messageSize);
    }

    std::string         TcpServer::getMessageDelimiter ( void ) const
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        return this->serverSocket.getMessageDelimiter();
    }

    void                TcpServer::setMessageDelimiter ( const std::string& messageDelimiter )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->serverSocket.setMessageDelimiter(messageDelimiter);
    }

    void                TcpServer::sendMessage ( TcpSocketShrPtr tcpSocketShrPtr , const std::string& message )
    {

        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->checkIsLaunched();

        try
        {

            if ( this->receiveMode == TCP_RECEIVE_MODE_BY_DELIMITER )
            {
                tcpSocketShrPtr->sendMessageWithDelimiter(message,MSG_NOSIGNAL);
            }

            else
            {
                tcpSocketShrPtr->sendMessage(message,MSG_NOSIGNAL);
            }

        }

        catch ( Exception<BrokenPipe> )
        {
            this->removeClient(tcpSocketShrPtr);
        }

        this->messageSentEvent.dispatch(tcpSocketShrPtr,message);

    }

    void                TcpServer::startProcedure ( void )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->checkIsNotLaunched();
        this->launchSocket();
        this->launchRoutines();
    }

    void                TcpServer::stopProcedure ( void )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->checkIsLaunched();
        this->stopRoutines();
        this->stopSocket();
    }

    void                TcpServer::launchSocket ( void )
    {
        this->serverSocket.open();
        this->serverSocket.setNonBlocking();
        this->serverSocket.setOption(SO_REUSEADDR,1);
        this->serverSocket.bind(this->addressUniPtr);
        this->serverSocket.listen();
    }

    void                TcpServer::launchRoutines ( void )
    {
        this->threadAccept.start();
        this->threadReceive.start();
        this->threadKeepAlive.start();
    }

    void                TcpServer::stopRoutines ( void )
    {
        this->threadAccept.stop();
        this->threadReceive.stop();
        this->threadKeepAlive.stop();
    }

    void                TcpServer::stopSocket ( void )
    {
        this->serverSocket.close();
    }

    void                TcpServer::removeClient ( TcpSocketShrPtr clientSocketShrPtr )
    {

        TcpSocketShrPtrListItr clientSocketsBegin = this->clientSockets.begin();
        TcpSocketShrPtrListItr clientSocketsEnd = this->clientSockets.end();
        TcpSocketShrPtrListItr clientSocketItr = clientSocketsBegin;
        TcpSocket* clientSocketPtr = clientSocketShrPtr.get();
        TcpSocket& clientSocket = *clientSocketPtr;

        while ( clientSocketItr != clientSocketsEnd )
        {
            TcpSocketShrPtr currentClientSocketShrPtr = *clientSocketItr;
            TcpSocket* currentClientSocketPtr = clientSocketItr->get();
            TcpSocket& currentClientSocket = *currentClientSocketPtr;
            if ( currentClientSocket == clientSocket )
            {
                this->clientSockets.erase(clientSocketItr);
                this->clientDisconnectedEvent.dispatch(currentClientSocketShrPtr);
                return;
            }
            clientSocketItr++;
        }

    }

    void                TcpServer::routineAccept ( TcpServer* tcpServerPtr )
    {
        //  Try to lock "serverMutex"
        std::unique_lock<std::recursive_mutex> serverUniqueLock(tcpServerPtr->serverMutex,std::defer_lock);
        if ( serverUniqueLock.try_lock() )
        {
            TcpSocketShrPtr clientSocketShrPtr = tcpServerPtr->serverSocket._accept();

            clientSocketShrPtr->setNonBlocking();
            clientSocketShrPtr->setUserTimeout(tcpServerPtr->disconnectTimeout*1000);
            clientSocketShrPtr->setKeepAliveEnabled(true);
            clientSocketShrPtr->setKeepAliveParameters(tcpServerPtr->disconnectTimeout,1,1);
            clientSocketShrPtr->setMessageSize(tcpServerPtr->serverSocket.getMessageSize());
            clientSocketShrPtr->setMessageDelimiter(tcpServerPtr->serverSocket.getMessageDelimiter());

            tcpServerPtr->clientSockets.push_back(clientSocketShrPtr);
            tcpServerPtr->clientConnectedEvent.dispatch(clientSocketShrPtr);

        }
    }

    void                TcpServer::routineReceive ( TcpServer* tcpServerPtr )
    {

        std::unique_lock<std::recursive_mutex> serverUniqueLock(tcpServerPtr->serverMutex,std::defer_lock);
        if ( serverUniqueLock.try_lock() )
        {
            for ( TcpSocketShrPtr& clientSocketShrPtr : tcpServerPtr->clientSockets )
            {
                try
                {
                    std::string receivedMessage;
                    switch ( tcpServerPtr->receiveMode )
                    {
                        case TCP_RECEIVE_MODE_DEFAULT: receivedMessage = clientSocketShrPtr->receiveDataBySize(); break;
                        case TCP_RECEIVE_MODE_BY_SIZE: receivedMessage = clientSocketShrPtr->receiveMessageBySize(); break;
                        case TCP_RECEIVE_MODE_BY_DELIMITER: receivedMessage = clientSocketShrPtr->receiveMessageByDelimiter(); break;
                    };
                    tcpServerPtr->messageReceivedEvent.dispatch(clientSocketShrPtr,receivedMessage);
                }
                catch ( ... )
                {
                    continue;
                }
            }
        }

    }

    void                TcpServer::routineKeepAlive ( TcpServer* tcpServerPtr )
    {

        std::unique_lock<std::recursive_mutex> serverUniqueLock(tcpServerPtr->serverMutex,std::defer_lock);

        if ( serverUniqueLock.try_lock() )
        {


            TcpSocketShrPtrListItr clientSocketsBegin = tcpServerPtr->clientSockets.begin();
            TcpSocketShrPtrListItr clientSocketsEnd = tcpServerPtr->clientSockets.end();
            TcpSocketShrPtrListItr clientSocketItr = clientSocketsBegin;

            while ( clientSocketItr != clientSocketsEnd )
            {

                TcpSocketShrPtr clientSocketShrPtr = *clientSocketItr;
                TcpSocket* clientSocketPtr = clientSocketItr->get();

                try
                {
                    //  If client disconnected properly "Exception<PeerDisconnected>" will be emited by "peekMessageBySize"
                    clientSocketPtr->peekDataBySize();
                }
                catch ( Exception<PeerDisconnected> )
                {
                    tcpServerPtr->clientDisconnectedEvent.dispatch(clientSocketShrPtr);
                    clientSocketItr = tcpServerPtr->clientSockets.erase(clientSocketItr);
                    continue;
                }
                catch ( ... )
                {
                    //  E.g. Exception<IncommingDataCouldNotBeRetrieved>
                }

                try
                {
                    //  If client disconnected improperly and TCP keepalive fails to get a response
                    int clientSocketError = clientSocketPtr->getOptionValue<int>(SO_ERROR);
                    if ( clientSocketError == ETIMEDOUT )
                    {
                        tcpServerPtr->clientDisconnectedEvent.dispatch(clientSocketShrPtr);
                        clientSocketItr = tcpServerPtr->clientSockets.erase(clientSocketItr);
                        continue;
                    }
                }
                catch ( ... )
                {

                }

                clientSocketItr++;

            }

            serverUniqueLock.unlock();

            std::this_thread::sleep_for(std::chrono::seconds(tcpServerPtr->keepAliveTimeout));

        }

    }

}
