#include <stdio.h>
#include "ChatServer.h""


int main()
{
	printf("==========chating server start===========\n");
	ChatServer server(54000); 
	server.start();

	return 0;

}