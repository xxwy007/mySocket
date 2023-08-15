#include "MySocket.h"
int main()
{
	CTcpServer* server = new CTcpServer();
	server->InitServer("8090");
	server->Accept();
	return 0;
}