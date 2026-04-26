#ifndef IInitiationDispatcherHeader
#define IInitiationDispatcherHeader

#include "EventHandler.h"
#include "EventType.h"

#include <memory>

class IInitiationDispatcher
{
public:
    virtual ~IInitiationDispatcher() = default;

    virtual int RegisterHandler(std::shared_ptr<EventHandler> handler, EventType event_type) = 0;
    virtual int RemoveHandler(std::shared_ptr<EventHandler> handler, EventType event_type) = 0;
    virtual void HandleEvents(int timeout = -1) = 0;
};

#endif
