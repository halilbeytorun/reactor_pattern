#include "LoggingAcceptor.h"
#include "InitiationDispatcher.h"
#include "LoggingHandler.h"
#include "hutiliy.h"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


LoggingAcceptor::LoggingAcceptor()
{

}


int LoggingAcceptor::create_server()
{
    Logger("LoggingAcceptor::create_server function is called");
    struct sockaddr_in  serverAddr;
    // Create a socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        return -1;
    }

    // Bind the socket to an address and port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 15) < 0) {
        perror("Listen failed");
        return -1;
    }

    // Acceptor socket creation successfull.

    return 0;


    // TODO delete here after writing handle_event hook method.
    int clientSocket;
    struct sockaddr_in  clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[1024] = {0};

    // Accept incoming connections
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen)) < 0) {
        perror("Accept failed");
        return -1;
    }

    // Read data from the client
    int bytesReceived = read(clientSocket, buffer, sizeof(buffer));
    if (bytesReceived < 0) {
        perror("Read failed");
        return -1;
    }

    // Print the received message
    std::cout << "Received message from client: " << buffer << std::endl;

    // Close the sockets
    close(clientSocket);
    close(serverSocket);
}



int LoggingAcceptor::handle_event(EventType event_type)
{
    Logger("LoggingAcceptor::handle_event function is called");

    int clientSocket;
    struct sockaddr_in  clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[1024] = {0};


    // Accept incoming connections
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen)) < 0) {
        perror("Accept failed");
        return -1;
    }

    LoggingHandler* handler = new LoggingHandler(clientSocket); // TODO unique_ptr

    InitiationDispatcher::getInstance()->register_handler(handler, ACCEPT_EVENT);

    return 0;
}

int LoggingAcceptor::get_handle()
{
    return serverSocket;
}