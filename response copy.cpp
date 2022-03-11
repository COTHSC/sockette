#define MAX_EVENTS 5
#define READ_SIZE 10
#include <stdio.h>     // for fprintf()
#include <unistd.h>    // for close(), read()
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <string.h>    // for strncmp
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <fstream>

#define PORT 8081

// char *hello = "HTTP/1.1 200 OK \nDate: Mon, 27 Jul 2009 12:28:53 GMT \nServer: Webserv B**** (He uses arcch btw.) \nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT \nContent-Length: 230 \nContent-Type: text/html; charset=iso-8859-1 \nConnection: Keep-Alive \n\n<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\
// <html>\
// <head>\
//    <title>400 Bad Request</title>\
// </head>\
// <body>\
//    <h1> ARGH </h1>\
//    <p>Your browser sent a request that this server could not understand.</p>\
//    <p>The request line contained invalid characters following the protocol string.</p>\
// </body>\
// </html>"
// ;

char *join(const char* s1, const char* s2)
{
    char* result = (char *)malloc(strlen(s1) + strlen(s2) + 1);

    if (result)
    {
        strcpy(result, s1);
        strcat(result, s2);
    }

    return result;
}

void  write_file_to_socket(int client_fd, const char *path)
{
    printf("\n\nAccepting new connection\n\n");

    std::ifstream infile(path);
    std::string content = "";
    int i;

    
    for(i=0 ; infile.eof()!=true ; i++) // get content of infile
        content += infile.get();
    i--;

    printf("%s\n", content.c_str());
    content.erase(content.end()-1);     // erase last character

    infile.close();
    char *str;
    std::string hi = "HTTP/1.1 200 OK \nDate: Mon, 27 Jul 2009 12:28:53 GMT \nServer: Webserv B**** (He uses arcch btw.) \nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT \nContent-Length: 230 \nContent-Type: text/html; charset=iso-8859-1 \nConnection: Keep-Alive \n\n<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n";

    str = join(hi.c_str(), content.c_str());
    write(client_fd, str, strlen(content.c_str()));                 // output
}

static int
make_socket_non_blocking (int sfd)
{
    int flags, s;

    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1)
    {
        perror ("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1)
    {
        perror ("fcntl");
        return -1;
    }

    return 0;
}

void print_event(struct epoll_event *event_tab)
{
  int i = 0;
  while (i < 2)
  {
    printf("%i\n", event_tab[i].data.fd);
    i++;
  }
}

int main()
{
  int running = 1, event_count, i;
  int ret_read;
  size_t bytes_read;
  struct epoll_event event;
  struct epoll_event events[10];
  int fifo_fd;
  char buffer[100000];

  int server_fd, new_socket;
  long valread;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  // Creating socket file descriptor
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  make_socket_non_blocking(server_fd);


  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  
  // No Idea what it is, a phoque ?
  memset(address.sin_zero, '\0', sizeof address.sin_zero);

  bind(server_fd, (struct sockaddr *)&address, sizeof(address));

  listen(server_fd, 10);

  struct sockaddr in_addr;
  socklen_t in_len;
  int infd;

  in_len = sizeof in_addr;

  int epoll_fd = epoll_create(1); // the argument is useless but must be greater than zero.

  //fill up the struct with the necessary info, namely fd to watch
  
  
  //this adds the server socket fd to the epoll watchlist, allowing us to use epoll to detect incoming connections
  event.data.fd = server_fd;
  event.events = EPOLLIN | EPOLLOUT ;//| EPOLLET;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event);

  //events = calloc (10, sizeof event);

  int d;
  int number_of_fds_with_events;
  while (100)
  {
    number_of_fds_with_events = epoll_wait(epoll_fd, events, 2, -1);
    //print_event(events);
    d = -1;
    while (++d < number_of_fds_with_events)
    {
      if (events[d].data.fd == server_fd)
      {
        printf("\n\nAccepting new connection\n\n");

        infd = accept(server_fd, &in_addr, &in_len);
        event.data.fd = infd;
        event.events = EPOLLIN | EPOLLOUT;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event);
        make_socket_non_blocking(infd);
      }
      else
      {
        if (events[d].events & EPOLLIN)
        {
          ret_read = read(events[d].data.fd, buffer, 100000);
          printf("I have read something of size: %i\n", ret_read);
          if (!ret_read)
          {
            close(events[d].data.fd);
            printf("\n\njust closed a big client\n\n");
          }
          if (ret_read < 0)
          {
            perror("error due to: ");
            perror(strerror(errno));
            perror("\n");
          }
          if (events[d].events & EPOLLOUT)
          {
            std::string str = "test.html";
            write_file_to_socket(events[d].data.fd, str.c_str());
            //write(infd, hello, strlen(hello));
          }
        }
      }
    }
  }
  close(infd);
  close(server_fd);
  return 0;
}