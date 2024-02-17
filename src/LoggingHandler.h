#ifndef LoggingHandlerHeader
#define LoggingHandlerHeader

#include "EventHandler.h"


class LoggingHandler : public EventHandler
{
    explicit LoggingHandler(int clientSocket);
    int handle_event(EventType) override;
    int get_handle() override;

private:
    int clientSocket;
};



#endif
