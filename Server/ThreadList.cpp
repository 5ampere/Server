#include "ThreadList.h"

// 开启一个会话线程并不断接受消息
void conversationThread(Conversation* aConversation, mySocket* fatherSocket)
{
	aConversation->createConversation(*fatherSocket);
}

// 该线程处理会话来的数据（用户编写）
void conversationProcess(Conversation* aConversation)
{
	while (aConversation->ifConnectSuccess() == 0);		// 等待到连接成功
	while (true)
	{
		if (aConversation->ifReceiveMessage() == 1)					// 若有消息则进行处理
		{
			/* 用户自行编写数据处理代码 begin */
			cout << aConversation->ClientAddress + ":  " << aConversation->receiveMessage() << endl;
			/* 用户自行编写数据处理代码 end */
		}
	}
}

// 动态处理会话链表的线程
void listProcessThread(Linklist& myList)
{
	thread listProcess(&Linklist::listProcess, myList);
	listProcess.detach();
}

//构造函数，创建一组默认会话线程
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

// 动态增加、删除会话列表里的元素
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

//在末尾插入一组会话进程
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

//删除i处的数据
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

	cout << "会话关闭！" << endl;
}


