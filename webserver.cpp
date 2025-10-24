#include"webserver.h"
#include<sys/epoll.h>

WebServer::WebServer(){
    users=new http_conn[MAX_FD];//酒店的服务员人数
}//调用这个类里面的构造函数，声明和定义分开“分离编译“
WebServer::~WebServer(){
    close(m_epollfd);
    close(m_listenfd);
    delete[] users;
}

void WebServer::init(int port){
    m_port=port;
}

void WebServer::eventListen(){
    //创建监听ｓｏｃｋｅｔ
    m_listenfd=socket(PF_INET,SOCK_STREAM,0);
    //设置端口复用
    int reuse=1;
    setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));

    //bind绑定
    struct sockaddr_in address;
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=htonl(INADDR_ANY);
    address.sin_port=htons(m_port);
    bind(m_listenfd,(struct sockaddr*)&address,sizeof(address));
    
    //监听
    listen(m_listenfd,5);
    //创建ｅｐｏｌｌ
    m_epollfd=epoll_create(5);

    //添加监听事件
    struct epoll_event event;
    event.data.fd=m_listenfd;
    event.events=EPOLLET;
    epoll_ctl(m_epollfd,EPOLL_CTL_ADD,m_listenfd,&event);
}
void WebServer::eventLoop(){
    struct epoll_event events[1024];

    while(true){
        int number=epoll_wait(m_epollfd,events,1024,-1);

        for(int i=0;i<number;i++){
            int sockfd=events[i].data.fd;

            if(sockfd==m_listenfd){
                //处理新连接
                struct sockaddr_in client_address;
                socklen_t client_addresslen=sizeof(client_address);
                int connfd=accept(m_listenfd,(struct sockaddr*)&client_address,
                &client_addresslen);

                //初始化ｈｔｔｐ链接
                users[connfd].init(connfd,client_address);

                //添加到ｅｐｏｌｌ
                struct epoll_event event;
                event.data.fd=connfd;
                event.events=EPOLLIN;
                epoll_ctl(m_epollfd,EPOLL_CTL_ADD,connfd,&event);
            }else if(events[i].events&EPOLLIN){
                //处理客户端请求
                users[sockfd].process();
            }
        }
    }
}