#include "TcpServer.hpp"

namespace Unet
{

                        TcpServer::TcpServer ( void )
                            :
                                threadAccept(std::bind(TcpServer::routineAccept,this)),
                                threadReceive(std::bind(TcpServer::routineReceive,this)),
                                threadKeepAlive(std::bind(TcpServer::routineKeepAlive,this)),
                                receiveMode(TCP_RECEIVE_MODE_DEFAULT),
                                keepAliveTimeout(1000),
                                disconnectTimeout(1000)
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

    void                TcpServer::start ( void )
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

        //  TCP server must use "MSG_NOSIGNAL" otherwise it will be terminated after writing to a dead socket
        tcpSocket.sendMessage(message,MSG_NOSIGNAL);

        this->messageSentEvent.dispatch(tcpSocket,message);
    }

    void                TcpServer::sendMessageWithDelimiter ( const TcpSocket& tcpSocket , const std::string& message )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->checkIsLaunched();
        tcpSocket.sendMessageWithDelimiter(message);
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
        this->threadReceive.launch();
        this->threadKeepAlive.launch();
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

    void                TcpServer::routineAccept ( TcpServer* tcpServerPtr )
    {

        //  Try to lock "serverMutex"
        std::unique_lock<std::recursive_mutex> serverUniqueLock(tcpServerPtr->serverMutex,std::defer_lock);
        if ( serverUniqueLock.try_lock() )
        {
            TcpSocket tcpSocket = tcpServerPtr->serverSocket.accept();
            tcpSocket.setNonBlocking();
            tcpSocket.setUserTimeout(tcpServerPtr->disconnectTimeout);
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

        //  http://tldp.org/HOWTO/TCP-Keepalive-HOWTO/usingkeepalive.html
        //  http://tldp.org/HOWTO/TCP-Keepalive-HOWTO/overview.html
        //  http://www.codeproject.com/Articles/37490/Detection-of-Half-Open-Dropped-TCP-IP-Socket-Conne
        //  http://mindprod.com/jgloss/socket.html#DISCONNECT
        //  http://blog.stephencleary.com/2009/05/detection-of-half-open-dropped.html
        //  Ping clients to check for half-opened connections

        std::unique_lock<std::recursive_mutex> serverUniqueLock(tcpServerPtr->serverMutex,std::defer_lock);

        if ( serverUniqueLock.try_lock() )
        {

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
                            clientSocket.sendMessage("@",MSG_NOSIGNAL | MSG_OOB);
                        }
                        catch ( std::exception& exception )
                        {
                            std::cout << exception.what() << std::endl;
                            tcpServerPtr->clientDisconnectedEvent.dispatch(clientSocket);
                            return true;
                        }
                        return false;
                    }
                ),
                tcpServerPtr->clientSockets.end()
            );

            serverUniqueLock.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(tcpServerPtr->keepAliveTimeout));

        }

    }

}