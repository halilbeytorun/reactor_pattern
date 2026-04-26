#ifndef EventHandlerHeader
#define EventHandlerHeader

#include "EventType.h"


class EventHandler
{
public:
    virtual ~EventHandler() = default;
    virtual int HandleEvent(EventType) = 0;
    virtual int GetHandle() = 0;
};
#endif
