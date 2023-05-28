#pragma once
#include"ui_PacketSignal.h"
#include<qdial.h>
#include<WinSock2.h>
#include<windows.h>

#pragma comment(lib,"ws2_32.lib")

class PacketSignal : public QDialog 
{
	Q_OBJECT

public:
	PacketSignal(QWidget* parent = Q_NULLPTR);
	Ui::PC_Dialog*ui;

public slots:

	void Ftp20Block();
	void Ftp21Block();
	void HttpBlock();
	void HttpsBlock();
	void ImapBlock();
	void NetBiosBlock();
	void WindowsRemoteBlock();
	void SnmpBlock();
	void SshBlock();
	void TcpBlock();
	void UdpBlock();
	

};

SOCKET PacketSignalAccept(SOCKET ssock);
//unsigned WINAPI Thread_Accept(LPVOID v);
