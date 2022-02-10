#pragma once

#include <string>
#include <cstring>

#include <arpa/inet.h>

#include "SocketAddress.h"

class SocketAddressFactory
{
    public:
    static SocketAddress NewAddress(const std::string& ip, int port)
    {
        sockaddr_in address;
        bzero(&address, sizeof(address));

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(ip.c_str());
        address.sin_port = htons(port);

        return SocketAddress(address);
    }
};