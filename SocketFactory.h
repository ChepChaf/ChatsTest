#pragma once

#include <iostream>
#include <memory>

#include <sys/types.h>
#include <sys/socket.h>

#include "SocketAddress.h"
#include "TCPSocket.h"

class SocketFactory
{
    public:
    void Bind()
    {
        
    }
    static TCPSocketPtr NewTCPSocket(bool nonBlocking=true)
    {
        auto tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (tcpSocket == -1)
        {
            std::cout << "Error creating socket" << std::endl;
            return nullptr;
        }

        auto sock = new TCPSocket(tcpSocket);

        if (nonBlocking)
            sock->SetNonBlocking(nonBlocking);

        return TCPSocketPtr(sock);
    }
    static TCPSocketPtr NewTCPSocket(int socket, bool nonBlocking=true)
    {
        auto sock = new TCPSocket(socket);

        if (nonBlocking)
            sock->SetNonBlocking(nonBlocking);

        return TCPSocketPtr(sock);
    }
};