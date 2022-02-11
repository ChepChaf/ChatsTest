#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <queue>

#include <sys/select.h>

#include "SocketAddress.h"
#include "SocketAddressFactory.h"
#include "SocketFactory.h"

#include "TCPSocket.h"

class TCPServer
{
    TCPSocketPtr masterSocket;
    std::vector<TCPSocketPtr> clientSockets;
    std::queue<std::string> clientMessages;

public:
    bool Start(int port)
    {
        auto socketAddress = SocketAddressFactory::NewLocalAddressOnPort(port);
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

    void RemoveSocket(int socket)
    {
        auto it = std::remove_if(
            clientSockets.begin(), clientSockets.end(), [=](auto s1){return s1->GetSocket() == socket;});

        clientSockets.erase(it, clientSockets.end());
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

        std::string message = "Welcome!\r\n";
        if (FD_ISSET(masterSocket->GetSocket(), &readfds))
        {
            auto clientAddr = SocketAddress();
            auto cSock = masterSocket->Accept(clientAddr);

            cSock->Send(message);
            std::cout << "Client connected." << std::endl;

            clientSockets.push_back(cSock);
        }

        char buffer[1024];
        for (auto cSocket : clientSockets)
        {
            if (FD_ISSET(cSocket->GetSocket(), &readfds))
            {
                bzero(buffer, sizeof(buffer));

                int n = cSocket->Receive(buffer, 1024);

                if (n <= 0)
                {
                    RemoveSocket(cSocket->GetSocket());
                    std::cout << "Client disconnected." << std::endl;

                    continue;
                }

                clientMessages.push(buffer);
            }
        }

        while(clientMessages.size() > 0)
        {
            auto message = clientMessages.front();
            clientMessages.pop();

            for (auto socket : clientSockets)
            {
                socket->Send("Client: " + message);
            }
        }
    }
};