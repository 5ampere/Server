#include "ThreadList.h"

// ����һ���Ự�̲߳����Ͻ�����Ϣ
void conversationThread(Conversation* aConversation, mySocket* fatherSocket)
{
	aConversation->createConversation(*fatherSocket);
}

// ���̴߳���Ự�������ݣ��û���д��
void conversationProcess(Conversation* aConversation, Linklist* aLinklist)
{
	while (aConversation->ifConnectSuccess() == 0);		// �ȴ������ӳɹ�
	while (true)
	{
		if (aConversation->ifReceiveMessage() == 1)					// ������Ϣ����д���
		{
			/* �û����б�д���ݴ������ begin */
			/*
			�˷�������������������sim800ģ�����û����ֻ��ƶ��˽���ͨѶ
			�������ݸ�ʽ ��
				Ŀ��IP���ַ�����\r\n
				�������ݣ��ַ�����\n
			����������Ŀ��IP���ͻ���A���͵� ���������ݡ� ת���� ��Ŀ��IP����Ӧ�Ŀͻ���B
			*/
			static char times = 0;
			static string str[2];
			str[times] = aConversation->receiveMessage();
			times++;
			if (times == 2)		// ������һ�����ݰ�
			{
				times = 0;

				Node *temp;
				temp = aLinklist->Head;
				while (temp)		// �������лỰ�������Ƿ���Ŀ��IP
				{
					if (temp->aConversation->ClientAddress+"\r\n" == str[0])
					{
						temp->aConversation->sendMessage(str[1]);
						cout << "ת���ɹ���" << endl;
					}
					temp = temp->next;
				}
			}
			/* �û����б�д���ݴ������ end */
		}
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
	delete s;

	cout << "�Ự�رգ�" << endl;
}


