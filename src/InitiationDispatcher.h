#ifndef InitiationDispatcherHeader
#define InitiationDispatcherHeader

#include "EventHandler.h"
#include "EventType.h"

#include <poll.h>

#include <vector>


/// @brief Demultiplext and dispatch EventHandlers in response to client requests.
class InitiationDispatcher
{
public:
    InitiationDispatcher& getInstance()
    {
        static InitiationDispatcher Instance{};
        return Instance;
    }

    /// @brief Register an EventHandler of a particilar EventType (e.g., READ_EVENT, ACCEPT_EVENT etc.)
    int register_handler(EventHandler* handler, EventType)
    {
        handlers.push_back(handler);
    }
    
    /// @brief Remvoe an EventHandler of a particilar EventType 
    int remove_handler(EventHandler* handler, EventType)
    {
        auto iter = handlers.begin();
        while(iter != handlers.end())
        {
            if(*iter == handler)
            {
                handlers.erase(iter);
                return 0;
            }
        }
        return -1;
    }

    /// @brief Entry point into the reactive event loop.
    void handle_events(int timeout = -1)
    {
        // TODO need handles to use select.
        std::vector<pollfd> fd(handlers.size());
        for(int i = 0; i < handlers.size(); i++)
        {
            fd[i].fd = handlers[i]->get_handle();
            fd[i].events = POLLIN;
        }

    }
private:
    std::vector<EventHandler*> handlers;
    
    // Singleton pattern.
    InitiationDispatcher() = default;
    InitiationDispatcher(const InitiationDispatcher&) = default;
};

#endif
