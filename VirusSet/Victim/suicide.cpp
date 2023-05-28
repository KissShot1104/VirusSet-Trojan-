#include"main.h"


void Suicide() 
{
	CString victim{ L"victim.exe" };
	DWORD dwcurrentprocessid = GetCurrentProcessId();
	
	HANDLE prosnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (prosnap == INVALID_HANDLE_VALUE) 
	{
		return;
	}

	PROCESSENTRY32 pe32;
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	
	if (!Process32First(prosnap, &pe32)) 
	{
		wcout << L"Process32First Error!" << endl;
		CloseHandle(prosnap);
		return;
	
	}
	do 
	{

		if (pe32.th32ProcessID != dwcurrentprocessid && !victim.CompareNoCase(pe32.szExeFile)) 
		{

			HANDLE hpro = OpenProcess(PROCESS_ALL_ACCESS, NULL, pe32.th32ProcessID);
			DWORD dwcode = 0;
			TerminateProcess(hpro, 0);
			CloseHandle(hpro);
			CloseHandle(prosnap);
			prosnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			ZeroMemory(&pe32, sizeof(PROCESSENTRY32));

			pe32.dwSize = sizeof(PROCESSENTRY32);
			
			Process32First(prosnap, &pe32);
			continue;
		}
		

	} while (Process32Next(prosnap, &pe32));
	

}