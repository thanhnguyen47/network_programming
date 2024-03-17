// 1. Viết chương trình tcp_client, kết nối đến một máy
// chủ xác định bởi địa chỉ IP và cổng. Sau đó nhận dữ liệu
// từ bàn phím và gửi đến server. Tham số được truyền
// vào từ dòng lệnh có dạng
// tcp_client <địa chỉ IP> <cổng>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client == -1) {
        perror("socket() failed.\n");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int ret = connect(client, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        perror("connect() failed.\n");
        exit(1);
    }    

    char buf[1024];
        printf("Send to server: ");
        fgets(buf, sizeof(buf), stdin);
        send(client, buf, sizeof(buf), 0);

        ret = recv(client, buf, sizeof(buf), 0);
        buf[ret] = 0;
        printf("Data received: %s\n", buf);
    close(client);
    return 0;
}