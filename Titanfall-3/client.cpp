#include "client.h"
#include <cstdio> // printf
#include <unistd.h> // write
#include "err_handle.h"

// Тут все почти аналогично server.cpp, комментирую только новое
int client_test() {
    int fd = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {};
    adr.sin_family = AF_INET;
    //TODO: PORT+IP МЕНЯТЬ
    adr.sin_port = htonl(34543);
    Inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr); // задаем адрес куда подключаемся
    Connect(fd, (struct sockaddr *) &adr, sizeof(adr));
    // чета там тройное рукопожатие произойдет...
    write(fd, "HOLA AMIGOS\n", 12);

    char buf[256];
    ssize_t nread;
    nread = read(fd, buf, 256);
    if (nread == -1) { // копипаст из server.cpp... ачебынет)
        perror("(client) read failed (client)");
        exit(EXIT_FAILURE);
    }
    if (nread == 0) {
        printf("EOF occurred (client)\n");
    }
    write(STDOUT_FILENO, buf, nread); // пишем в консось че пришло
    close(fd); // пака
    return 0;
}