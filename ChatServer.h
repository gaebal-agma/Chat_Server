#pragma once
#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <vector>
#include <mutex>
#include <WinSock2.h>

// 클래스 선언
class ChatServer {
public:
    ChatServer(int port);
    ~ChatServer();

    void start();

private:
    int serverPort;
    SOCKET serverSocket;
    std::vector<SOCKET> clientSockets; // 연결된 클라이언트 소켓을 추적합니다.
    std::mutex clientsMutex; // 클라이언트 목록에 대한 동시 접근을 방지합니다.

    void handleClient(SOCKET clientSocket);
};

#endif // CHATSERVER_H
