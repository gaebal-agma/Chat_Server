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
    std::mutex clientsMutex; // Ŭ���̾�Ʈ ��Ͽ� ���� ���� ������ ����

    void handleClient(SOCKET clientSocket);
};

#endif 
