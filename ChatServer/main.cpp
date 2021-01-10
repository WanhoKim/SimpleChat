#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <zconf.h>
#include <arpa/inet.h>
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"

#define BUF_SIZE 4096
//메시지 송수신에 사용될 버퍼 크기를 선언

int main(int argc, char *argv[])
{
    char buffer[BUF_SIZE];
    struct sockaddr_in server_addr, client_addr;
    char temp[20];
    int server_fd, client_fd;
    int len, msg_size;

    if(argc != 2)
    {
        std::cout << "./server {listen_port}" << std::endl;
        exit(0);
    }

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {// 소켓 생성
        printf("Server : Can't open stream socket\n");
        exit(0);
    }
    memset(&server_addr, 0x00, sizeof(server_addr));
    //server_Addr 을 NULL로 초기화

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));
    //server_addr 셋팅

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0)
    {//bind() 호출
        printf("Server : Can't bind local address.\n");
        exit(0);
    }

    if(listen(server_fd, 5) < 0)
    {//소켓을 수동 대기모드로 설정
        printf("Server : Can't listening connect.\n");
        exit(0);
    }

    memset(buffer, 0x00, sizeof(buffer));
    printf("Server : wating connection request.\n");
    len = sizeof(client_addr);
    while(true) {
        client_fd = accept(server_fd, (struct sockaddr *) &client_addr, reinterpret_cast<socklen_t *>(&len));

        if (client_fd < 0) {
            printf("Server: accept failed.\n");
            exit(0);
        }

        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
        printf("Server : %s client connected.\n", temp);
        std::string greeting = std::string("Hi client, I'm server!");
        write(client_fd, greeting.c_str(), greeting.length());
        while (true) {
            read(client_fd, buffer, BUF_SIZE);
            std::cout << "Server got msg from client : "  << buffer << std::endl;
            std::cout << "Write a msg to send client : ";
            memset(buffer, '\0', BUF_SIZE);
            std::cin.getline(buffer, BUF_SIZE);
            write(client_fd, buffer, strlen(buffer));
            memset(buffer, '\0', BUF_SIZE);
        }
        close(client_fd);
    }
    close(server_fd);
    return 0;
}
