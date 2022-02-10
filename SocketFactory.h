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
    static TCPSocketPtr NewTCPSocket()
    {
        auto tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (tcpSocket == -1)
        {
            std::cout << "Error creating socket" << std::endl;
            return nullptr;
        }

        return TCPSocketPtr(new TCPSocket(tcpSocket));
    }
    static TCPSocketPtr NewTCPSocket(int socket)
    {
        return TCPSocketPtr(new TCPSocket(socket));
    }
};