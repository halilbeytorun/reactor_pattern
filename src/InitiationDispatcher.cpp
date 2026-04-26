#include "InitiationDispatcher.h"
#include <poll.h>

int InitiationDispatcher::RegisterHandler(EventHandler* handler, EventType)
{
    std::lock_guard<std::recursive_mutex> lock(rmutex_);
    if(event_handlers_.end() == event_handlers_.find(handler))
    {
        event_handlers_.insert(handler);
        return 0;
    }
    return -1;
}

int InitiationDispatcher::RemoveHandler(EventHandler* handler, EventType)
{
    std::lock_guard<std::recursive_mutex> lock(rmutex_);
    return event_handlers_.erase(handler) == 1 ? 0 : 1;
}

void InitiationDispatcher::HandleEvents(int timeout)
{
    timeout = timeout == -1 ? -1 : timeout * 1000;   // switching to seconds...

    // allowing register/remove handle at the end of each cycle.
    std::lock_guard<std::recursive_mutex> lock(rmutex_);
    
    std::vector<pollfd> fd(event_handlers_.size());
    std::vector<EventHandler*> handlers(event_handlers_.size());
    
    int counter{};
    for(auto iter = event_handlers_.begin(); iter != event_handlers_.end(); iter++)
    {
        handlers[counter] = *iter;
        fd[counter].fd = (*iter)->GetHandle();
        fd[counter].events = POLLIN;
        counter++;
    }
    poll(fd.data(), fd.size(), timeout);

    counter = 0;
    for(auto iter = handlers.begin(); iter != handlers.end(); iter++)
    {
        if(POLLIN == fd[counter].revents)
        {
            (*iter)->HandleEvent(ACCEPT_EVENT);
        }
        counter++;
    }
}
