#include"PacketSignal.h"

SOCKET packet_csock;
PacketSignal::PacketSignal(QWidget* parent) : QDialog(parent), ui(new Ui::PC_Dialog)
{

	ui->setupUi(this);

	
	SOCKET ssock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN saddr = { 0, };
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(7009);
	bind(ssock, (LPSOCKADDR)&saddr, sizeof(saddr));
	listen(ssock, 64);
	packet_csock = PacketSignalAccept(ssock);


	connect(ui->FTP20_BLOCK, SIGNAL(clicked()), this, SLOT(Ftp20Block()));
	connect(ui->FTP21_BLOCK, SIGNAL(clicked()), this, SLOT(Ftp21Block()));
	connect(ui->HTTP_BLOCK, SIGNAL(clicked()), this, SLOT(HttpBlock()));
	connect(ui->HTTPS_BLOCK, SIGNAL(clicked()), this, SLOT(HttpsBlock()));
	connect(ui->IMAP_BLOCK, SIGNAL(clicked()), this, SLOT(ImapBlock()));
	connect(ui->NETBIOS_BLOCK, SIGNAL(clicked()), this, SLOT(NetBiosBlock()));
	connect(ui->WINDOWS_REMOTE_BLOCK, SIGNAL(clicked()), this, SLOT(WindowsRemoteBlock()));
	connect(ui->SNMP_BLOCK, SIGNAL(clicked()), this, SLOT(SnmpBlock()));
	connect(ui->SSH_BLOCK, SIGNAL(clicked()), this, SLOT(SshBlock()));
	connect(ui->TCP_ALL_BLOCK, SIGNAL(clicked()), this, SLOT(TcpBlock()));
	connect(ui->UDP_ALL_BLOCK, SIGNAL(clicked()), this, SLOT(UdpBlock()));
	

}

void PacketSignal::Ftp20Block() 
{

	if (ui->FTP20_BLOCK->isChecked()) {
		int sig = 1;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
	else 
	{
		int sig = 101;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
}

void PacketSignal::Ftp21Block() 
{
	if (ui->FTP21_BLOCK->isChecked()) {


		int sig = 2;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
	else 
	{
		int sig = 102;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
}
void PacketSignal::HttpBlock() 
{
	if (ui->HTTP_BLOCK->isChecked()) {
		int sig = 3;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
	else 
	{
		int sig = 103;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
}
void PacketSignal::HttpsBlock()
{
	if (ui->HTTPS_BLOCK->isChecked()) {
		int sig = 4;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
	else 
	{
		int sig = 104;
		send(packet_csock, (char*)&sig, 4, NULL);
	}

}

void PacketSignal::ImapBlock() 
{
	if (ui->IMAP_BLOCK->isChecked()) {
		int sig = 5;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
	else
	{
		int sig = 105;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
	
}

void PacketSignal::NetBiosBlock() 
{
	if (ui->NETBIOS_BLOCK->isChecked()) {
		int sig = 6;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
	else 
	{
		int sig = 106;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
}

void PacketSignal::WindowsRemoteBlock() 
{
	if (ui->WINDOWS_REMOTE_BLOCK->isChecked()) {
		int sig = 7;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
	else 
	{
		int sig = 107;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
}

void PacketSignal::SnmpBlock() 
{
	if (ui->SNMP_BLOCK->isChecked()) {
		int sig = 8;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
	else 
	{
		int sig = 108;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
}

void PacketSignal::SshBlock() 
{

	if (ui->SSH_BLOCK->isChecked()) {
		int sig = 9;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
	else 
	{
		int sig = 109;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
}

void PacketSignal::TcpBlock() 
{
	if (ui->TCP_ALL_BLOCK->isChecked()) {
		int sig = 10;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
	else 
	{
		int sig = 110;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
}

void PacketSignal::UdpBlock() 
{
	if (ui->UDP_ALL_BLOCK->isChecked()) {
		int sig = 11;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
	else
	{
		int sig = 111;
		send(packet_csock, (char*)&sig, 4, NULL);
	}
}
SOCKET PacketSignalAccept(SOCKET ssock)
{

	return accept(ssock, NULL, NULL);

}