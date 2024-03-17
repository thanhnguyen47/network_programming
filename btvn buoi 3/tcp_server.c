#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1) {
        perror("socket() failed.\n");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind() failed.\n");
        exit(1);
    }

    if (listen(listener, 5) == -1) {
        perror("listen() failed.\n");
        exit(1);
    }

    
    FILE *f1 = fopen(argv[2], "rb");
    char greetings[1024];
    int ret = fread(greetings, 1, sizeof(greetings), f1);

    FILE *f2 = fopen(argv[3], "wb");
    char buf[1024];

    while(1) {
        int client = accept(listener, NULL, NULL);
        send(client, greetings, ret, 0);
        
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0) {
            close(client);
            continue;
        } 
        buf[ret] = 0;
        
        fwrite(buf, 1, ret, f2);
        close(client);
    }

    fclose(f1); fclose(f2);
    close(listener);

    return 0;
    

}