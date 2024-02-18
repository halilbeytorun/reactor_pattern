#include <gtest/gtest.h>

#include "InitiationDispatcher.h"
#include "LoggingAcceptor.h"
#include "EventType.h"

#include "hutiliy.h"

#include <thread>

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


class ProxyClient : public testing::Test
{
protected:
  void SetUp() override
  {

  }
};


TEST_F(ProxyClient, LoggingAcceptorTest)
{
    LoggingAcceptor acceptor{};
    auto retval = acceptor.create_server();

    ASSERT_EQ(retval, 0);

    retval = InitiationDispatcher::getInstance()->register_handler(&acceptor, ACCEPT_EVENT);

    ASSERT_EQ(retval, 0);

    
    std::thread handlerThread(&InitiationDispatcher::handle_events, InitiationDispatcher::getInstance(), 15);
    

    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024] = {0};

    // Create a socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        return;
    }

    // Set the server address and port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return;
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        return;
    }

    sleep(5);

    // Send a message to the server
    std::string message = "Hello, server!";
    Logger(message, " message is sent!");
    
    if (send(clientSocket, message.c_str(), message.length(), 0) < 0) {
        perror("Send failed");
        return;
    }

    // Close the socket
    while(1) {}
    close(clientSocket);



}

// TEST_F(ProxyClient, connect)
// {
//   ASSERT_EQ(1, 1);

//     int clientSocket;
//     struct sockaddr_in serverAddr;
//     char buffer[1024] = {0};

//     // Create a socket
//     if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//         perror("Socket creation failed");
//         return;
//     }

//     // Set the server address and port
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(8080);
//     if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
//         perror("Invalid address/ Address not supported");
//         return;
//     }

//     // Connect to the server
//     if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
//         perror("Connection failed");
//         return;
//     }

//     // Send a message to the server
//     std::string message = "Hello, server!";
//     if (send(clientSocket, message.c_str(), message.length(), 0) < 0) {
//         perror("Send failed");
//         return;
//     }

//     // Close the socket
//     close(clientSocket);


// }
