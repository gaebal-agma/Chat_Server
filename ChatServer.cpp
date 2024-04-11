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
        std::cerr << "Winsock 초기화 실패, 에러 코드: " << wsOk << std::endl;
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
        std::cerr << "소켓 생성 실패, 에러 코드: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(serverPort);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;
    
    if (bind(serverSocket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
        std::cerr << "바인딩 실패, 에러 코드: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "리스닝 실패, 에러 코드: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "클라이언트 연결 실패, 에러 코드: " << WSAGetLastError() << std::endl;
            continue;
        }

        char nameBuf[1024];
        int bytesReceived = recv(clientSocket, nameBuf, 1024, 0);
        if (bytesReceived <= 0) {
            std::cerr << "클라이언트 이름 수신 실패" << std::endl;
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
            std::cerr << "클라이언트 연결 에러 또는 연결 종료, 에러 코드: " << WSAGetLastError() << std::endl;
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
