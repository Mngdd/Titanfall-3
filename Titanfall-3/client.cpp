#include "client.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")// TODO: помогите, откуда их в симейке привязывать...


// Client::Client() {
//     SOCKET ConnectSocket = INVALID_SOCKET;
//     struct addrinfo *result = NULL,
//                     *ptr = NULL,
//                     hints;

//     // init Winsock
//     iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//     if (iResult != 0) {
//         printf("WSAStartup failed with error: %d\n", iResult);
//         return 1;
//     }
// }


// Тут все почти аналогично server.cpp, комментирую только новое
int client_test(const char ip[], int port) {
    WSADATA wsaData;// содержит сведения о реализации сокетов Windows
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    const char *sendbuf = "HOLA AMIGOS!";// отправляемая инфа

    char buf[DEFAULT_BUFLEN];//
    int iResult;
    // int buf_size = DEFAULT_BUFLEN; // useless

    // init Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));// чистим кусок памяти от мусора, все в нулях
    hints.ai_family = AF_UNSPEC;      // без семейства
    hints.ai_socktype = SOCK_STREAM;  // тип сокета, берем дефолтный
    hints.ai_protocol = IPPROTO_TCP;  // берем протокол TCP, т.е. нам все одной пачкой придет

    // getaddrinfo обеспечивает независимое от протокола преобразование из имени узла ANSI в адрес
    // (крч в нормальный вид приводим адрес, и еще динамич. память выделяем на это!)
    iResult = getaddrinfo(ip, std::to_string(port).c_str(), &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // подключаемся до тех пор, пока нас не подключит
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // создаем сокет чтоб к серверу подключиться
        ConnectSocket = Socket(ptr);

        // пытаемся подключиться
        Connect(ConnectSocket, ptr);// если не удалось - отключаем сокет
        if (ConnectSocket == INVALID_SOCKET) { continue; }
        break;
    }

    freeaddrinfo(result);// мы настроились уже, не нужен он больше

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // отправляем...
    //TODO: добавить Send и Read!!
    iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // мы отправили все что хотели, отключаемся от отправки
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        // change da wolrd, my final message, goodbye
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // получаем до тех пор, пока соединение не разорвется
    do {
        iResult = recv(ConnectSocket, buf, DEFAULT_BUFLEN, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);

    // чистим, иначе сокеты будут висеть занятыми
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}