#include <stdio.h>
#include <stdlib.h>

int epoll_fd;


void recv_process(void)
{
    while(1)
    {
        maxfds = -1;
        maxfds = epoll_wait(epoll_fd, eventstmp, MAX_EPOLL_NUM, -1);
        for(i = 0; i < maxfds; ++i)
        {
            client_sock = eventsemp[i].data.fd
            if(eventstmp[i].events & EPOLLIN)
            {
                ret = RecvFrame(client_sock, &close_fd);
            
            }
        }
    }
}

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
    pthread_t recv_poress_fd;
    epoll_fd = epoll_create(100);
    
    ret = init_socket(port)
    if (0 >= ret)
    {
        printf(" init socket faile! \n");
        return -1;
    }

    ret = pthread_create(&recv_poress_fd, NULL, (void*)recv_poress, NULL);
    if(ret < 0)
    {
        printf("pthread create faile \n");
        exit(1);
    }
    
    int clilen;
    int clifd;
    struct sockaddr_in cliaddr;
    while(1)
    {
        clilen = sizeof(ciladdr);
        clifd = accept(socketfd, (void *)&cliaddr, &clilen);
        if (clifd < 0 && errno == EINTR)
        {
            continue;
        }
        else if(clifd < 0)
        {
            printf(" net server accept error !");
        }

        ev_reg.data.fd = clifd;
        ev_reg.events = EPOLLPRI| EPOLLIN |EPOLLET | EPOLLERR |EPOLLHUP;
        if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clifd, &ev_reg) < 0)
        {
            printf(" server add client to epoll ctl failed. clifd:", clifd);
            continue;
        }
    }
}

int main(int argc, char *argv[])
{
    return 0;
}
