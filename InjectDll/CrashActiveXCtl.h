#include <windows.h> 

#define    Max_Process_Count    1024 
#define    Max_Module_Count    1024 

//ȱ��psapi.hͷ���Լ��������� 
typedef BOOL (__stdcall *ENUMPROCESSMODULES)( IN HANDLE, OUT HMODULE*, IN DWORD, OUT LPDWORD ); 
ENUMPROCESSMODULES        EnumProcessModules;

#define SIZE_OF_NT_SIGNATURE (sizeof(DWORD))

int ReturnZero()
{
	return 0;
}

void TestWriteNULLPointer()
{
	int* p = 0;
	*p = 20;
}
int RecusiveInfinite()
{
	int a = 2;
	int b = 3 - 1;
	//int *c = (int* )malloc(10000 * sizeof(int));
	int c[10000];
	if (a == b)
		return RecusiveInfinite();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCrashActiveXCtrl message handlers

void BusyLoop() 
{
	// TODO: Add your dispatch handler code here
	RecusiveInfinite();
}

void TestArrayOutOfIndex() 
{
	// TODO: Add your dispatch handler code here
	// MessageBox(NULL,_T("hello"),0);// 
	int a[40];
	//MessageBox( NULL , _T("�±�Խ��") , _T("��ʾ") , MB_SETFOREGROUND );
	int *p = a;
	int Times = 100000;
	while(Times){
		*p = 1;
		p = p - 1;
		Times--;	
	}

	//for (int i = 0; i< 1000000;i++)
	//{
	//	a[i] = 0;
	//}
}


void TestNullPointer() 
{
	// TODO: Add your dispatch handler code here
	//MessageBox( NULL , _T("��ָ��") , _T("��ʾ") , MB_SETFOREGROUND );
/*	int *p = 0;
	*p = 1;*/
	int *p = new int;
	delete p;
	delete p;
	p = NULL;
}

void TestBusyLoop() 
{
	// TODO: Add your dispatch handler code here
	int i = 1;
	//MessageBox( NULL , _T("������ѭ��") , _T("��ʾ") , MB_SETFOREGROUND );
	//while (true)
	//{
	//	i = i * 1;
	//}
	//i++;
	BusyLoop();

}

void TestInvalidMemAddr() 
{
	// TODO: Add your dispatch handler code here
	//MessageBox( NULL , _T("��Ч��ַ") , _T("��ʾ") , MB_SETFOREGROUND );
	PINT pnData = (PINT)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	for (int i =0 ; i< 10000000;i++)
	{
		pnData[i] = 1;
	}
}

void TestDivByZero() 
{
	// TODO: Add your dispatch handler code here
	/*MessageBox(NULL,_T("hello"),0);*/
	int i ;
	//MessageBox( NULL , _T("��0") , _T("��ʾ") , MB_SETFOREGROUND );
	i = 4 /ReturnZero();
}

void TestAccessViolation() 
{
	// TODO: Add your dispatch handler code here
	//MessageBox( NULL , _T("�Ƿ�����") , _T("��ʾ") , MB_SETFOREGROUND );
	PINT pnData = (PINT)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READONLY);
	for (int i =0 ; i< 10000000;i++)
	{
		pnData[i] = 1;
	}
}

void TestWriteCore() 
{
	// TODO: Add your dispatch handler code here
	//MessageBox( NULL , _T("д�ں˿ռ�") , _T("��ʾ") , MB_SETFOREGROUND );
	BYTE* bBuf = new BYTE[70000];
	DWORD dwNumBytesWritten;
	HANDLE hProcess = GetCurrentProcess();
	WriteProcessMemory(hProcess,(LPVOID)0x7FFEEFF0,bBuf,sizeof(bBuf),&dwNumBytesWritten);
}

void TestModifyVMAttr()  //�޸Ľ����������ַҳ�Ķ�д���ԣ������������ַҳ��д�������� 
{
	// TODO: Add your dispatch handler code here
	//MessageBox( NULL , _T("д�����ַҳ") , _T("��ʾ") , MB_SETFOREGROUND );

	HMODULE hModule;
	DWORD dwOffset, dwRet, cbneeded;
	IMAGE_DOS_HEADER DOSHeader;
	IMAGE_OPTIONAL_HEADER OptionalHeader;
	IMAGE_DATA_DIRECTORY DataDirectory;
	MEMORY_BASIC_INFORMATION mbi;
	DWORD dwOldProtect;
	LPVOID lpBaseAddress;
	BYTE* bBuf = new BYTE[700000];
	memset(bBuf,1,700000);

	HANDLE hProcess = GetCurrentProcess();     //PROCESS_ALL_ACCESS
	if (hProcess == NULL)
	{
		MessageBox( NULL , _T("Fail to open process!") , _T("��ʾ") , MB_OK );
		//printf("Fail to open process!");
		return;
	}

	if(!EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbneeded))
	{
		MessageBox( NULL , _T("Fail to enum process modules!") , _T("��ʾ") , MB_OK );
		return;
	}

	if (!ReadProcessMemory(hProcess, hModule, (void *)&DOSHeader, sizeof(IMAGE_DOS_HEADER), &dwRet))
	{
		MessageBox( NULL , _T("Fail to read memory in remote process! 1st") , _T("��ʾ") , MB_OK );
		return;
	}
	dwOffset = DOSHeader.e_lfanew + SIZE_OF_NT_SIGNATURE + sizeof(IMAGE_FILE_HEADER);
	if (!ReadProcessMemory(hProcess, (PBYTE)hModule + dwOffset, (void *)&OptionalHeader, sizeof(IMAGE_OPTIONAL_HEADER), &dwRet))
	{
		MessageBox( NULL , _T("Fail to read memory in remote process! 2nd") , _T("��ʾ") , MB_OK );
		return;
	}
	DataDirectory = OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

	lpBaseAddress = (PBYTE)DataDirectory.VirtualAddress;
	VirtualQueryEx(hProcess, lpBaseAddress, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
	VirtualProtectEx(hProcess, mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect);
    

	WriteProcessMemory(hProcess, lpBaseAddress, bBuf, sizeof(bBuf), &dwRet);

	VirtualProtectEx(hProcess, mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &dwOldProtect);
}