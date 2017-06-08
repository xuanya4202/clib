#include <stdio.h>
#include <stdlib.h>

int epoll_fd;

int init_socket(unsigned int port)
{
    int socketfd;
    struct sockaddr_in server_addr;
    // SOCK_STREAM -> tcp
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == socketfd)
    {
        printf(" socket error! \n")
        return -1;
    }

    bzero((void *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htonl(port);
    bzero((void*)&(server_addr.sin_zero), 8);

    if (bind(socketfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) < 0)
    {
        printf(" bind error! \n");
        return -1;
    }
    //等待队列为5
    if(listen(socketfd, 5))
    {
        printf(" listen error! \n");
    }
    
    return socketfd;
}
int start_service(const char *ip, int port)
{
    int ret = 0; 
    epoll_fd = epoll_create(100);
    
    ret = init_socket(port)
    if (0 >= ret)
    {
        printf(" init socket faile! \n");
        return -1;
    }
}

int main(int argc, char *argv[])
{
    return 0;
}
