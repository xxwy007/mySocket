/**
* @Anthor: xxwy007
* @Date: 2023/07/02 9:19:19
*/
#ifndef _MYSOCKET_H_
#define _MYSOCKET_H_
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")


using namespace std;
/**
*@class:
*/
class CTcpServer
{
	WSADATA wsaData;//用于存储socket信息
	SOCKET m_listenfd;//服务端用于监听的socket
	SOCKET m_clientfd;// 客户端连上来的socket
	SOCKADDR_IN  clientAddr;//客户端地址
public:
	CTcpServer();
	// 初始化服务端
	bool InitServer(PCSTR port);
	// 等待客户端的连接
	bool Accept();  

	// 向对端发送报文
	int  Send(const void* buf, const int buflen);
	// 接收对端的报文
	int  Recv(void* buf, const int buflen);
	//获取客户端ip
	char* GetIP();
	// 关闭客户端的socket
	void CloseClient();
	// 关闭用于监听的socket
	void CloseListen();

	~CTcpServer();

};


class CTcpClient
{
	SOCKET m_sockfd;//客户端sock
	WSADATA wsaData;//用于存储socket信息
public:

	CTcpClient();

	// 向服务器发起连接，serverip-服务端ip，port通信端口
	bool ConnectToServer(PCSTR serverip, PCSTR port);
	// 向对端发送报文
	int  Send(const void* buf, const int buflen);
	// 接收对端的报文
	int  Recv(void* buf, const int buflen);

	~CTcpClient();
};


#endif //_MYSOCEKT_H_