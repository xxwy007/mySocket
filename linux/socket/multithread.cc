#include"myTcp.h"
// 与客户端通信线程的主函数
void *pth_main(void *arg);
vector<long> vpthid;
CTcpServer TcpServer;
void EXIT(int sig)
{
  cout<<"EXIT begin..."<<endl;
  TcpServer.CloseListen();
  for (int ii=0;ii<vpthid.size();ii++) 
  {
    printf("cancel %ld ",vpthid[ii]);
    pthread_cancel(vpthid[ii]);
  }
  cout<<"EXIT END..."<<endl;
  exit(0);
}
//线程清理函数
void pth_main_exit(void* arg);

int main()
{
  // 忽略全部的信号
  for (int ii=0;ii<50;ii++) signal(ii,SIG_IGN);

  // 设置SIGINT和SIGTERM的处理函数
  signal(SIGINT,EXIT); signal(SIGTERM,EXIT);
  if (TcpServer.InitServer(8090)==false)
  { printf("服务端初始化失败，程序退出。\n"); return -1; }
  cout<<"初始化成功，正在监听……"<<endl;
  while (1)
  {
    if (TcpServer.Accept() == false) continue;
    cout<<"客户端连接成功:"<<TcpServer.GetIP()<<endl;
    pthread_t pthid;   // 创建一线程，与新连接上来的客户端通信
    if (pthread_create(&pthid,NULL,pth_main,(void*)((long)TcpServer.m_clientfd))!=0)
    { printf("创建线程失败，程序退出。n"); return -1; }

    vpthid.push_back(pthid);
    printf("与客户端通信的线程已创建。\n");
  }
}


// 与客户端通信线程的主函数
void *pth_main(void *arg)
{
  cout<<"pth_main begin."<<endl;
  //设置线程清理函数
  pthread_cleanup_push(pth_main_exit,arg);
  //分离线程
  pthread_detach(pthread_self());
  //设置取消方式-立即取消
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);



  int clientfd=(int)(long) arg; // arg参数为新客户端的socket。
  // 与服务端通信
  char strbuffer[1024];

  while (1)
  {
    memset(strbuffer,0,sizeof(strbuffer));
    if (recv(clientfd,strbuffer,sizeof(strbuffer),0)<=0) break;
    printf("接收：%s\n",strbuffer);

    strcpy(strbuffer,"ok");
    if (send(clientfd,strbuffer,strlen(strbuffer),0)<=0) break;
    printf("发送：%s\n",strbuffer);

    // if (TcpServer.Recv(strbuffer,sizeof(strbuffer))<=0) break;
    // printf("接收：%s\n",strbuffer);
    // strcpy(strbuffer,"ok");
    // if (TcpServer.Send(strbuffer,strlen(strbuffer))<=0) break;
    // printf("发送：%s\n",strbuffer);

  }

   pthread_cleanup_pop(1);
  printf("客户端已断开连接。\n");

  //close(clientfd);  // 关闭客户端的连接。
  cout<<"pth_main begin."<<endl;
  pthread_exit(0); 
}

void pth_main_exit(void* arg)
{
  cout<<"pth_main_exit begin."<<endl;
  //关闭与客户端链接的socket
  close((int)(long)arg);

  //从vpthid中删除本线程id
  for(int i=0;i<vpthid.size();i++)
  if(vpthid[i]==pthread_self())
    vpthid.erase(vpthid.begin()+i);
  cout<<"pth_main_exit end."<<endl;
}