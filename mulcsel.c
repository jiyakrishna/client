#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	char ch,a[50],b[50];
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int result;
	fd_set readfds, testfds;
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("192.168.1.7");
	server_address.sin_port = (9734);
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);
	FD_ZERO(&readfds);
	FD_SET(server_sockfd, &readfds);
	while(1)
	{
		int fd;
		int nread;
		testfds = readfds;
		printf("server waiting\n");
		result = select(FD_SETSIZE, &testfds, (fd_set *)0,(fd_set *)0, (struct timeval *) 0);
		if(result < 1)
		{
			perror("mulcsel");
			exit(1);
		}

		for(fd = 0; fd < FD_SETSIZE; fd++)
		{
			if(FD_ISSET(fd,&testfds))
			{
				if(fd == server_sockfd)
				{
					client_len = sizeof(client_address);
					client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);
					FD_SET(client_sockfd, &readfds);
					printf("adding client on fd %d\n", client_sockfd);
				}
				else
				{
					ioctl(fd, FIONREAD, &nread);
					if(nread == 0)
					{
						close(fd);
						FD_CLR(fd, &readfds);
						printf("removing client on fd %d\n", fd);
					}
					else
					{

						read(fd,a, 50);
        					printf("Msg from fd %d :%s",fd,a);
						sleep(2);
        					printf("Me: ");
      						fgets(b,50,stdin);
        					write(fd,b , 50);

               
					}
				}
			}
		}
	}
}
