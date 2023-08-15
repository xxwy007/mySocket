#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <signal.h>
#include <pthread.h>
#include <vector>
using namespace std;
//TCP服务端类
class CTcpServer
{
private:
  struct sockaddr_in m_clientaddr;//客户端地址
public:
  int m_listenfd;   // 服务端用于监听的socket
  int m_clientfd;   // 客户端连上来的socket

  CTcpServer();
  bool InitServer(int port);  // 初始化服务端

  bool Accept();  // 等待客户端的连接

  // 向对端发送报文
  int  Send(const void *buf,const int buflen);
  // 接收对端的报文
  int  Recv(void *buf,const int buflen);

  //获取客户端ip
  char *GetIP();

  // 关闭客户端的socket
  void CloseClient();    
  // 关闭用于监听的socket
  void CloseListen();    

 ~CTcpServer();
};

// TCP客户端类
class CTcpClient
{
public:
  int m_sockfd;

  CTcpClient();

  // 向服务器发起连接，serverip-服务端ip，port通信端口
  bool ConnectToServer(const char *serverip,const int port);
  // 向对端发送报文
  int  Send(const void *buf,const int buflen);
  // 接收对端的报文
  int  Recv(void *buf,const int buflen);

 ~CTcpClient();
};
