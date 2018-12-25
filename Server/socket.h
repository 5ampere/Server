#ifndef __SOCKER_H
#define __SOCKER_H

#include <winsock2.h>  
#pragma comment(lib,"ws2_32.lib")
#include <string>
using namespace std;

class mySocket;
class Conversation;

class Conversation
{
public:
	int createConversation(mySocket& slisten);
	void closeConversation();
	int ifReceiveMessage();
	int ifConnectSuccess(int i = -1);
	void sendMessage(string mess);
	string receiveMessage();
	string FromID;
	string ToID;
	char ifGetMessage;
private:
	SOCKET sClient;
	sockaddr_in remoteAddr;
	char ifConSucc;
	string mess;
	string ClientAddress;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	int ret;
protected:
};

class mySocket
{
	friend int Conversation::createConversation(mySocket& slisten);
public:
	int WSAInit();
	int CreateSockets();
	int BindingIPandPort(int port);
	int StartMonitoring();
	int SocketInit(int port);
private:
	WORD sockVersion;
	WSADATA wsaData;
	SOCKET slisten;
	sockaddr_in sin;
public:
};

#endif /*__SOCKER_H*/
