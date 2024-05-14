#include "LoggingHandler.h"
#include "hutiliy.h"

#include <unistd.h>
#include <cstdio>
#include <iostream>

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
        // Read data from the client
    int bytesReceived = read(clientSocket, buffer, sizeof(buffer));
    if (bytesReceived < 0) {
        perror("Read failed");
        return -1;
    }
    buffer[bytesReceived] = '\0';
    Logger("LoggingHandler::handle_event", std::string{buffer}, " message is read");

    return -1;
}
