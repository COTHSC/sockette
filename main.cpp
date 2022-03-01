#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (void)
{
    int retval;
    struct in_addr addrptr;

    memset(&addrptr, '\0', sizeof(addrptr));
    retval = inet_aton("68.178.157.132", &addrptr);

    struct sockaddr_in dest;

    memset(&dest, '\0', sizeof(dest));
    dest.sin_addr.s_addr = inet_addr("127.0.0.1");

    char *ip;

    ip = inet_ntoa(dest.sin_addr);

    printf("IP Address is: %s\n", ip);
    sockaddr sockaddr_test;

    dest.sin_port = 80;  		     
  //  dest.sin_addr.s_addr = INADDR_ANY;
    printf("this is the ip I geuss %s\n", inet_ntoa(dest.sin_addr));
    printf("this is the port I geuss %d\n", dest.sin_port);
    sockaddr_test.sa_family = AF_INET;
  //  sockaddr_test.sa_data = dest;

    int sockfd = socket(AF_INET, SOCK_STREAM,  IPPROTO_TCP);
    printf("this is sockfd %d\n", sockfd);
    if (bind(sockfd, (struct sockaddr *) &dest, sizeof(dest)) == -1)
        printf("Shit something went wrong\n");
}