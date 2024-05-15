#ifndef InitiationDispatcherHeader
#define InitiationDispatcherHeader

#include "EventHandler.h"
#include "EventType.h"

#include <poll.h>

#include <unordered_set>
#include <vector>
#include <mutex>

// TODO: is it the best approach to store EventHandlers as raw pointers?

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
        std::lock_guard<std::recursive_mutex> lock(m_rmutex);
        if(m_handlers.end() == m_handlers.find(handler))
        {
            m_handlers.insert(handler);
            return 0;
        }
        return -1;
    }
    
    /// @brief Remove an EventHandler of a particular EventType 
    int remove_handler(EventHandler* handler, EventType)
    {
        std::lock_guard<std::recursive_mutex> lock(m_rmutex);
        return m_handlers.erase(handler) == 1 ? 0 : 1;
    }

    /// @brief Entry point into the reactive event loop.
    void handle_events(int timeout = -1)
    {
        timeout = timeout * 1000;   // switching to seconds...

        // allowing register/remove handle at the end of each cycle.
        std::lock_guard<std::recursive_mutex> lock(m_rmutex);
        
        std::vector<pollfd> fd(m_handlers.size());
        std::vector<EventHandler*> handlers(m_handlers.size());
        
        int counter{};
        for(auto iter = m_handlers.begin(); iter != m_handlers.end(); iter++)
        {
            handlers[counter] = *iter;
            fd[counter].fd = (*iter)->get_handle();
            fd[counter].events = POLLIN;
            counter++;
        }
        poll(fd.data(), fd.size(), timeout);

        counter = 0;
        for(auto iter = handlers.begin(); iter != handlers.end(); iter++)
        {
            if(POLLIN == fd[counter].revents)
            {
                (*iter)->handle_event(ACCEPT_EVENT);
            }
            counter++;
        }

    }
private:
    std::unordered_set<EventHandler*> m_handlers;
    std::recursive_mutex m_rmutex;
    
    // Singleton pattern.
    InitiationDispatcher() = default;
    InitiationDispatcher(const InitiationDispatcher&) = default;
};

#endif
