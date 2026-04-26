#ifndef LoggingHandlerHeader
#define LoggingHandlerHeader

#include "EventHandler.h"


class LoggingHandler : public EventHandler
{
public:
    explicit LoggingHandler(int client_socket);
    ~LoggingHandler();
    int HandleEvent(EventType) override;
    int GetHandle() override;
private:
    int client_socket_{};
    char buffer_[1024]{};
    bool socket_closed_{};
};



#endif
