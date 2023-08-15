#include "MySocket.h"
int main()
{
	CTcpClient* client = new CTcpClient();
	//client->ConnectToServer("1.14.16.83", "8090");
	client->ConnectToServer("127.0.0.1", "8090");
	Sleep(10000);
	return 0;
}