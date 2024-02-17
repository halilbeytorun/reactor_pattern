#ifndef InitiationDispatcherHeader
#define InitiationDispatcherHeader

#include "EventHandler.h"
#include "EventType.h"

#include <poll.h>

#include <unordered_set>
#include <vector>

/// @brief Demultiplext and dispatch EventHandlers in response to client requests.
class InitiationDispatcher
{
public:
    static InitiationDispatcher* getInstance()
    {
        static InitiationDispatcher Instance{};
        return &Instance;
    }

    /// @brief Register an EventHandler of a particilar EventType (e.g., READ_EVENT, ACCEPT_EVENT etc.)
    int register_handler(EventHandler* handler, EventType)
    {
        if(handlers.end() == handlers.find(handler))
        {
            handlers.insert(handler);
            return 0;
        }
        return -1;
    }
    
    /// @brief Remvoe an EventHandler of a particilar EventType 
    int remove_handler(EventHandler* handler, EventType)
    {
        return handlers.erase(handler);
    }

    /// @brief Entry point into the reactive event loop.
    void handle_events(int timeout = -1)
    {
        // the thread that is handling the events.. can have an exit point somehow later
        while(1)
        {

            // TODO need handles to use select.
            std::vector<pollfd> fd(handlers.size());
            int counter{};

            for(auto iter = handlers.begin(); iter != handlers.end(); iter++)
            {
                fd[counter].fd = (*iter)->get_handle();
                fd[counter].events = POLLIN;
                counter++;
            }
            // poll(fd.data(), )

        }

    }
private:
    std::unordered_set<EventHandler*> handlers;
    
    // Singleton pattern.
    InitiationDispatcher() = default;
    InitiationDispatcher(const InitiationDispatcher&) = default;
};

#endif
