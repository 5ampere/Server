#include "ThreadList.h"

// 开启一个会话线程并不断接受消息
void conversationThread(Conversation* aConversation, mySocket* fatherSocket)
{
	aConversation->createConversation(*fatherSocket);
}

// 该线程处理会话来的数据（用户编写）
void conversationProcess(Conversation* aConversation, Linklist* aLinklist)
{
	while (aConversation->ifConnectSuccess() == 0);		// 等待到连接成功
	while (true)
	{
		if (aConversation->ifReceiveMessage() == 1)					// 若有消息则进行处理
		{
			/* 用户自行编写数据处理代码 begin */
			/*
			此服务器用于智能门锁的sim800模块与用户的手机移动端进行通讯
			发送数据格式 ：
				目标IP（字符串）\r\n
				数据内容（字符串）\n
			服务器根据目标IP将客户端A发送的 ‘数据内容’ 转发到 ‘目标IP’对应的客户端B
			*/
			static char times = 0;
			static string str[2];
			str[times] = aConversation->receiveMessage();
			times++;
			if (times == 2)		// 接收完一次数据包
			{
				times = 0;

				Node *temp;
				temp = aLinklist->Head;
				while (temp)		// 遍历所有会话，查找是否有目标IP
				{
					if (temp->aConversation->ClientAddress+"\r\n" == str[0])
					{
						temp->aConversation->sendMessage(str[1]);
						cout << "转发成功！" << endl;
					}
					temp = temp->next;
				}
			}
			/* 用户自行编写数据处理代码 end */
		}
	}
}

// 调用此函数，开启动态处理会话链表的线程
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
		new thread(conversationProcess, aConversation, this),
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


