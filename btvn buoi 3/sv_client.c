// Viết chương trình sv_client, cho phép người dùng nhập dữ liệu
// là thông tin của sinh viên bao gồm MSSV, họ tên, ngày sinh, và
// điểm trung bình các môn học. Các thông tin trên được đóng gói
// và gửi sang sv_server. Địa chỉ và cổng của server được nhập từ
// tham số dòng lệnh

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

struct sinhvien {
    char mssv[10];
    char hoten[30];
    char ngaysinh[20];
    float diemtb;
};

int main(int argc, char* argv[]) 
{
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        perror("connect() failed");
        return 1;
    }

    struct sinhvien sv;
    
    while (1) {
        printf("MSSV: ");
        scanf("%s", sv.mssv);
        printf("ho ten: ");
        fgets(sv.hoten, sizeof(sv.hoten), stdin);
        printf("ngay sinh: ");
        scanf("%s", sv.ngaysinh);
        printf("diem trung binh: ");
        scanf("%f", &sv.diemtb);

        send(client, &sv, sizeof(sv), 0);   
    }

    close(client);

    return 0;
}