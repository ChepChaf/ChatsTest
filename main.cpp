#include "TCPServer.h"

int main()
{
    auto server = TCPServer();
    server.Start(8888);

    while(true)
    {
        server.Run();
    }

    return 0;   
}