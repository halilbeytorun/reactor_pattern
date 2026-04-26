#include "LoggingAcceptor.h"
#include "IInitiationDispatcher.h"
#include "LoggingHandler.h"
#include "hutiliy.h"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


LoggingAcceptor::LoggingAcceptor(std::weak_ptr<IInitiationDispatcher> dispatcher) : dispatcher_{std::move(dispatcher)}  
{

}


int LoggingAcceptor::CreateServer()
{
    Logger("LoggingAcceptor::CreateServer function is called");
    struct sockaddr_in  serverAddr;
    // Create a socket
    if ((server_socket_ = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        Logger("LoggingAcceptor::CreateServer Socket creation failed");
        return -1;
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        Logger("LoggingAcceptor::CreateServer: Setsockopt failed");
        return -1;
    }

    // Bind the socket to an address and port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    if (bind(server_socket_, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        Logger("LoggingAcceptor::CreateServer: Bind failed");
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_socket_, 15) < 0) {
        Logger("LoggingAcceptor::CreateServer: Listen failed");
        return -1;
    }

    // Acceptor socket creation successfull.

    return 0;
}

int LoggingAcceptor::DestroyServer()
{
    for(auto client_handle : client_handlers_)
    {
        auto dispatcher_shared = dispatcher_.lock();
        if(!dispatcher_shared)
        {
            throw std::runtime_error{"LoggingAcceptor::DestroyServer, the dispacher is deleted"};
        }
        dispatcher_shared->RemoveHandler(client_handle, ACCEPT_EVENT);
    }
    client_handlers_.resize(0);
    client_handlers_.shrink_to_fit();
    close(server_socket_);
    return 0;
}

int LoggingAcceptor::HandleEvent(EventType event_type)
{
    Logger("LoggingAcceptor::HandleEvent function is called");

    int clientSocket;
    struct sockaddr_in  clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[1024] = {0};


    // Accept incoming connections
    if ((clientSocket = accept(server_socket_, (struct sockaddr *)&clientAddr, &clientAddrLen)) < 0) {
        Logger("Accept failed");
        return -1;
    }

    auto handler = std::make_shared<LoggingHandler>(clientSocket);
    
    client_handlers_.push_back(handler);    // TODO: Think about lifetime and ownership!
    auto dispatcher_shared = dispatcher_.lock();
    if(!dispatcher_shared)
    {
        throw std::runtime_error{"LoggingAcceptor::DestroyServer, the dispacher is deleted"};
    }    
    dispatcher_shared->RegisterHandler(handler, ACCEPT_EVENT);

    return 0;
}

int LoggingAcceptor::GetHandle()
{
    return server_socket_;
}