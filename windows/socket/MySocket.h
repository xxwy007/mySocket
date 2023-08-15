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
	WSADATA wsaData;//���ڴ洢socket��Ϣ
	SOCKET m_listenfd;//��������ڼ�����socket
	SOCKET m_clientfd;// �ͻ�����������socket
	SOCKADDR_IN  clientAddr;//�ͻ��˵�ַ
public:
	CTcpServer();
	// ��ʼ�������
	bool InitServer(PCSTR port);
	// �ȴ��ͻ��˵�����
	bool Accept();  

	// ��Զ˷��ͱ���
	int  Send(const void* buf, const int buflen);
	// ���նԶ˵ı���
	int  Recv(void* buf, const int buflen);
	//��ȡ�ͻ���ip
	char* GetIP();
	// �رտͻ��˵�socket
	void CloseClient();
	// �ر����ڼ�����socket
	void CloseListen();

	~CTcpServer();

};


class CTcpClient
{
	SOCKET m_sockfd;//�ͻ���sock
	WSADATA wsaData;//���ڴ洢socket��Ϣ
public:

	CTcpClient();

	// ��������������ӣ�serverip-�����ip��portͨ�Ŷ˿�
	bool ConnectToServer(PCSTR serverip, PCSTR port);
	// ��Զ˷��ͱ���
	int  Send(const void* buf, const int buflen);
	// ���նԶ˵ı���
	int  Recv(void* buf, const int buflen);

	~CTcpClient();
};


#endif //_MYSOCEKT_H_