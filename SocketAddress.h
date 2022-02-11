#pragma once

#include <netinet/in.h>
#include <cstring>

struct SocketAddress
{
    sockaddr_in address;

    int GetSize()
    {
        return sizeof(address);
    }

    SocketAddress(sockaddr_in inAddress) : address(inAddress) {}
    public:
        SocketAddress()
        {
            bzero(&address, sizeof(address));
        }
};