#include "TcpServer.hpp"

namespace Unet
{

                        TcpServer::~TcpServer ( void )
    {
        this->destructRoutine();
        this->destructSocket();
    }

    unsigned char       TcpServer::getMessageDelimiter ( void ) const
    {
        return this->serverSocket.getMessageDelimiter();
    }

    void                TcpServer::setMessageDelimiter ( unsigned char messageDelimiter )
    {
        this->serverSocket.setMessageDelimiter(messageDelimiter);
    }

    unsigned char       TcpServer::getConnectionsLimit ( void ) const
    {
        return this->serverSocket.getConnectionsLimit();
    }

    void                TcpServer::setConnectionsLimit ( unsigned char connectionsLimit )
    {
        this->serverSocket.setConnectionsLimit(connectionsLimit);
    }

    void                TcpServer::launch ( void )
    {
        this->configureSocket();
        this->threadAccept = std::move(std::thread(TcpServer::routineAccept,this));
        this->threadRecieve = std::move(std::thread(TcpServer::routineRecieve,this));
    }

    void                TcpServer::stop ( void )
    {
        this->serverSocket.close();
    }

    void                TcpServer::routineAccept ( TcpServer* /*serverPtr*/ ) noexcept
    {
        while ( true )
        {/*
            try
            {
                TcpSocket acceptedTcpSocket = serverPtr->serverSocket.accept();
                TcpSocket&& acceptedTcpSocketRvalRef = std::move(acceptedTcpSocket);
                serverPtr->clientSockets.push_back(acceptedTcpSocketRvalRef);
            }
            catch ( ... )
            {
                continue;
            }*/
        }
    }

    void                TcpServer::routineRecieve ( TcpServer* /*serverPtr*/ ) noexcept
    {
        while ( true )
        {/*
            for ( TcpSocket&& clientSocket : serverPtr->clientSockets )
            {
                try
                {
                    //  No need to check if the serverSocket has unread data since if it does not then
                    //  an exception will be thrown and caught. This will save some machine time
                    //  when the serverSocket does have unread data.
                    std::string recievedMessage = clientSocket.recieveMessage();
                    Unet::Datagram recievedDatagram(clientMesage,clientSocket.getPeerAddress());
                    serverPtr->DispatchEvent(ServerEvent::MESSAGE_RECIEVED,&recievedDatagram);
                }
                catch ( ... )
                {
                    continue;
                }
            }*/
        }
    }

    void                TcpServer::configureSocket ( void )
    {
        this->serverSocket.open();
        this->serverSocket.setOption(SO_REUSEADDR,1);
        this->serverSocket.bind(*this->addressShrPtr.get());
        this->serverSocket.listen();
    }

    void                TcpServer::destructRoutine ( void ) noexcept
    {
        try
        {
            //  Will throw "std::system_error" is the thread is not in joinable state or if some error occurs.
            //  http://en.cppreference.com/w/cpp/thread/thread/join
            this->thread.join();
        }
        catch ( std::system_error )
        {
            this->dispatchEvent(SocketServerEvent::COULD_NOT_TERMINATE_THREAD,nullptr);
        }
        catch ( ... )
        {
            //  Extra sucurity
        }
    }

    void                TcpServer::destructSocket ( void ) noexcept
    {
        try
        {
            //  Will throw e. g. if the serverSocket is not opened yet.
            this->stop();
        }
        catch ( Unet::Exception )
        {
        }
    }

}