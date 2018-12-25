#include "socket.h"
#include <windows.h>
#include <iostream>
using namespace std;

/**
  * @brief  �ڶ˿��׽����ڴ���һ���Ự������ͣ������Ϣ
  * @param  
  *		-mySocket& aSocket ���˿�
  * @retval
  *		-0 ʧ��
  *		-1 �ɹ�
  */
int Conversation::createConversation(mySocket& aSocket)
{
	ifConSucc = 0;
	cout << "�ȴ�����..." << endl;
	sClient = accept(aSocket.slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
	cout << "���ӳɹ�" << endl;
	ifConSucc = 1;
	ClientAddress = inet_ntoa(remoteAddr.sin_addr);
	while (true)
	{
		//��������  
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
  * @brief  �رձ��Ự
  * @param  ��
  * @retval ��
  */
void Conversation::closeConversation()
{
	closesocket(sClient);
}

/**
  * @brief  �ж��Ƿ��յ�����Ϣ
  * @param  ��
  * @retval 
  *		-1 �յ���Ϣ
  *		-0 δ�յ���Ϣ
  */
int Conversation::ifReceiveMessage()
{
	return ifGetMessage;
}

/**
  * @brief  �жϸûỰ�Ƿ�������
  * @param  i �������ݲ����򷵻ص�ǰifConSucc��ֵ�����������������ifConSucc��ֵ������
  * @retval
  *		-1 �����ӿͻ���
  *		-0 δ����
  *		-2 ���Ӻ�ʧ
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
  * @brief  ������Ϣ
  * @param  string mess Ҫ���͵���Ϣ
  * @retval ��
  */
void Conversation::sendMessage(string mess)
{
	const char* a = mess.c_str();
	send(sClient, a, mess.length(), 0);
}

/**
  * @brief  ������Ϣ�������ַ�����ͬʱ���յ���Ϣ��־λ����
  * @param  ��
  * @retval �����յ�����Ϣ
  */
string Conversation::receiveMessage()
{
	ifGetMessage = 0;
	return mess;
}

/**
  * @brief  ��ʼ��wsa
  * @param  ��
  * @retval
  *		-0 ʧ��
  *		-1 �ɹ�
  */
int mySocket::WSAInit()
{
	//��ʼ��WSA  
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
		return 0;
	else
		return 1;
}

/**
  * @brief  ��ʼ���׽���  
  * @param  ��
  * @retval
  *		-0 ʧ��
  *		-1 �ɹ�
  */
int mySocket::CreateSockets()
{
	//�����׽���  
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
		return 0;
	else
		return 1;
}

/**
  * @brief  ��IP�Ͷ˿�
  * @param  ��
  *		-int port �˿ں�
  * @retval
  *		-0 ʧ��
  *		-1 �ɹ�
  */
int mySocket::BindingIPandPort(int port)
{
	//��IP�Ͷ˿�  
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
  * @brief  ��ʼ����
  * @param  ��
  * @retval
  *		-0 ʧ��
  *		-1 �ɹ�
  */
int mySocket::StartMonitoring()
{
	//��ʼ����  
	if (listen(slisten, 5) == SOCKET_ERROR)
		return 0;
	else
		return 1;
}

/**
  * @brief  ׼�������������ӵ��׽���
  * @param  
  *		-int port �˿ں�
  * @retval
  *		-0 ʧ��
  *		-1 �ɹ�
  */
int mySocket::SocketInit(int port)
{
	if (WSAInit() == 0)
	{
		cout << "WSA��ʼ��ʧ��" << endl;
		return 0;
	}
	else 
		cout << "WSA��ʼ���ɹ�" << endl;

	if (CreateSockets() == 0)
	{
		cout << "�����׽���ʧ��" << endl;
		return 0;
	}
	else
		cout << "�����׽��ֳɹ�" << endl;

	if (BindingIPandPort(port) == 0)
	{
		cout << "��IP�Ͷ˿�ʧ��" << endl;
		return 0;
	}
	else
		cout << "��IP�Ͷ˿ڳɹ�" << endl;

	if (StartMonitoring() == 0)
	{
		cout << "��ʼ����ʧ��" << endl;
		return 0;
	}
	else
		cout << "��ʼ�����ɹ�" << endl;
	return 1;
}