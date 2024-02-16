#include "EventHandler.h"



class LoggingAcceptor : public EventHandler
{
public:
    
    /// @brief Constructor is responsible for socket creation, dtor is for destroying the socket.
    LoggingAcceptor();
    int create_server();
    int handle_event(EventType) override
    {
        return -1;
    }
    int get_handle() override
    {
        return -1;
    }
private:
    int serverSocket;
};

