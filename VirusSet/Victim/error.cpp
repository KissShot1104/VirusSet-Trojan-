#include"main.h"

VOID SocketError(const TCHAR* msg)
{

	LPWSTR err = nullptr;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		err,
		NULL,
		NULL);

	//wcout << msg << " : " << err << endl;

	LocalFree(err);
}

VOID SystemError(const TCHAR* msg)
{

	LPWSTR err = nullptr;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		err,
		NULL,
		NULL);

	//wcout << msg << " : " << err << endl;

	LocalFree(err);
}