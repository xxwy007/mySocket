#include"myTcp.h"
int main()
{
    signal(SIGCHLD,SIG_IGN);  // 忽略子进程退出的信号，避免产生僵尸进程 
    CTcpServer server;
    if(server.InitServer(8090) == false)
    {cout<<"Init fail"<<endl;return -1;}
    cout<<"初始化成功，正在监听……"<<endl;
    while(1)
    {
        if(server.Accept()==false) continue;
        if(fork() > 0)
        {server.CloseClient();continue;}//父进程只进行监听

        server.CloseListen();//子进程只负责信息处理
        cout<<"客户端已连接："<<server.GetIP()<<endl;

        char strbuffer[1024];
        while (1)
        {
        memset(strbuffer,0,sizeof(strbuffer));
        if (server.Recv(strbuffer,sizeof(strbuffer))<=0) break;
        printf("接收：%s\n",strbuffer);

        strcpy(strbuffer,"ok");
        if (server.Send(strbuffer,strlen(strbuffer))<=0) break;
        printf("发送：%s\n",strbuffer);
        }
        cout<<"客户端断开连接："<<server.GetIP()<<endl;
        return 0;//子进程结束
    }
}