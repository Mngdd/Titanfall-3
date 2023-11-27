#include "server.h"
// TODO: раскидать на хедеры!!
#include <cstdio> // printf
#include <unistd.h> // write
#include "err_handle.h"


int server_test() {
    WSADATA wsaData; // содержит сведения о реализации сокетов Windows
    int iResult;

    // инициализируем Winsock, проверяем на ошибки
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData); // используем winsock 2.2
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }


    int server = Socket(AF_INET, SOCK_STREAM, 0);
    // 1-какое семейство протоколов (ipv4)
    // tcp-sock_steam, udp-dgram; 0-я хз, дефолтное значние какоето

    struct sockaddr_in adr = {}; // структура, чтоб задать адрес по протоколу ipv4
    adr.sin_family = AF_INET; // семейство адресов
    //TODO: ПОРТ МЕНЯТЬ
    adr.sin_port = htonl(34543); // порт, на котором слушаем, 34543 - по фану порт взяли
    Bind(server, (struct sockaddr *) &adr, sizeof(adr)); // теперь привязываем сокет к адресу

    Listen(server, 5);  // слушаем на сокете server, максимум 5 челов может ждать в очереди
    socklen_t adrlen = sizeof(adr);
    int fd = Accept(server, (struct sockaddr *) &adr, &adrlen); // принимаем клиента с сокета прослушиваемого
    // adr - впишем инфу о клиенте(юзаем эту переменную для других целей, bind себе уже сохранил)

    ssize_t nread; // далее читаем
    char buf[256]; // куда пишем
    nread = read(fd, buf, 256); // от кого, куда, сколько максимум прочтем в буфер
    // nread - сколько байт считали

    if (nread == -1) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    if (nread == 0) { // дошли до конца файла
        printf("EOF occurred\n");
    } // клиент ниче не собирается больше передавать, но мб ждет передачу
    write(STDOUT_FILENO, buf, nread); // пишем скок получили
    write(fd, buf, nread); // передаем на fd -> buf размером nread
    // просто эхо

    sleep(1);

    close(fd);
    close(server);
    return 0;
}