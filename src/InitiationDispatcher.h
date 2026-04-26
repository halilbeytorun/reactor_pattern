#ifndef InitiationDispatcherHeader
#define InitiationDispatcherHeader

#include "EventHandler.h"
#include "EventType.h"


#include <unordered_set>
#include <vector>
#include <mutex>

// TODO: is it the best approach to store EventHandlers as raw pointers?

class InitiationDispatcher
{
public:
    int RegisterHandler(EventHandler* handler, EventType);
    
    int RemoveHandler(EventHandler* handler, EventType);

    void HandleEvents(int timeout = -1);

private:
    std::unordered_set<EventHandler*> event_handlers_;
    std::recursive_mutex rmutex_;
};

#endif
