#include <iostream>
#include "socket.h"
#include <thread>
#include "ThreadList.h"
void listProcessThread(Linklist& myList);
int main(int argc, char* argv[])  
{  
	mySocket aServer;
	aServer.SocketInit(8080);

	Linklist conversationList(aServer);
	listProcessThread(conversationList);

	while (true);
} 
