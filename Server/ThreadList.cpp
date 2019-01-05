#include "ThreadList.h"
#include <windows.h>

// ����һ���Ự�̲߳����Ͻ�����Ϣ
void conversationThread(Conversation* aConversation, mySocket* fatherSocket)
{
	aConversation->createConversation(*fatherSocket);
	return;
}

// ���̴߳���Ự�������ݣ��û���д��
void conversationProcess(Conversation* aConversation, Linklist* aLinklist)
{
	while (aConversation->ifConnectSuccess() == 0)Sleep(200);		// �ȴ������ӳɹ�
	while (true)
	{
		if (aConversation->ifConnectSuccess() == 2) return;			// ���ѶϿ�������������
		if (aConversation->ifReceiveMessage() == 1)					// ������Ϣ����д���
		{
			/* �û����б�д���ݴ������ begin */
			/*
			�˷�������������������sim800ģ�����û����ֻ��ƶ��˽���ͨѶ
			�������ݸ�ʽ ��
				��һ�䣺From����ID���ַ�����
				�ڶ��䣺ToĿ��ID���ַ�����
				�����䣺Dat�������ݣ��ַ�����
			����������Ŀ��ID���ͻ���A���͵� ���������ݡ� ת���� ��Ŀ��ID����Ӧ�Ŀͻ���B
			*/
			string str;
			str = aConversation->receiveMessage();
			if (str.find("From") != -1){
				str.replace(0, 4, "");
				cout << str << endl;
				aConversation->FromID = str;
			}
			if (str.find("To") != -1) {
				str.replace(0, 2, "");
				cout << str << endl;
				aConversation->ToID = str;
			}
			if (str.find("Dat") != -1)
			{
				str.replace(0, 3, "");
				cout << str << endl;
				Node *temp;
				temp = aLinklist->Head;
				while (temp)		// �������лỰ�������Ƿ���Ŀ��IP
				{
					if (temp->aConversation->FromID == aConversation->ToID && aConversation->ToID != "")
					{
						temp->aConversation->sendMessage(str);		// �������ݲ���
						cout << "ת���ɹ���" << endl;
					}
					temp = temp->next;
				}
			}
			/* �û����б�д���ݴ������ end */
		}
		else
			Sleep(50);
	}
}

// ���ô˺�����������̬����Ự������߳�
void listProcessThread(Linklist& myList)
{
	thread listProcess(&Linklist::listProcess, myList);
	listProcess.detach();
}

//���캯��������һ��Ĭ�ϻỰ�߳�
Linklist::Linklist(mySocket& fatherServer) {                   
	fatherSocket = fatherServer;

	Conversation* aConversation = new Conversation;
	aConversation->ifConnectSuccess(1);
	Node *aNode = new Node 
	({
		aConversation,
		NULL,
		NULL,
		NULL
	});

	Head = aNode;	
}

// ��̬���ӡ�ɾ���Ự�б����Ԫ��
void Linklist::listProcess()
{
	Node *temp;
	temp = Head;
	int j = 0;
	while (true)
	{
		while (temp) {	
			if (temp->next == NULL && temp->aConversation->ifConnectSuccess() == 1)
			{
				Insert();
				break;
			}				
			if (temp->aConversation->ifConnectSuccess() == 2)
			{
				Delete(j);
				break;
			}
				
			j++;
			temp = temp->next;
		}
		temp = Head;
		j = 0;
		Sleep(25);
	}
}

//��ĩβ����һ��Ự����
void Linklist::Insert() {       
	Node *temp;
	temp = Head;

	while (temp) {
		if (temp->next == NULL)
			break;
		temp = temp->next;
	}

	Conversation* aConversation = new Conversation ;
	Node *aNode = new Node
	({
		aConversation,
		new thread(conversationThread, aConversation, &fatherSocket),
		new thread(conversationProcess, aConversation, this),
		NULL
	});
	aNode->tConversation->detach();
	aNode->tProcess->detach();
	temp->next = aNode;
}

//ɾ��i��������
void Linklist::Delete(int i) {          
	Node *temp;
	temp = Head;
	int j = 0;
	while (temp && j < i-1) {
		temp = temp->next;
		j++;
	}

	Node *s;
	s = temp->next;
	temp->next = s->next;
	cout << "����" << s->aConversation->FromID << "�ĻỰ�رգ�" << endl;
	delete s->tProcess;
	delete s->tConversation;
	delete s->aConversation;
	delete s;
}


