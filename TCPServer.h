#pragma once

#include <memory>
#include <string>
#include <vector>

#include <sys/select.h>

#include "SocketAddress.h"
#include "SocketAddressFactory.h"
#include "SocketFactory.h"

#include "TCPSocket.h"

class TCPServer
{
    TCPSocketPtr masterSocket;
    std::vector<TCPSocketPtr> clientSockets;

public:
    bool Start(std::string ip, int port)
    {
        auto socketAddress = SocketAddressFactory::NewAddress(ip, port);
        auto socket = SocketFactory::NewTCPSocket();

        socket->Bind(socketAddress);
        socket->Listen();

        if (socket != nullptr)
        {
            masterSocket = socket;

            return true;
        }

        return false;
    }

    void Run()
    {
        fd_set readfds;

        FD_ZERO(&readfds);
        FD_SET(masterSocket->GetSocket(), &readfds);

        int maxfd = masterSocket->GetSocket();

        for (auto cSocket : clientSockets)
        {
            auto fd = cSocket->GetSocket();
            FD_SET(fd, &readfds);

            if (fd > maxfd)
            {
                maxfd = fd;
            }
        }

        int activity = select(maxfd + 1, &readfds, nullptr, nullptr, nullptr);

        if (activity == -1)
        {
            std::cout << "Error waiting for request on server" << std::endl;
            return;
        }

        std::string message = "Welcome!\n";
        if (FD_ISSET(masterSocket->GetSocket(), &readfds))
        {
            sockaddr_in clientAddr;
            auto cliLen = sizeof(clientAddr);
            int cSock = accept(masterSocket->GetSocket(), reinterpret_cast<sockaddr *>(&clientAddr),
                           reinterpret_cast<socklen_t *>(&cliLen));

            if (cSock == -1)
            {
                std::cout << "Error accepting connection from client" << std::endl;
            }

            send(cSock, message.c_str(), sizeof(message.c_str()), 0);

            clientSockets.push_back(SocketFactory::NewTCPSocket(cSock));
        }

        char buffer[1024];
        for (auto cSocket : clientSockets)
        {   
            auto fd = cSocket->GetSocket();

            if (FD_ISSET(fd, &readfds))
            {
                bzero(&buffer, 1024);

                int n = read(fd, &buffer, 1024);

                if (n > 0)
                    std::cout << "Client: " << buffer << std::endl;
            }
        }
    }
};