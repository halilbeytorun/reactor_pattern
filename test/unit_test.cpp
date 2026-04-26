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
#include <memory>


void serverConnection(int threadNumber)
{
    
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
    std::string message = std::string("Hello, server from thread ") + std::to_string(threadNumber);
    Logger("LoggingAcceptorTest", message, " message is sent!");
    
    if (send(clientSocket, message.c_str(), message.length(), 0) < 0) {
        perror("Send failed");
        return;
    }

    sleep(1);
    close(clientSocket);
    sleep(1);

}

class ProxyClient : public testing::Test
{
protected:
  void SetUp() override
  {

  }
};


TEST_F(ProxyClient, LoggingAcceptorTest)
{
    auto init_dispatcher =  std::make_shared<InitiationDispatcher>();
    LoggingAcceptor acceptor{init_dispatcher};
    auto retval = acceptor.CreateServer();

    ASSERT_EQ(retval, 0);

    retval = init_dispatcher->RegisterHandler(&acceptor, ACCEPT_EVENT);

    ASSERT_EQ(retval, 0);

    std::atomic<bool> reactor_done(false);
    std::thread handler_thread([&](){
        while(!reactor_done)
        {
            init_dispatcher->HandleEvents(15);
        }

    });    
    
    serverConnection(1);

    reactor_done = true;
    handler_thread.join();
    acceptor.DestroyServer();

    retval = init_dispatcher->RemoveHandler(&acceptor, ACCEPT_EVENT);
    ASSERT_EQ(retval, 0);
}



TEST_F(ProxyClient, LoggingAcceptorMultipleClientsTest)
{
    auto init_dispatcher =  std::make_shared<InitiationDispatcher>();
    LoggingAcceptor acceptor{init_dispatcher};
    auto retval = acceptor.CreateServer();

    ASSERT_EQ(retval, 0);

    retval = init_dispatcher->RegisterHandler(&acceptor, ACCEPT_EVENT);

    ASSERT_EQ(retval, 0);

    std::atomic<bool> reactor_done(false);
    std::thread handler_thread([&](){
        while(!reactor_done)
        {
            init_dispatcher->HandleEvents(15);
        }

    });

    const int num_threads = 10;
    std::vector<std::thread> threads;

    // Create threads
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(serverConnection, i);
    }

    // Join threads to the main thread
    for (std::thread& t : threads) {
        t.join();
    }

    reactor_done = true;
    handler_thread.join();
    acceptor.DestroyServer();

}
