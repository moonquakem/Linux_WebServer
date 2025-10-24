// webserver.h
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "./http/http_conn.h"

const int MAX_FD=65536;//限制服务器支持的最大并发连接数（每个连接对应一个文件描述符
class WebServer{
public:
    WebServer();
    ~WebServer();
    void init(int port);
    void eventListen();
    void eventLoop();
private:
    int m_port;//服务器监听的端口号
    int m_listenfd;//存储服务器的监听 socket 文件描述符（通过 socket() 函数创建），
    //用于接收客户端的连接请求（类似 “总机电话”，专门负责接新客人）。
    int m_epollfd;//存储 epoll 实例的文件描述符
    http_conn *users;//为每个客户端连接分配一个 http_conn 对象（通过文件描述符作为索引），
    //用于单独处理该客户端的 HTTP 请求（类似 “为每个客人分配一个专属服务员”）。

};

#endif