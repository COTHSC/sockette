#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

int main(int argc, char *argv[])
{
    int sfd, cfd;
    struct sockaddr_in my_addr;
    struct sockaddr_in peer_addr;

    socklen_t peer_addr_size;

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&my_addr, 0, sizeof(my_addr));

 

    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    my_addr.sin_port = 80;
    connect(sfd, (struct sockaddr *) &my_addr, sizeof(my_addr));
    std::string tbsent;
    while (42)
    {
        std::cin >> tbsent;
        send(sfd, tbsent.c_str(), tbsent.size(), 0);
    }
}