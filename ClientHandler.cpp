#include "ClientHandler.h"
#include "ChatServer.h"
#include <iostream>
#include <WS2tcpip.h>

ClientHandler::ClientHandler(SOCKET sock, ChatServer* server, const std::string& name)
    : clientSocket(sock), server(server), clientName(name) {}

ClientHandler::~ClientHandler() {
    closesocket(clientSocket);
}

void ClientHandler::process() {
    std::string msg;
    while (receiveMessage(msg)) {
        std::string formattedMsg = clientName + ": " + msg; // 클라이언트 이름을 포함하여 메시지 형식화
        std::cout << "Received: " << formattedMsg << std::endl;
    }
}

bool ClientHandler::receiveMessage(std::string& msg) {
    char buf[4096];
    ZeroMemory(buf, 4096);
    int bytesReceived = recv(clientSocket, buf, 4096, 0);
    if (bytesReceived > 0) {
        msg = std::string(buf, 0, bytesReceived);
        return true;
    }
    return false;
}

void ClientHandler::sendMessage(const std::string& msg) {
    send(clientSocket, msg.c_str(), msg.size() + 1, 0); 
}
