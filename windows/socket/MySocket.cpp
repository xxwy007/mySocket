#include "MySocket.h"

CTcpServer::CTcpServer()
{
	m_listenfd = INVALID_SOCKET;
	m_clientfd = INVALID_SOCKET;
	int iResult;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
	}
}

bool CTcpServer::InitServer(PCSTR port)
{

	struct addrinfo* result = NULL, * ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;//协议族
	hints.ai_socktype = SOCK_STREAM;//指定流套接字
	hints.ai_protocol = IPPROTO_TCP;//TCP
	hints.ai_flags = INADDR_ANY;
	// Resolve the local address and port to be used by the server
	int iResult;
	iResult = getaddrinfo(NULL, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return false;
	}

	//配置监听
	m_listenfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_listenfd == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	//配置绑定
	iResult = bind(m_listenfd, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(m_listenfd);
		WSACleanup();
		return false;
	}
	
	//监听
	if (listen(m_listenfd, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(m_listenfd);
		WSACleanup();
		return false;
	}
	else
	{
		printf("Server is start!!\nport is %s listening!!!",port);
	}
	return true;
}

// 等待客户端的连接
bool CTcpServer::Accept()
{
	m_clientfd = INVALID_SOCKET;

	// Accept a client socket
	int addrlen = sizeof(clientAddr);
	m_clientfd = accept(m_listenfd, (SOCKADDR*)&clientAddr, &addrlen);
	if (m_clientfd == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(m_listenfd);
		WSACleanup();
		return false;
	}
	char* clientIP = GetIP();
	printf("%s is connect!!", clientIP);
	return true;
}


int CTcpServer::Send(const void* buf, const int buflen)
{
	const char* buffer = static_cast<const char*>(buf);
	return send(m_clientfd, buffer, buflen, 0);
}

int CTcpServer::Recv(void* buf, const int buflen)
{
	char* buffer = static_cast<char*>(buf);
	return recv(m_clientfd, buffer, buflen, 0);
}

char* CTcpServer::GetIP()
{
	
	return (inet_ntoa(clientAddr.sin_addr));
}

void CTcpServer::CloseClient()
{
	if (m_clientfd != INVALID_SOCKET) closesocket(m_clientfd);
}

void CTcpServer::CloseListen()
{
	// cleanup
	if(m_listenfd != INVALID_SOCKET) closesocket(m_listenfd);
}

CTcpServer::~CTcpServer()
{
	if (m_listenfd != INVALID_SOCKET) closesocket(m_listenfd);  // 析构函数关闭socket
	if (m_clientfd != INVALID_SOCKET) closesocket(m_clientfd);  // 析构函数关闭socket
	WSACleanup();
}


CTcpClient::CTcpClient()
{
	m_sockfd = INVALID_SOCKET;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
	}
}

bool CTcpClient::ConnectToServer(PCSTR serverip, PCSTR port)
{
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	int iResult;
	// Resolve the server address and port
	iResult = getaddrinfo(serverip, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return false;
	}
	ptr = result;

	// Create a SOCKET for connecting to server
	m_sockfd = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);
	if (m_sockfd == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}
	iResult = connect(m_sockfd, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("Connect fail!!");
		closesocket(m_sockfd);
		m_sockfd = INVALID_SOCKET;
		return false;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(result);

	if (m_sockfd == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return false;
	}
	printf("Connect successful!!");
	return true;
}

int CTcpClient::Send(const void* buf, const int buflen)
{
	const char* buffer = static_cast<const char*>(buf);
	return send(m_sockfd, buffer, buflen, 0);
}

int CTcpClient::Recv(void* buf, const int buflen)
{
	char* buffer = static_cast<char*>(buf);
	return recv(m_sockfd, buffer, buflen, 0);
}

CTcpClient::~CTcpClient()
{
	if (m_sockfd != 0) closesocket(m_sockfd);
	WSACleanup();
}
