#include"myTcp.h"
/////////////////////////////////////////////////
//服务端实现
CTcpServer::CTcpServer()
{
  // 构造函数初始化socket
  m_listenfd=m_clientfd=0;
}

CTcpServer::~CTcpServer()
{
  if (m_listenfd!=0) close(m_listenfd);  // 析构函数关闭socket
  if (m_clientfd!=0) close(m_clientfd);  // 析构函数关闭socket
}

// 初始化服务端的socket，port为通信端口
bool CTcpServer::InitServer(int port)
{
  m_listenfd = socket(AF_INET,SOCK_STREAM,0);  // 创建服务端的socket

  // 把服务端用于通信的地址和端口绑定到socket上
  struct sockaddr_in servaddr;    // 服务端地址信息的数据结构
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;  // 协议族，在socket编程中只能是AF_INET
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // 本主机的任意ip地址
  servaddr.sin_port = htons(port);  // 绑定通信端口
  if (bind(m_listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) != 0 )
  { close(m_listenfd); m_listenfd=0; return false; }

  // 把socket设置为监听模式
  if (listen(m_listenfd,5) != 0 ) { close(m_listenfd); m_listenfd=0; return false; }

  return true;
}

bool CTcpServer::Accept()
{
  if ( (m_clientfd=accept(m_listenfd,0,0)) <= 0) return false;

  return true;
}

int CTcpServer::Send(const void *buf,const int buflen)
{
  return send(m_clientfd,buf,buflen,0);
}

int CTcpServer::Recv(void *buf,const int buflen)
{
  return recv(m_clientfd,buf,buflen,0);
}
char *CTcpServer::GetIP()
{
  return(inet_ntoa(m_clientaddr.sin_addr));
}

void CTcpServer::CloseClient()    // 关闭客户端的socket
{
  if (m_clientfd!=0) { close(m_clientfd); m_clientfd=0; }
}

void CTcpServer::CloseListen()    // 关闭用于监听的socket
{
  if (m_listenfd!=0) { close(m_listenfd); m_listenfd=0; }
}

///////////////////////////////////////////////////////////////////////////////
//客户端实现
CTcpClient::CTcpClient()
{
  m_sockfd=0;  // 构造函数初始化m_sockfd
}

CTcpClient::~CTcpClient()
{
  if (m_sockfd!=0) close(m_sockfd);  // 析构函数关闭m_sockfd
}

// 向服务器发起连接，serverip-服务端ip，port通信端口
bool CTcpClient::ConnectToServer(const char *serverip,const int port)
{
  m_sockfd = socket(AF_INET,SOCK_STREAM,0); // 创建客户端的socket

  struct hostent* h; // ip地址信息的数据结构
  if ( (h=gethostbyname(serverip))==0 ) 
  { close(m_sockfd); m_sockfd=0; return false; }

  // 把服务器的地址和端口转换为数据结构
  struct sockaddr_in servaddr;
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port); 
  memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);

  // 向服务器发起连接请求
  if (connect(m_sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
  { close(m_sockfd); m_sockfd=0; return false; }

  return true;
}

int CTcpClient::Send(const void *buf,const int buflen)
{
  return send(m_sockfd,buf,buflen,0);
}

int CTcpClient::Recv(void *buf,const int buflen)
{
  return recv(m_sockfd,buf,buflen,0);
}