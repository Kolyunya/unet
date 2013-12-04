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
        this->launchSocket();
        this->launchThreads();
        this->threadAccept = std::move(std::thread(TcpServer::routineAccept,this));
        this->threadRecieve = std::move(std::thread(TcpServer::routineRecieve,this));
    }

    void                TcpServer::stop ( void )
    {
        std::lock_guard<std::recursive_mutex> lockGuard(this->serverMutex);
        this->stopThreads();
        this->stopSocket();

    }

    void                TcpServer::launchSocket ( void )
    {
        this->serverSocket.open();
        this->serverSocket.setOption(SO_REUSEADDR,1);
        this->serverSocket.bind(*this->addressShrPtr.get());
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
        this->socket.close();
    }

    void                TcpServer::routineAccept ( TcpServer* serverPtr )
    {
        /*  Next line is equivalent to:

            TcpSocket acceptedTcpSocket = serverPtr->serverSocket.accept();
            TcpSocket&& acceptedTcpSocketRvalRef = std::move(acceptedTcpSocket);
            serverPtr->clientSockets.push_back(std::move(acceptedTcpSocketRvalRef));
        */
        serverPtr->clientSockets.push_back(serverPtr->serverSocket.accept());
    }

    void                TcpServer::routineRecieve ( TcpServer* serverPtr )
    {
        for ( TcpSocket& clientSocket : serverPtr->clientSockets )
        {
            try
            {
                //  No need to check if the serverSocket has unread data since if it does not then
                //  an exception will be thrown and caught. This will save some machine time
                //  when the serverSocket does have unread data.
                std::string recievedMessage = clientSocket.recieveMessage();
                Unet::Datagram recievedDatagram(recievedMessage,clientSocket.getPeerAddress());
                serverPtr->dispatchEvent(SocketServerEvent::MESSAGE_RECIEVED,&recievedDatagram);
            }
            catch ( ... )
            {
                continue;
            }
        }
    }

}