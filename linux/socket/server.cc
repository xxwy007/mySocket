#include"myTcp.h"

int main()
{
    CTcpServer server;
    if(server.InitServer(8090)==false)
    {cout<<"Init fail"<<endl;return -1;}
    cout<<"初始化成功，正在监听……"<<endl;
    if(server.Accept() == false) 
    {cout<<"Accept fail";return -1;}

    cout<<"客户端连接成功:"<<server.GetIP()<<endl;

    char strbuffer[1024];
    while(1)
    {
        memset(strbuffer,0,sizeof(strbuffer));
        if(server.Recv(strbuffer,sizeof(strbuffer)) <=0 ) break;
        cout<<"接收："<<strbuffer<<endl;

        strcpy(strbuffer,"ok");
        if (server.Send(strbuffer,strlen(strbuffer))<=0) break;
        cout<<"发送："<<strbuffer<<endl;
    }

    cout<<"客户端断开连接"<<endl;
    return 0;
}
