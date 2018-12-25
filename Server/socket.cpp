#include "socket.h"
#include <windows.h>
#include <iostream>
using namespace std;

/**
  * @brief  在端口套接字内创造一个会话，并不停接受消息
  * @param  
  *		-mySocket& aSocket 父端口
  * @retval
  *		-0 失败
  *		-1 成功
  */
int Conversation::createConversation(mySocket& aSocket)
{
	ifConSucc = 0;
	cout << "等待连接..." << endl;
	sClient = accept(aSocket.slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
	cout << "连接成功" << endl;
	ifConSucc = 1;
	ClientAddress = inet_ntoa(remoteAddr.sin_addr);
	while (true)
	{
		//接收数据  
		ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			mess = revData;
			ifGetMessage = 1;
		}
		else 
		{
			ifConSucc = 2;
			return 0;
		}
		Sleep(10);
	}
}

/**
  * @brief  关闭本会话
  * @param  无
  * @retval 无
  */
void Conversation::closeConversation()
{
	closesocket(sClient);
}

/**
  * @brief  判断是否收到了消息
  * @param  无
  * @retval 
  *		-1 收到消息
  *		-0 未收到消息
  */
int Conversation::ifReceiveMessage()
{
	return ifGetMessage;
}

/**
  * @brief  判断该会话是否有连接
  * @param  i 若不传递参数则返回当前ifConSucc的值，若传入参数则设置ifConSucc的值并返回
  * @retval
  *		-1 已连接客户端
  *		-0 未连接
  *		-2 连接后丢失
  */
int Conversation::ifConnectSuccess(int i)
{
	if (i == -1)
		return ifConSucc;
	else
		ifConSucc = i;
	return ifConSucc;
}

/**
  * @brief  发送消息
  * @param  string mess 要发送的消息
  * @retval 无
  */
void Conversation::sendMessage(string mess)
{
	const char* a = mess.c_str();
	send(sClient, a, mess.length(), 0);
}

/**
  * @brief  接受消息，返回字符串，同时将收到消息标志位清零
  * @param  无
  * @retval 返回收到的消息
  */
string Conversation::receiveMessage()
{
	ifGetMessage = 0;
	return mess;
}

/**
  * @brief  初始化wsa
  * @param  无
  * @retval
  *		-0 失败
  *		-1 成功
  */
int mySocket::WSAInit()
{
	//初始化WSA  
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
		return 0;
	else
		return 1;
}

/**
  * @brief  初始化套接字  
  * @param  无
  * @retval
  *		-0 失败
  *		-1 成功
  */
int mySocket::CreateSockets()
{
	//创建套接字  
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
		return 0;
	else
		return 1;
}

/**
  * @brief  绑定IP和端口
  * @param  无
  *		-int port 端口号
  * @retval
  *		-0 失败
  *		-1 成功
  */
int mySocket::BindingIPandPort(int port)
{
	//绑定IP和端口  
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
		return 0;
	else
		return 1;
}

/**
  * @brief  开始监听
  * @param  无
  * @retval
  *		-0 失败
  *		-1 成功
  */
int mySocket::StartMonitoring()
{
	//开始监听  
	if (listen(slisten, 5) == SOCKET_ERROR)
		return 0;
	else
		return 1;
}

/**
  * @brief  准备用于网络连接的套接字
  * @param  
  *		-int port 端口号
  * @retval
  *		-0 失败
  *		-1 成功
  */
int mySocket::SocketInit(int port)
{
	if (WSAInit() == 0)
	{
		cout << "WSA初始化失败" << endl;
		return 0;
	}
	else 
		cout << "WSA初始化成功" << endl;

	if (CreateSockets() == 0)
	{
		cout << "创建套接字失败" << endl;
		return 0;
	}
	else
		cout << "创建套接字成功" << endl;

	if (BindingIPandPort(port) == 0)
	{
		cout << "绑定IP和端口失败" << endl;
		return 0;
	}
	else
		cout << "绑定IP和端口成功" << endl;

	if (StartMonitoring() == 0)
	{
		cout << "开始监听失败" << endl;
		return 0;
	}
	else
		cout << "开始监听成功" << endl;
	return 1;
}