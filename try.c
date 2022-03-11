
#include <poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main()
{
	int  pret, ret;
	char buf[1100000];
	int fd = STDIN_FILENO;
	struct pollfd fds[1];
	int timeout = -1;
	while (42)
	{
		fds[0].fd = fd;
//		fds[0].events = 0;
		fds[0].events |= POLLIN;

		pret = poll(fds, 1, timeout);
		if (pret == 0)
		{
			printf("timeout\n");
		}
		else
		{
			printf("something can be read\n");
			memset(buf, 0,11);
			ret = read(fd, (void*)buf, 1);
		}
		
	}
	return 0;

}
