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
    TCPSocket(int inSocket) : socket(inSocket) {}
    public:
    TCPSocket(const TCPSocket& inSocket) : socket(inSocket.socket) {}
    ~TCPSocket()
    {
        close(socket);
    }
    int GetSocket(){ return socket; };
    bool Bind(SocketAddress& sockAddress)
    {
        int c = bind(socket, reinterpret_cast<sockaddr*>(&sockAddress.address), sockAddress.GetSize());

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
};
typedef std::shared_ptr<TCPSocket> TCPSocketPtr;