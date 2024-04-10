#pragma once
#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <vector>
#include <mutex>
#include <WinSock2.h>

// Ŭ���� ����
class ChatServer {
public:
    ChatServer(int port);
    ~ChatServer();

    void start();

private:
    int serverPort;
    SOCKET serverSocket;
    std::vector<SOCKET> clientSockets; // ����� Ŭ���̾�Ʈ ������ �����մϴ�.
    std::mutex clientsMutex; // Ŭ���̾�Ʈ ��Ͽ� ���� ���� ������ �����մϴ�.

    void handleClient(SOCKET clientSocket);
};

#endif // CHATSERVER_H
