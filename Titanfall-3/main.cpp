// у меня не робят закоменченные инклюды лол
//#include "level_gen.h"
//#include "main.h"
//
//#include <Graph_lib/Graph.h>
//#include <Graph_lib/Simple_window.h>
#include "server.h"
#include "client.h"

//using namespace Graph_lib;

int main() {
    // одновременно не запустятся тк сервер умирает потом клиент ждет
    server_test();
    client_test();
}
//  закоментил для своих тестов, вот так уот
//try
//{
//    Point t1{100,100};
//    Simple_window win{t1, 600, 400, "Canvas"}; // как сделать пустое поле без простейшей кнопки? и что означает первая корда?
//}