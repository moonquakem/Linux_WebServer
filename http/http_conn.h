#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

class http_conn
{
private:
    int m_sockfd;          // 存储与客户端通信的 socket 文件描述符（“管道编号”），后续通过它读写数据和关闭连接。
    sockaddr_in m_address; // 存储客户端的 IP 地址和端口号
    // （sockaddr_in 是 IPv4 地址结构），可用于日志记录等
public:
    http_conn()
    {
    }
    ~http_conn()
    {
    }
    void init(int sockfd, const sockaddr_in &addr)
    {
        m_sockfd=sockfd;
        m_address=addr;
    }

    //处理一次 HTTP 连接的核心逻辑：
    //接收客户端请求、解析请求、生成响应、发送响应并关闭连接。
    void process(){
        //读取请求
        char buffer[1024];
        memset(buffer,0,sizeof(buffer));
        //通过 m_sockfd（客户端 socket）读取数据到缓冲区
        read(m_sockfd,buffer,sizeof(buffer)-1);

        //简单的解析请求
        char method[10],path[10];
        sscanf(buffer,"%s %s",method,path);
        //构建响应
        const char* response="HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/html\r\n"
                            "Content-Length:23\r\n"
                            "\r\n"
                            "<h1>Hello World!</h1>";
        //发送响应
        write(m_sockfd,response,strlen(response));
        close(m_sockfd);
        
    }
};
//v1.1只处理最基础的ｇｅｔ请求

#endif