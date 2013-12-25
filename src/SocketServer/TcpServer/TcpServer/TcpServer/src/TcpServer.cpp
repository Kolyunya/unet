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

    void                TcpServer::sendMessage ( const TcpSocket& tcpSocket , const std::string& message )
    {

        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->checkIsLaunched();

        try
        {

            if ( this->receiveMode == TCP_RECEIVE_MODE_BY_DELIMITER )
            {
                tcpSocket.sendMessageWithDelimiter(message,MSG_NOSIGNAL);
            }

            else
            {
                tcpSocket.sendMessage(message,MSG_NOSIGNAL);
            }

        }

        catch ( Exception<BrokenPipe> )
        {
            this->removeClient(tcpSocket);
        }

        this->messageSentEvent.dispatch(tcpSocket,message);

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

    void                TcpServer::removeClient ( const TcpSocket& clientSocket )
    {

        TcpSocketsVecItr clientSocketsCitr = this->clientSockets.begin();
        for ( ; clientSocketsCitr != this->clientSockets.end() ; clientSocketsCitr++ )
        {
            if ( (*clientSocketsCitr).getDescriptor() == clientSocket.getDescriptor() )
            {
                this->clientSockets.erase(clientSocketsCitr);
                this->clientDisconnectedEvent.dispatch(clientSocket);
                return;
            }
        }

    }

    void                TcpServer::routineAccept ( TcpServer* tcpServerPtr )
    {
        //  Try to lock "serverMutex"
        std::unique_lock<std::recursive_mutex> serverUniqueLock(tcpServerPtr->serverMutex,std::defer_lock);
        if ( serverUniqueLock.try_lock() )
        {
            TcpSocket tcpSocket = tcpServerPtr->serverSocket.accept();
            tcpSocket.setNonBlocking();
            tcpSocket.setUserTimeout(tcpServerPtr->disconnectTimeout*1000);
            tcpSocket.setKeepAliveEnabled(true);
            tcpSocket.setKeepAliveParameters(tcpServerPtr->disconnectTimeout,1,1);
            tcpSocket.setMessageSize(tcpServerPtr->serverSocket.getMessageSize());
            tcpSocket.setMessageDelimiter(tcpServerPtr->serverSocket.getMessageDelimiter());
            tcpServerPtr->clientConnectedEvent.dispatch(tcpSocket);
            tcpServerPtr->clientSockets.push_back(std::move(tcpSocket));
        }
    }

    void                TcpServer::routineReceive ( TcpServer* tcpServerPtr )
    {
        std::unique_lock<std::recursive_mutex> serverUniqueLock(tcpServerPtr->serverMutex,std::defer_lock);
        if ( serverUniqueLock.try_lock() )
        {
            for ( TcpSocket& clientSocket : tcpServerPtr->clientSockets )
            {
                try
                {
                    //  Receive message
                    std::string receivedMessage;
                    switch ( tcpServerPtr->receiveMode )
                    {
                        case TCP_RECEIVE_MODE_DEFAULT: receivedMessage = clientSocket.receiveDataBySize(); break;
                        case TCP_RECEIVE_MODE_BY_SIZE: receivedMessage = clientSocket.receiveMessageBySize(); break;
                        case TCP_RECEIVE_MODE_BY_DELIMITER: receivedMessage = clientSocket.receiveMessageByDelimiter(); break;
                    };
                    tcpServerPtr->messageReceivedEvent.dispatch(clientSocket,receivedMessage);
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

            //std::cout << tcpServerPtr->clientSockets.size() << std::endl;

            try{

            tcpServerPtr->clientSockets.erase
            (
                std::remove_if
                (
                    tcpServerPtr->clientSockets.begin(),
                    tcpServerPtr->clientSockets.end(),
                    [
                        tcpServerPtr
                    ]
                    (
                        TcpSocket& clientSocket
                    )
                    {

                        try
                        {
                            //  If client disconnected improperly and TCP keepalive fails to get a response
                            int clientSocketError = clientSocket.getOptionValue<int>(SO_ERROR);
                            if ( clientSocketError == ETIMEDOUT )
                            {
                                tcpServerPtr->clientDisconnectedEvent.dispatch(clientSocket);
                                return true;
                            }
                        }
                        catch ( ... )
                        {

                        }

                        try
                        {
                            //  If client disconnected properly "Exception<PeerDisconnected>" will be emited by "peekMessageBySize"
                            clientSocket.peekDataBySize();
                        }
                        catch ( Exception<PeerDisconnected> )
                        {
                            tcpServerPtr->clientDisconnectedEvent.dispatch(clientSocket);
                            return true;
                        }
                        catch ( ... )
                        {
                            //  E.g. Exception<IncommingDataCouldNotBeRetrieved>
                        }

                        return false;

                    }
                ),
                tcpServerPtr->clientSockets.end()
            );

            }catch(std::exception& e ){std::cout << e.what() << std::endl;}

            serverUniqueLock.unlock();

            std::this_thread::sleep_for(std::chrono::seconds(tcpServerPtr->keepAliveTimeout));

        }

    }

}