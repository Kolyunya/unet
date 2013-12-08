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

    void                TcpServer::sendDatagram ( Unet::Datagram& datagram )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->checkIsLaunched();
//        this->serverSocket.sendDatagram(datagram);
        //this->serverSocket.sendMessage(datagram.message,);
        this->dispatchEvent(SocketServerEvent::MESSAGE_SENT,&datagram);
    }

    void                TcpServer::launchSocket ( void )
    {
        this->serverSocket.open();
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
            //std::coutmt << "routineAccept\n";
            if ( tcpServerPtr->serverSocket.hasUnreadData() )
            {
                /*
                    Next line is equivalent to:
                    TcpSocket acceptedTcpSocket = tcpServerPtr->serverSocket.accept();
                    TcpSocket&& acceptedTcpSocketRvalRef = std::move(acceptedTcpSocket);
                    tcpServerPtr->clientSockets.push_back(std::move(acceptedTcpSocketRvalRef));
                */
                tcpServerPtr->clientSockets.push_back(tcpServerPtr->serverSocket.accept());
                std::coutmt << "CONNECTED\n";
            }
        }
    }

    void                TcpServer::routineRecieve ( TcpServer* tcpServerPtr )
    {
        //  Try to lock "serverMutex"
        std::unique_lock<std::recursive_mutex> serverUniqueLock(tcpServerPtr->serverMutex,std::defer_lock);
        if ( serverUniqueLock.try_lock() )
        {
            //std::coutmt << "routineRecieve\n";
            for ( TcpSocket& clientSocket : tcpServerPtr->clientSockets )
            {
                if ( clientSocket.hasUnreadData() )
                {
                    std::coutmt << "unread data\n";
                    std::string recievedMessage = clientSocket.recieveMessageByDelimiter(tcpServerPtr->serverSocket.getMessageDelimiter());
                    std::coutmt << "!!!!" << recievedMessage << std::endl;
                    Unet::Datagram recievedDatagram(recievedMessage,clientSocket.getPeerAddress());
                    tcpServerPtr->dispatchEvent(SocketServerEvent::MESSAGE_RECIEVED,&recievedDatagram);
                    std::coutmt << "333" << std::endl;
                }
            }
        }
    }

}