#ifndef LoggingHandlerHeader
#define LoggingHandlerHeader

#include "EventHandler.h"


class LoggingHandler : public EventHandler
{
public:
    explicit LoggingHandler(int _clientSocket);
    int handle_event(EventType) override;
    int get_handle() override;

private:
    int clientSocket;
    char buffer[1024];
};



#endif
