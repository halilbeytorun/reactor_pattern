#ifndef EventHandlerHeader
#define EventHandlerHeader

#include "EventType.h"


/// @brief Abstract base class that serves as the targed of the InitiationDispatcher
class EventHandler
{
public:

    virtual ~EventHandler() = default;

    /// @brief Hook method that is called back by the InitiationDispacher to handle events.
    /// @param  
    /// @return 
    virtual int handle_event(EventType) = 0;
    
    /// @brief Hook method that returns the underlying I/0 Handle.
    /// @return Handle
    virtual int get_handle() = 0;


};
#endif
