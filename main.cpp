#include "TCPServer.h"

int main()
{
    auto server = TCPServer();
    server.Start("127.0.0.1", 8888);

    while(true)
    {
        server.Run();
    }

    return 0;   
}