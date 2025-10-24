#include"webserver.h"

int main(){
    int port=9006;
    WebServer server;
    server.init(port);
    server.eventListen();
    server.eventLoop();
    return 0;
}