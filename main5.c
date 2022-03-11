#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <poll.h>


#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char *hello = "HTTP/1.1 200 OK \nDate: Mon, 27 Jul 2009 12:28:53 GMT \nServer: Webserv B**** (He uses arcch btw.) \nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT \nContent-Length: 230 \nContent-Type: text/html; charset=iso-8859-1 \nConnection: Keep-Alive \n\n<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\
<html>\
<head>\
   <title>400 Bad Request</title>\
</head>\
<body>\
   <h1>Bad Request</h1>\
   <p>Your browser sent a request that this server could not understand.</p>\
   <p>The request line contained invalid characters following the protocol string.</p>\
</body>\
</html>"
;
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    new_socket = 0;
    struct pollfd pfds;
	pfds.events = POLLIN;
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
         if (!new_socket)
        {

            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("In accept");
                exit(EXIT_FAILURE);
            }
            pfds.fd = new_socket;
         }

        pfds.events = 0;
        pfds.events |= POLLIN;
        char buffer[30000] = {0};

 
    }
    return 0;
}
