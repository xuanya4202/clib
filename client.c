#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <string.h>

#define MAX_READ_FRAME 1024
#define MAX_EPOLL_NUM 1000
int epoll_fd;

int printf_t(unsigned char *buf, size_t len)
{
    int i = 0;
    printf("..........\n");
    for(i = 0; i < len; ++i)
    {
        printf("%x ", buf[i]);
    }
    printf("----------\n");
}

int RecvFrame(int fd, int *close_fd)
{
    int count = 0;
    unsigned char rCharCase = 0;
    int recLen = 0;
    unsigned char buff[MAX_READ_FRAME];
    unsigned char recBuf[MAX_READ_FRAME];

    int i = 0;
    unsigned short frameLen = 0;
    unsigned char nChar;

    int rcvt;
    int rs = 1;
    int j = 0;
    unsigned char sum;
    while(rs)
    {
        count = recv(fd, buff, MAX_READ_FRAME, 0);
        if (count == 0) /* 对方正常断开*/
        {
            *close_fd = fd;
            return 1; 
        }
        else if( count < 0)
        {
            if (errno == EAGAIN)
            {
                printf("没有读到数据...\n");
                return 0;
            }
            else
            {
                printf("链接出现错误\n");
                return 1;
            }
        }
        
        printf_t(buff, count);
        if(count == MAX_READ_FRAME)
        {
            printf("接收帧数据超过最大值\n");
            rs = 1;
        }
        else
        {
            rs = 0;
        }
    }
}

int close_net(int fd)
{
    printf("close net dev!\n");
    if(fd > 0)
    {
        if(close(fd) == -1)
            return -1;
        fd = -1;
    }
    return 0;
}
void Disconnected(int client_sock)
{
    struct epoll_event ev_reg;
    close_net(client_sock);

    ev_reg.data.fd = client_sock;
    if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_sock, &ev_reg) < 0)
    {
        printf("recv_process:epoll delete success \n");
    }
    else
    {
        printf("recv_process:epoll delete failed\n");
    }
}

void recv_poress(void)
{
    int client_sock;  
    int maxfds;
    int i;
    struct epoll_event eventstmp[MAX_EPOLL_NUM];
    struct epoll_event ev_reg;
    int ret;
    int close_fd;

    while(1)
    {
        maxfds = -1;
        maxfds = epoll_wait(epoll_fd, eventstmp, MAX_EPOLL_NUM, -1);
        for(i = 0; i < maxfds; ++i)
        {
            client_sock = eventstmp[i].data.fd;
            if(eventstmp[i].events & EPOLLIN)
            {
                printf("接收 数据\n");
                ret = RecvFrame(client_sock, &close_fd);
                if (1 == ret)
                {
                    printf("对方正常退出\n");
                    Disconnected(close_fd);
                }
                else if(2 == ret)
                {
                    printf("关闭连接\n");
                    Disconnected(close_fd);
                }
            }
            else
            {
                printf("对方出现错误\n");
                Disconnected(client_sock);
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
        printf(" socket error! \n");
        return -1;
    }
    printf(" port:%d\n", port);
    memset((void *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    memset((void*)&(server_addr.sin_zero), 0, 8);

    if(connect(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        close(socketfd);
        printf("链接服务器失败\n");
        return -2;
    } 
    return socketfd;
}
int start_service(const char *ip, int port)
{
    int ret = 0;
    int i = 0;
    pthread_t recv_poress_fd;
    epoll_fd = epoll_create(100);
    struct epoll_event ev_reg; 
    int socketfd = init_socket(port);
    if (0 >= socketfd)
    {
        printf(" init socket faile! \n");
        return -1;
    }
    printf("init socket succ socketfd:%d\n", socketfd);
//    for(i = 0; i < 10; ++i)
    send(socketfd, "123456", sizeof("123456"), 0);
    sleep(10);
    close_net(socketfd);
}

int main(int argc, char *argv[])
{
    start_service("127.0.0.1", 8899);
    while(1) sleep(100);
    return 0;
}
