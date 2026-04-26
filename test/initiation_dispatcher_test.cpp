#include <gtest/gtest.h>
#include "InitiationDispatcher.h"
#include "EventHandler.h"
#include "EventType.h"

#include <memory>
#include <unistd.h>
#include <sys/socket.h>

class MockEventHandler : public EventHandler
{
public:
    MockEventHandler(int handle = -1) : handle_(handle) {}
    
    int HandleEvent(EventType) override
    {
        events_handled++;
        return 0;
    }

    int GetHandle() override
    {
        return handle_;
    }

    int events_handled = 0;

private:
    int handle_;
};

class InitiationDispatcherTest : public testing::Test
{
protected:
    void SetUp() override
    {
        dispatcher = std::make_shared<InitiationDispatcher>();
    }

    std::shared_ptr<InitiationDispatcher> dispatcher;
};

TEST_F(InitiationDispatcherTest, RegisterHandlerSuccess)
{
    auto handler = std::make_shared<MockEventHandler>();
    int result = dispatcher->RegisterHandler(handler, ACCEPT_EVENT);
    EXPECT_EQ(result, 0);
}

TEST_F(InitiationDispatcherTest, RegisterSameHandlerTwiceFailure)
{
    auto handler = std::make_shared<MockEventHandler>();
    int result1 = dispatcher->RegisterHandler(handler, ACCEPT_EVENT);
    int result2 = dispatcher->RegisterHandler(handler, ACCEPT_EVENT);
    
    EXPECT_EQ(result1, 0);
    EXPECT_EQ(result2, -1);  // Should fail on second registration
}

// Test removing a registered handler
TEST_F(InitiationDispatcherTest, RemoveHandlerSuccess)
{
    auto handler = std::make_shared<MockEventHandler>();
    dispatcher->RegisterHandler(handler, ACCEPT_EVENT);
    
    int result = dispatcher->RemoveHandler(handler, ACCEPT_EVENT);
    EXPECT_EQ(result, 0);
}

// Test removing a non-existent handler fails
TEST_F(InitiationDispatcherTest, RemoveNonExistentHandlerFailure)
{
    auto handler = std::make_shared<MockEventHandler>();
    
    int result = dispatcher->RemoveHandler(handler, ACCEPT_EVENT);
    EXPECT_NE(result, 0);
}

// Test multiple handlers can be registered
TEST_F(InitiationDispatcherTest, RegisterMultipleHandlers)
{
    auto handler1 = std::make_shared<MockEventHandler>();
    auto handler2 = std::make_shared<MockEventHandler>();
    auto handler3 = std::make_shared<MockEventHandler>();
    
    EXPECT_EQ(dispatcher->RegisterHandler(handler1, ACCEPT_EVENT), 0);
    EXPECT_EQ(dispatcher->RegisterHandler(handler2, ACCEPT_EVENT), 0);
    EXPECT_EQ(dispatcher->RegisterHandler(handler3, ACCEPT_EVENT), 0);
}

// Test HandleEvents doesn't crash with empty handlers
TEST_F(InitiationDispatcherTest, HandleEventsWithNoHandlers)
{
    EXPECT_NO_THROW(dispatcher->HandleEvents(0));
}

// Test HandleEvents with timeout
TEST_F(InitiationDispatcherTest, HandleEventsWithTimeout)
{
    auto handler = std::make_shared<MockEventHandler>();
    dispatcher->RegisterHandler(handler, ACCEPT_EVENT);
    
    EXPECT_NO_THROW(dispatcher->HandleEvents(5));
}

TEST_F(InitiationDispatcherTest, HandleEventDispatchesToReadyHandler)
{
    int pipe_fds[2];
    ASSERT_EQ(pipe(pipe_fds), 0);

    auto handler = std::make_shared<MockEventHandler>(pipe_fds[0]);
    dispatcher->RegisterHandler(handler, READ_EVENT);
    write(pipe_fds[1], "test", 4);

    dispatcher->HandleEvents(1);

    ASSERT_EQ(handler->events_handled, 1);

    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

