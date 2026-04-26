#include <gtest/gtest.h>
#include "LoggingHandler.h"
#include "EventType.h"

#include <unistd.h>
#include <sys/socket.h>
#include <thread>
#include <chrono>

class LoggingHandlerTest : public testing::Test
{
protected:
    void SetUp() override
    {
        // Create a socket pair for testing
        int sockets[2];
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) != 0)
        {
            throw std::runtime_error("Failed to create socket pair");
        }
        
        server_socket = sockets[0];
        client_socket = sockets[1];
        
        handler = std::make_unique<LoggingHandler>(server_socket);
    }

    void TearDown() override
    {
        if (client_socket >= 0)
            close(client_socket);
        // handler destructor will close server_socket
    }

    int server_socket = -1;
    int client_socket = -1;
    std::unique_ptr<LoggingHandler> handler;
};

// Test GetHandle returns the correct socket
TEST_F(LoggingHandlerTest, GetHandleReturnsCorrectSocket)
{
    EXPECT_EQ(handler->GetHandle(), server_socket);
}

// Test HandleEvent with incoming data
TEST_F(LoggingHandlerTest, HandleEventWithData)
{
    const char* test_message = "Hello Server";
    
    // Send message from client side
    ssize_t sent = send(client_socket, test_message, strlen(test_message), 0);
    EXPECT_EQ(sent, (ssize_t)strlen(test_message));
    
    // Handler should read the data
    int result = handler->HandleEvent(ACCEPT_EVENT);
    EXPECT_EQ(result, 0);
}

// Test HandleEvent when client closes connection
TEST_F(LoggingHandlerTest, HandleEventWithClosedConnection)
{
    // Close client socket
    close(client_socket);
    client_socket = -1;  // Mark as closed so TearDown doesn't close again
    
    // Give server a moment to detect closure
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    // Handler should return 0 (connection closed gracefully)
    int result = handler->HandleEvent(ACCEPT_EVENT);
    EXPECT_EQ(result, 0);
}

// Test multiple HandleEvent calls
TEST_F(LoggingHandlerTest, MultipleHandleEventCalls)
{
    const char* message1 = "Message1";
    const char* message2 = "Message2";
    
    // Send first message
    send(client_socket, message1, strlen(message1), 0);
    EXPECT_EQ(handler->HandleEvent(ACCEPT_EVENT), 0);
    
    // Send second message
    send(client_socket, message2, strlen(message2), 0);
    EXPECT_EQ(handler->HandleEvent(ACCEPT_EVENT), 0);
}

// Test handler destructor closes socket
TEST_F(LoggingHandlerTest, DestructorClosesSocket)
{
    int socket_to_close = server_socket;
    handler.reset();
    
    // Try to use the socket - should fail since it's closed
    char buffer[1];
    int result = recv(socket_to_close, buffer, 1, MSG_DONTWAIT);
    EXPECT_EQ(result, -1);  // Should fail
}
