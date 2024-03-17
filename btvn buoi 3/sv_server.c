#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

struct sinhvien {
    char mssv[10];
    char hoten[30];
    char ngaysinh[20];
    float diemtb;
};

int main(int argc, char *argv[]) {
    // Tao socket cho ket noi
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1) {
        perror("socket() failed");
        return 1;
    }

    // Khai bao dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    // Gan socket voi cau truc dia chi
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind() failed");
        return 1;
    }

    // Chuyen socket sang trang thai cho ket noi
    if (listen(listener, 5)) {
        perror("listen() failed");
        return 1;
    }

    int client = accept(listener, NULL, NULL);

    struct sinhvien sv;

    FILE *f = fopen(argv[2], "a"); // Mở tệp để ghi thêm vào cuối tệp

    if (f == NULL) {
        perror("fopen() failed");
        return 1;
    }

    while (1) {
        int ret = recv(client, &sv, sizeof(sv), 0);
        if (ret <= 0)
            break;

        // Lấy thời gian hiện tại
        time_t now = time(NULL);
        struct tm *tm_now = localtime(&now);
        char time_str[50];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_now);

        // Ghi dữ liệu vào file log
        fprintf(f, "%s %s %s %.2f\n", inet_ntoa(addr.sin_addr), time_str, sv.mssv, sv.diemtb);

        // In ra màn hình
        printf("Received data from client: %s %s %s %.2f\n", inet_ntoa(addr.sin_addr), sv.mssv, sv.hoten, sv.diemtb);
    }

    fclose(f);
    close(client);
    close(listener);

    return 0;
}
