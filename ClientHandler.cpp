#include "ClientHandler.h"
#include "ChatServer.h"
#include <iostream>
#include <WS2tcpip.h>

ClientHandler::ClientHandler(SOCKET sock, ChatServer* server)
    : clientSocket(sock), server(server) {
}

ClientHandler::~ClientHandler() {
    closesocket(clientSocket);
}

void ClientHandler::process() {
    std::string msg;
    while (receiveMessage(msg)) {
        std::cout << "Received: " << msg << std::endl;
        
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
