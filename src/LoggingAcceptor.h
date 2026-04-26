#ifndef LoggingAcceptorHeader
#define LoggingAcceptorHeader

#include "EventHandler.h"

#include <vector>
#include <memory>

class LoggingHandler;
class InitiationDispatcher;

class LoggingAcceptor : public EventHandler
{
public:
    /// @brief Constructor is responsible for socket creation, dtor is for destroying the socket.
    LoggingAcceptor(std::weak_ptr<InitiationDispatcher>);
    int CreateServer();
    int DestroyServer();
    int HandleEvent(EventType) override;
    int GetHandle() override;

private:
    std::weak_ptr<InitiationDispatcher> dispatcher_;
    int server_socket_;
    std::vector<LoggingHandler*> client_handlers_;  // LoggingAcceptor is responsible for them.
};

#endif
