#include"myTcp.h"

int main()
{
    CTcpClient client;
    if(client.ConnectToServer("1.14.16.83",8090) == false)
    {cout<<"connect fail,exit"<<endl;return -1;}
    cout<<"连接成功"<<endl;
    string str;
    char strbuffer[1024];
    // while(1)
    // {
    //     cout<<endl<<"输入发送信息:"<<endl;
    //     cin>>str;
    //     if(str == "quit")break;
    //     if(client.Send(str.c_str(),str.size()) <= 0) break;

    //     memset(strbuffer,0,sizeof(strbuffer));
    //     if(client.Recv(strbuffer,sizeof(strbuffer)) <= 0) break;
    //     cout<<"接收："<<strbuffer<<endl;
    // }
    for (int ii=0;ii<50;ii++)
    {
        memset(strbuffer,0,sizeof(strbuffer));
        sprintf(strbuffer,"这是第%d个超级女生,编号%03d(%d)。",ii+1,ii+1,getpid());
        if (client.Send(strbuffer,strlen(strbuffer))<=0) break;
        printf("发送：%s\n",strbuffer);
        
        memset(strbuffer,0,sizeof(strbuffer));
        if (client.Recv(strbuffer,sizeof(strbuffer))<=0) break;
        printf("接收：%s\n",strbuffer);

        sleep(1);  // sleep一秒，方便观察程序的运行。
    }
    cout<<"连接已断开"<<endl;
}