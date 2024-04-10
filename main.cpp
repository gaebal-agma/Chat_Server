#include <stdio.h>
#include "ChatServer.h""


int main()
{
	printf("==========chating server start===========\n");
	ChatServer server(54000); // 서버 포트를 54000으로 설정합니다.
	server.start();

	return 0;

}