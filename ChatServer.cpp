#include <iostream>
#include <thread>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <mutex>

#include "Chatserver.h"
#include "ClientHandler.h"

#pragma comment(lib, "Ws2_32.lib")

ChatServer::ChatServer(int port) : serverPort(port), serverSocket(INVALID_SOCKET) {
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        std::cerr << "Winsock �ʱ�ȭ ����, ���� �ڵ�: " << wsOk << std::endl;
        exit(EXIT_FAILURE);
    }
}

ChatServer::~ChatServer() {
    closesocket(serverSocket);
    WSACleanup();
}

void ChatServer::start() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "���� ���� ����, ���� �ڵ�: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(serverPort);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;
    
    if (bind(serverSocket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
        std::cerr << "���ε� ����, ���� �ڵ�: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "������ ����, ���� �ڵ�: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Ŭ���̾�Ʈ ���� ����, ���� �ڵ�: " << WSAGetLastError() << std::endl;
            continue;
        }

        char nameBuf[1024];
        int bytesReceived = recv(clientSocket, nameBuf, 1024, 0);
        if (bytesReceived <= 0) {
            std::cerr << "Ŭ���̾�Ʈ �̸� ���� ����" << std::endl;
            closesocket(clientSocket);
            continue;
        }

   
        std::string clientName(nameBuf, bytesReceived);
        ClientHandler* handler = new ClientHandler(clientSocket, this, clientName);
        std::thread clientThread([handler]() {
            handler->process();
            delete handler;
            });
        clientThread.detach();
    }

}

void ChatServer::handleClient(SOCKET clientSocket) {
    char buf[4096];

    while (true) {
        ZeroMemory(buf, 4096);

        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            std::cerr << "Ŭ���̾�Ʈ ���� ���� �Ǵ� ���� ����, ���� �ڵ�: " << WSAGetLastError() << std::endl;
            break;
        }


        for (auto const& c : clientSockets) {
            if (c != clientSocket) { 
                send(c, buf, bytesReceived, 0);
            }
        }
    }

    closesocket(clientSocket);
}
