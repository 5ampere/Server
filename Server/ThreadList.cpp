#include "ThreadList.h"

// ����һ���Ự�̲߳����Ͻ�����Ϣ
void conversationThread(Conversation* aConversation, mySocket* fatherSocket)
{
	aConversation->createConversation(*fatherSocket);
}

// ���̴߳���Ự�������ݣ��û���д��
void conversationProcess(Conversation* aConversation)
{
	while (aConversation->ifConnectSuccess() == 0);		// �ȴ������ӳɹ�
	while (true)
	{
		if (aConversation->ifReceiveMessage() == 1)					// ������Ϣ����д���
		{
			/* �û����б�д���ݴ������ begin */
			cout << aConversation->ClientAddress + ":  " << aConversation->receiveMessage() << endl;
			/* �û����б�д���ݴ������ end */
		}
	}
}

// ��̬����Ự������߳�
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
		new thread(conversationProcess, aConversation),
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


