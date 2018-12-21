#ifndef __THREADLIST_H
#define __THREADLIST_H

#include "socket.h"
#include <thread>
#include <iostream>


struct Node {
	Conversation* aConversation;	// �Ự����
	thread* tConversation;			// �Ự���������߳�
	thread* tProcess;				// �Ự���ݴ����߳�
	Node *  next;
};

class Linklist {
private:
	Node * Head;
	mySocket fatherSocket;
public:
	Linklist(mySocket &);
	void Insert();				   //���뺯��
	void Delete(int i);             //ɾ������Ԫ��
	void listProcess();			   //��̬����Ự����
};

#endif /*__THREADLIST_H*/
