#pragma once

#include <netinet/in.h>

struct SocketAddress
{
    sockaddr_in address;

    int GetSize()
    {
        return sizeof(address);
    }

    SocketAddress(sockaddr_in inAddress) : address(inAddress) {}
};