#include <stdio.h>     // for fprintf()
#include <unistd.h>    // for close(), read()
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <string.h>    // for strncmp
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#define MAX_EVENTS 5
#define READ_SIZE 30000
#define PORT 8080

int main()
{

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
  int running = 1, event_count, i;
  size_t bytes_read;
  char read_buffer[READ_SIZE + 1];
  struct epoll_event event;
  struct epoll_event events[MAX_EVENTS];
  int epoll_fd = epoll_create1(0);
  int server_fd, new_socket;
  long valread;
  struct sockaddr_in address;
  int addrlen = sizeof(address);


  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("In socket");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  //No Idea what it is, a phoque ?
  memset(address.sin_zero, '\0', sizeof address.sin_zero);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("In bind");
    exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
  if(epoll_fd == -1)
  {
    fprintf(stderr, "Failed to create epoll file descriptor\n");
    return 1;
  }
  printf("\n+++++++ Waiting for new connection ++++++++\n\n");
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
  {
    perror("In accept");
    exit(EXIT_FAILURE);
  }
 
  event.events = EPOLLIN | EPOLLRDHUP | EPOLLHUP;
  event.data.fd = new_socket;
 
  if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event))
  {
    fprintf(stderr, "Failed to add file descriptor to epoll\n");
    close(epoll_fd);
    return 1;
  }
  while (running)
    {
    printf("\nPolling for input...\n");
    event_count = epoll_wait(epoll_fd, events, 3, 300);
    printf("%d ready events\n", event_count);
    for(i = 0; i < event_count; i++)
    {
      printf("Reading file descriptor '%d' -- ", events[i].data.fd);
      bytes_read = read(events[0].data.fd, read_buffer, READ_SIZE);
      if (bytes_read <= 0)
        return 0;
      printf("%zd bytes read.\n", bytes_read);
      read_buffer[bytes_read] = '\0';
      printf("Read '%s'\n", read_buffer);
      memset(read_buffer, '\0', 30000);
      write(new_socket, hello, strlen(hello));
    }
    //close(event.data.fd);
  }
 
  if(close(epoll_fd))
  {
    fprintf(stderr, "Failed to close epoll file descriptor\n");
    return 1;
  }
  return 0;
}