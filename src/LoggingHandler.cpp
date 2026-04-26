#include "LoggingHandler.h"
#include "hutiliy.h"

#include <unistd.h>
#include <cstdio>
#include <iostream>

LoggingHandler::LoggingHandler(int client_socket) : client_socket_(client_socket), socket_closed_(false)
{

}
LoggingHandler::~LoggingHandler()
{
    if(!socket_closed_)
        close(client_socket_);
}

int LoggingHandler::GetHandle()
{
    return client_socket_;
}


int LoggingHandler::HandleEvent(EventType)
{
    int received_byte_number = read(client_socket_, buffer_, sizeof(buffer_));
    if (received_byte_number < 0) {
        Logger("LoggingHandler::HandleEvent: Read failed");
        return -1;
    }
    else if(0 == received_byte_number) // Connection closed --> Action needed.
    {
        close(client_socket_);
        socket_closed_ = true;
        Logger("LoggingHandler::HandleEvent client socket is closed from server side!");
        return 0;
    }
    buffer_[received_byte_number] = '\0';
    Logger("LoggingHandler::HandleEvent", std::string{buffer_}, " message is read");

    return 0;
}
