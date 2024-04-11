#pragma once
#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <vector>
#include <mutex>
#include <WinSock2.h>


class ChatServer {
public:
    ChatServer(int port);
    ~ChatServer();

    void start();

private:
    int serverPort;
    SOCKET serverSocket;
    std::vector<SOCKET> clientSockets; 
    std::mutex clientsMutex; // 클라이언트 목록에 대한 동시 접근을 방지

    void handleClient(SOCKET clientSocket);
};

#endif 
