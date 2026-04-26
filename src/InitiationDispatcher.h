#ifndef InitiationDispatcherHeader
#define InitiationDispatcherHeader

#include "IInitiationDispatcher.h"
#include "EventHandler.h"
#include "EventType.h"


#include <unordered_set>
#include <vector>
#include <mutex>
#include <memory>

class InitiationDispatcher : public IInitiationDispatcher
{
public:
    int RegisterHandler(std::shared_ptr<EventHandler> handler, EventType) override;
    
    int RemoveHandler(std::shared_ptr<EventHandler> handler, EventType) override;

    void HandleEvents(int timeout = -1) override;

private:
    struct SharedPtrHash {
        template<typename T>
        size_t operator()(const std::shared_ptr<T>& ptr) const{
            return std::hash<T*>()(ptr.get());
        }
    };

    struct SharedPtrEqual{
        template<typename T>
        bool operator()(const std::shared_ptr<T>& l, const std::shared_ptr<T>& r) const
        {
            return l.get() == r.get();
        }
    };

    std::unordered_set<std::shared_ptr<EventHandler>, SharedPtrHash, SharedPtrEqual> event_handlers_;
    std::recursive_mutex rmutex_;
};

#endif
