#pragma once
#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <WinSock2.h>
#include <string>

class ChatServer; 

class ClientHandler {
public:
    ClientHandler(SOCKET sock, ChatServer* server);
    ~ClientHandler();

    void process();

private:
    SOCKET clientSocket;
    ChatServer* server; 

    bool receiveMessage(std::string& msg);
    void sendMessage(const std::string& msg);
};

#endif 
