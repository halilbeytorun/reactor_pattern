#ifndef LoggingAcceptorHeader
#define LoggingAcceptorHeader

#include "EventHandler.h"

#include <vector>

class LoggingHandler;

class LoggingAcceptor : public EventHandler
{
public:
    
    /// @brief Constructor is responsible for socket creation, dtor is for destroying the socket.
    LoggingAcceptor();
    int create_server();
    int destroy_server();
    int handle_event(EventType) override;
    int get_handle() override;

private:
    int m_serverSocket;
    std::vector<LoggingHandler*> m_clientHandlers;  // LoggingAcceptor is responsible for them.
};

#endif
