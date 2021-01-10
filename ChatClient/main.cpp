/*
daytime 클라이언트 프로그램이 실행되기 위해서는
서버쪽에서 xinetd의 daytime이 구동하고 있어야한다.
coder YoWu
*/

#include <cstdlib>
#include <iostream>
#include <zconf.h>
#include <arpa/inet.h>
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"

#define BUF_SIZE 4096

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "./client {server_hostname} {server_port_num}" << std::endl;
        exit(0);
    }
    int socket_fd;
    struct sockaddr_in server_addr;
    //struct sockaddr_in server_addr : 서버의 소켓주소 구조체
    char buf[BUF_SIZE+1];

    if((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {//소켓 생성과 동시에 소켓 생성 유효검사
        printf("can't create socket\n");
        exit(0);
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    //서버의 소켓주소 구조체 server_addr을 NULL로 초기화

    server_addr.sin_family = AF_INET;
    //주소 체계를 AF_INET 로 선택
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    //32비트의 IP주소로 변환
    server_addr.sin_port = htons(atoi(argv[2]));
    //daytime 서비스 포트 번호

    if(connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {//서버로 연결요청
        printf("can't connect.\n");
        exit(0);
    }

    while (true) {
        read(socket_fd, buf, BUF_SIZE);
        std::cout << "Client got msg from server : " << buf << std::endl;
	memset(buf, '\0', BUF_SIZE);
        std::cout << "Write msg to send server : ";
        std::cin.getline(buf, BUF_SIZE);
        write(socket_fd, buf, strlen(buf));
	memset(buf, '\0', BUF_SIZE);
    }

    close(socket_fd);
}
