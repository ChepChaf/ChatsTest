#pragma once

#include <iostream>
#include <memory>
#include <unistd.h>

#include "SocketAddress.h"

const int defaultNumberOfConns = 10; // Move to config

class TCPSocket
{
    friend class SocketFactory;
    int socket;

public:
    TCPSocket(int inSocket) : socket(inSocket) {}
    TCPSocket(const TCPSocket &inSocket) : socket(inSocket.socket) {}
    ~TCPSocket()
    {
        std::cout << "Closing connection" << std::endl;
        close(socket);
    }
    int GetSocket() { return socket; };
    bool Bind(SocketAddress &sockAddress)
    {
        int c = bind(socket, reinterpret_cast<sockaddr *>(&sockAddress.address), sockAddress.GetSize());

        if (c != 0)
        {
            std::cout << "Failed to bind socket to address" << std::endl;
            return false;
        }

        return true;
    }
    bool Listen()
    {
        int c = listen(socket, defaultNumberOfConns);

        if (c != 0)
        {
            std::cout << "Failed to listen on socket" << std::endl;
            return false;
        }

        return true;
    }
    std::shared_ptr<TCPSocket> Accept(SocketAddress &clientAddr)
    {
        auto cliLen = clientAddr.GetSize();
        int cSock = accept(socket, reinterpret_cast<sockaddr *>(&clientAddr.address),
                           reinterpret_cast<socklen_t *>(&cliLen));

        if (cSock == -1)
        {
            std::cout << "Error accepting connection from client" << std::endl;
            return nullptr;
        }

        return std::shared_ptr<TCPSocket>(new TCPSocket(cSock));
    }
    int Send(std::string message)
    {
        int n = send(socket, message.c_str(), message.length(), 0);

        if (n < 0)
        {
            std::cout << "Error sending message from socket: " << socket << std::endl;
        }

        return n;
    }
    int Receive(void* buffer, int bufLen)
    {
        int n = recv(socket, static_cast<char*>(buffer), bufLen, 0);

        if (n < 0)
        {
            std::cout << "Error receiving message on socket: " << socket << std::endl;
        }

        return n;
    }

};
typedef std::shared_ptr<TCPSocket> TCPSocketPtr;