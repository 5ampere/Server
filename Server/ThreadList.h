#ifndef __THREADLIST_H
#define __THREADLIST_H

#include "socket.h"
#include <thread>
#include <iostream>


struct Node {
	Conversation* aConversation;	// 会话窗口
	thread* tConversation;			// 会话接收数据线程
	thread* tProcess;				// 会话数据处理线程
	Node *  next;
};

class Linklist {
private:
	Node * Head;
	mySocket fatherSocket;
public:
	Linklist(mySocket &);
	void Insert();				   //插入函数
	void Delete(int i);             //删除表中元素
	void listProcess();			   //动态处理会话链表
};

#endif /*__THREADLIST_H*/
