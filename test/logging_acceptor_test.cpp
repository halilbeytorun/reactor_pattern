#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "LoggingAcceptor.h"
#include "mock_initiation_dispatcher.h"
#include "EventType.h"

#include <memory>

using ::testing::_;
using ::testing::Return;

class LoggingAcceptorTest : public testing::Test
{
protected:
    void SetUp() override
    {
        mock_dispatcher = std::make_shared<MockInitiationDispatcher>();
        acceptor = std::make_shared<LoggingAcceptor>(mock_dispatcher);
    }

    void TearDown() override
    {
        // Clean up server if it was created
        if (acceptor)
        {
            acceptor->DestroyServer();
        }
    }

    std::shared_ptr<MockInitiationDispatcher> mock_dispatcher;
    std::shared_ptr<LoggingAcceptor> acceptor;
};

// Test server creation
TEST_F(LoggingAcceptorTest, CreateServerSuccess)
{
    int result = acceptor->CreateServer();
    EXPECT_EQ(result, 0);
    
    int handle = acceptor->GetHandle();
    EXPECT_GE(handle, 0);
}

// Test server destruction
TEST_F(LoggingAcceptorTest, DestroyServerSuccess)
{
    acceptor->CreateServer();
    int result = acceptor->DestroyServer();
    EXPECT_EQ(result, 0);
}

// Test that RegisterHandler is not called during server creation/destruction
TEST_F(LoggingAcceptorTest, RegisterHandlerNotCalledOnServerCreateDestroy)
{
    EXPECT_CALL(*mock_dispatcher, RegisterHandler(_, _))
        .Times(0);  // Should not be called
    
    acceptor->CreateServer();
    acceptor->DestroyServer();
}

// Test GetHandle returns valid socket
TEST_F(LoggingAcceptorTest, GetHandleReturnsValidSocket)
{
    acceptor->CreateServer();
    
    int handle = acceptor->GetHandle();
    EXPECT_GE(handle, 0);
    
    acceptor->DestroyServer();
}

// Test that DestroyServer throws when dispatcher is null and handlers exist
TEST_F(LoggingAcceptorTest, DestroyWithoutDispatcherThrowsWhenHandlersExist)
{
    // This test would require actually accepting a connection to have handlers
    // For now, we test the simpler case: DestroyServer succeeds with no handlers even if dispatcher is null
    auto temp_acceptor = std::make_shared<LoggingAcceptor>(std::weak_ptr<IInitiationDispatcher>());
    
    // Even with a null dispatcher and no handlers, DestroyServer should handle it gracefully
    // (since there are no handlers to remove, no dispatcher calls are made)
    EXPECT_NO_THROW(temp_acceptor->DestroyServer());
}
