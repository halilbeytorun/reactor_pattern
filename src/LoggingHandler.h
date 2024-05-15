#ifndef LoggingHandlerHeader
#define LoggingHandlerHeader

#include "EventHandler.h"


class LoggingHandler : public EventHandler
{
public:
    explicit LoggingHandler(int _clientSocket);
    ~LoggingHandler();
    int handle_event(EventType) override;
    int get_handle() override;

private:
    int m_clientSocket{};
    char buffer[1024]{};
    bool m_socketClosed{};
};



#endif
