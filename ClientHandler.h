#pragma once
#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <WinSock2.h>
#include <string>

class ChatServer; 

class ClientHandler {
public:
    ClientHandler(SOCKET sock, ChatServer* server, const std::string& name);
    ~ClientHandler();

    void process();

private:
    SOCKET clientSocket;
    ChatServer* server; 
    std::string clientName;

    bool receiveMessage(std::string& msg);
    void sendMessage(const std::string& msg);
};

#endif 
