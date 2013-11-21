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
        return this->tcpSocket.getMessageDelimiter();
    }
    
    void                TcpServer::setMessageDelimiter ( usigned char messageDelimiter )
    {
        this->tcpSocket.setMessageDelimiter(messageDelimiter);
    }
    
    unsigned char       TcpServer::getConnectionsLimit ( void ) const
    {
        this->tcpSocket.getConnectionsLimit();
    }
    
    void                TcpServer::setConnectionsLimit ( unsigned char connectionsLimit ) const
    {
        this->tcpSocket.setConnectionsLimit(connectionsLimit);
    }
    
    void                TcpServer::launch ( void )
    {
        this->configureSocket();
        this->threadAccept = std::move(std::thread(TcpServer::routineAccept,this));
        this->threadRecieve = std::move(std::thread(TcpServer::routineRecieve,this));
    }

    void                TcpServer::stop ( void )
    {
        this->tcpSocket.close();
    }

    void                TcpServer::routineAccept ( TcpServer* tcpServerPtr ) noexcept
    {
        while ( true )
        {
            try
            {
                TcpSocket acceptedTcpSocket = this->tcpSocket.accept();
                TcpSocket&& acceptedTcpSocketRvalRef = std::move(acceptedTcpSocket);
                this->clientSockets.push_back(acceptedTcpSocketRvalRef);
            }
            catch ( ... )
            {
                continue;
            }
        }
    }
    
    void                TcpServer::routineRecieve ( TcpServer* tcpServerPtr ) noexcept
    {
        while ( true )
        {
            for ( TcpSocket&& clientSocket : this->clientSockets )
            {
                try
                {
                    //  No need to check if the tcpSocket has unread data since if it does not then
                    //  an exception will be thrown and caught. This will save some machine time
                    //  when the tcpSocket does have unread data.
                    std::string recievedMessage = clientSocket.recieveMessage();
                    Unet::Datagram recievedDatagram(clientMesage,clientSocket.getPeerAddress());
                    this->DispatchEvent(ServerEvent::MESSAGE_RECIEVED,&recievedDatagram);
                }
                catch ( ... )
                {
                    continue;
                }
            }
        }
    }
    
    void                TcpServer::configureSocket ( void )
    {
        this->tcpSocket.open();
        this->tcpSocket.setOption(SO_REUSEADDR,1);
        this->tcpSocket.bind(*this->addressShrPtr.get());
        this->tcpSocket.listen();
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
            //  Will throw e. g. if the tcpSocket is not opened yet.
            this->stop();
        }
        catch ( Unet::Exception )
        {
        }
    }

}