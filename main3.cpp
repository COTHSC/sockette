
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MY_SOCK_PATH "/mnt/nfs/homes/jescully/Documents/socket_practice"
#define LISTEN_BACKLOG 50

#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int main(int argc, char *argv[])
{
    int sfd, cfd;
    struct sockaddr_in my_addr;
    struct sockaddr_in peer_addr;

    socklen_t peer_addr_size;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
        handle_error("socket");

    memset(&my_addr, 0, sizeof(my_addr));

    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    my_addr.sin_port = 80;



    if (bind(sfd, (struct sockaddr *)&my_addr,
             sizeof(my_addr)) == -1)
        handle_error("bind");

    if (listen(sfd, LISTEN_BACKLOG) == -1)
           handle_error("listen");

    /* Now we can accept incoming connections one
       at a time using accept(2). */

    peer_addr_size = sizeof(peer_addr);
    char buf[30000] = {0};
    cfd = accept(sfd, (struct sockaddr *) &peer_addr, &peer_addr_size);
    while (recv(cfd, buf, 100, 0) == -1)
        ;
    printf("%s", buf);
      if (cfd == -1)
         handle_error("accept");


    /* Code to deal with incoming connection(s)... */

   // int recv(int sockfd, void *buf, int len, unsigned int flags);
    
    /* When no longer required, the socket pathname, MY_SOCK_PATH
       should be deleted using unlink(2) or remove(3). */
}