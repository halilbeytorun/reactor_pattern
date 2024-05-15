#include "LoggingHandler.h"
#include "hutiliy.h"

#include <unistd.h>
#include <cstdio>
#include <iostream>

LoggingHandler::LoggingHandler(int _clientSocket) : m_clientSocket(_clientSocket), m_socketClosed(false)
{

}
LoggingHandler::~LoggingHandler()
{
    if(!m_socketClosed)
        close(m_clientSocket);
}

int LoggingHandler::get_handle()
{
    return m_clientSocket;
}


int LoggingHandler::handle_event(EventType)
{
        // Read data from the client
    int bytesReceived = read(m_clientSocket, buffer, sizeof(buffer));
    if (bytesReceived < 0) {
        perror("Read failed");
        return -1;
    }
    else if(0 == bytesReceived) // Connection closed --> Action needed.
    {
        close(m_clientSocket);
        m_socketClosed = true;
        Logger("LoggingHandler::handle_event client socket is closed from server side!");
        return 0;
    }
    buffer[bytesReceived] = '\0';
    Logger("LoggingHandler::handle_event", std::string{buffer}, " message is read");

    return 0;
}
