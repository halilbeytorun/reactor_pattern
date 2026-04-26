#ifndef MockInitiationDispatcherHeader
#define MockInitiationDispatcherHeader

#include <gmock/gmock.h>
#include "IInitiationDispatcher.h"
#include "EventHandler.h"
#include "EventType.h"

#include <memory>

class MockInitiationDispatcher : public IInitiationDispatcher
{
public:
    MOCK_METHOD(int, RegisterHandler, (std::shared_ptr<EventHandler> handler, EventType event_type), (override));
    MOCK_METHOD(int, RemoveHandler, (std::shared_ptr<EventHandler> handler, EventType event_type), (override));
    MOCK_METHOD(void, HandleEvents, (int timeout), (override));
};

#endif
