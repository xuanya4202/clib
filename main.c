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
        printf(" socket faile! \n")
        return -1;
    }



}
int start_service(const char *ip, int port)
{
    epoll_fd = epoll_create(100);


}

int main(int argc, char *argv[])
{
    return 0;
}
