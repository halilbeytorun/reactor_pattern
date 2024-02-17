#include "LoggingHandler.h"



LoggingHandler::LoggingHandler(int _clientSocket) : clientSocket(_clientSocket)
{

}

int LoggingHandler::get_handle()
{
    return clientSocket;
}


// TODO implement.
int LoggingHandler::handle_event(EventType)
{
    return -1;
}
