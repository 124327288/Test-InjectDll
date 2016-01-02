// InjectDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <tchar.h>
#include "detours.h"
#include "CrashActiveXCtl.h"
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;
#define  SleepTime 50

#define WM_CRASH WM_USER + 0x010


//#pragma comment(lib,"detours.lib")
//
//#ifdef _MANAGED
//#pragma managed(push, off)
//#endif
/*
//��������
BOOL SetDetoursHook();
BOOL UnSetDetoursHook();

*/
/*
	�����Ƕ�ҪHook��API������������
		���޸ĵĺ���:
			Replace_CreateProcessW	
			Replace_CreateProcessA
		ԭʼ����:
			Real_CreateProcessW
			Real_CreateProcessA
*/
// ---------------------------------------------------------------------------
/*
DETOUR_TRAMPOLINE( BOOL WINAPI Real_CreateProcessW( LPCWSTR lpApplicationName,
				  LPWSTR lpCommandLine,
				  LPSECURITY_ATTRIBUTES lpProcessAttributes,
				  LPSECURITY_ATTRIBUTES lpThreadAttributes,
				  BOOL bInheritHandles,
				  DWORD dwCreationFlags,
				  LPVOID lpEnvironment,
				  LPCWSTR lpCurrentDirectory,
				  LPSTARTUPINFOW lpStartupInfo,
				  LPPROCESS_INFORMATION lpProcessInformation),
				  CreateProcessW);

DETOUR_TRAMPOLINE( BOOL WINAPI Real_CreateProcessA(	LPCTSTR lpApplicationName,
				  LPTSTR lpCommandLine,
				  LPSECURITY_ATTRIBUTES lpProcessAttributes,
				  LPSECURITY_ATTRIBUTES lpThreadAttributes,
				  BOOL bInheritHandles,
				  DWORD dwCreationFlags,
				  LPVOID lpEnvironment,
				  LPCTSTR lpCurrentDirectory,
				  LPSTARTUPINFO lpStartupInfo,
				  LPPROCESS_INFORMATION lpProcessInformation),
				  CreateProcessA);

// ---------------------------------------------------------------------------

BOOL WINAPI Replace_CreateProcessW(
								   LPCWSTR lpApplicationName,
								   LPWSTR lpCommandLine,
								   LPSECURITY_ATTRIBUTES lpProcessAttributes,
								   LPSECURITY_ATTRIBUTES lpThreadAttributes,
								   BOOL bInheritHandles,
								   DWORD dwCreationFlags,
								   LPVOID lpEnvironment,
								   LPCWSTR lpCurrentDirectory,
								   LPSTARTUPINFOW lpStartupInfo,
								   LPPROCESS_INFORMATION lpProcessInformation
								   )
{
	BOOL res = 0;
	__try 
	{	
		MessageBoxW( NULL, lpApplicationName, _T("���سɹ�!"), MB_OK );
		res = Real_CreateProcessW(	lpApplicationName,
			lpCommandLine,
			lpProcessAttributes,
			lpThreadAttributes,
			bInheritHandles,
			dwCreationFlags,
			lpEnvironment,
			lpCurrentDirectory,
			lpStartupInfo,
			lpProcessInformation);
	} 
	__finally 
	{
	};
	return res;
}

BOOL Replace_CreateProcessA(
							LPCTSTR lpApplicationName,
							LPTSTR lpCommandLine,
							LPSECURITY_ATTRIBUTES lpProcessAttributes,
							LPSECURITY_ATTRIBUTES lpThreadAttributes,
							BOOL bInheritHandles,
							DWORD dwCreationFlags,
							LPVOID lpEnvironment,
							LPCTSTR lpCurrentDirectory,
							LPSTARTUPINFO lpStartupInfo,
							LPPROCESS_INFORMATION lpProcessInformation
							)
{
	BOOL res = 0;
	__try 
	{	
		MessageBoxA( NULL, (LPCSTR)lpApplicationName, "���سɹ�!", MB_OK );
		res = Real_CreateProcessA(	lpApplicationName,
			lpCommandLine,
			lpProcessAttributes,
			lpThreadAttributes,
			bInheritHandles,
			dwCreationFlags,
			lpEnvironment,
			lpCurrentDirectory,
			lpStartupInfo,
			lpProcessInformation);
	} 
	__finally 
	{
	};
	return res;
}
*/
int strToNum(TCHAR* str)
{
	TCHAR constStr[9][10]={{_T("�±�Խ��")},{_T("��ѭ��")},
	{_T("��0")},{_T("��Ч��ַ")},
	{_T("��ָ��")},{_T("�Ƿ�����")},
	{_T("д�ں�")},{_T("д��ָ��")},{_T("д�����ַҳ")}};
	for (int i=0;i<9;i++)
	{
		if (_tcscmp(str,constStr[i])==0)
			return (30+i);
	}
	return 0;
}


//DWORD WINAPI InjectDllThread(void * lParam)
//{
//	OutputDebugString(L"Start message loop.");
//	//m_dwThreadID = GetCurrentThreadId();
//	MSG msg;
//	::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
//
//	while (::GetMessage(&msg, NULL, 0, 0))
//	{
//		if (msg.message == WM_CRASH)
//		{
//			OutputDebugString(L"Receive Crash message.");
//			TestArrayOutOfIndex();
//		}
//	}
//	return 1;
//}

DWORD WINAPI InjectDllThread(void * lParam) //���붨�����������ʽ��������ò��ɹ�������˵��
{
	

	Sleep(SleepTime);
	wifstream iTempF;

	TCHAR strPath[MAX_PATH];
	GetTempPath(MAX_PATH, strPath);
	_tcscat_s(strPath, _T("\\crashTemp.txt"));

	iTempF.open(strPath, ios_base::in);
	if (!iTempF)
	{
		MessageBox( NULL , _T("Temp�ļ���ʧ�ܣ�") , _T("��ʾ") , MB_OK );
		return FALSE;
	}

	//MessageBox( NULL , _T("��Temp�ļ���") , _T("��ʾ") , MB_OK );

	TCHAR crashcaseStr0[50]=_T("");
	iTempF.getline(crashcaseStr0,50);
	iTempF.close();
	iTempF.clear();

	TCHAR crashcaseStr[20]=_T("");
	int i = 2;
	int l = 0;

	while (crashcaseStr0[i]!='#')  //��ϳ�����
	{
		crashcaseStr[l] = crashcaseStr0[i + 1];
		crashcaseStr[l] = crashcaseStr[l] * 256 +crashcaseStr0[i];
		i = i + 2;
		l++;
	}
	crashcaseStr[l] = '\0';

	int crashcase = strToNum(crashcaseStr);

	switch (crashcase)
	{
	case 30:
		TestArrayOutOfIndex();
		break;
	case 31:
		TestBusyLoop();
		break;
	case 32:
		TestDivByZero();
		break;
	case 33:
		TestInvalidMemAddr();
		break;
	case 34:
		TestNullPointer();
		break;
	case 35:
		TestAccessViolation();
		break;
	case 36:
		TestWriteCore();
		break;
	case 37:
		//TestModifyVMAttr();
		TestWriteNULLPointer();
		break;
	case 38:
		TestModifyVMAttr();
	default:
		TestModifyVMAttr();
		break;
	}
	return 1;
}




BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		{
			DWORD dwThreadID;
			CreateThread(NULL,0,InjectDllThread,NULL,0,&dwThreadID);
			OutputDebugString(_T("DLL_PROCESS_ATTACH Sucessed!"));
		}
		break;
	case DLL_THREAD_ATTACH:
		{
			OutputDebugString(_T("DLL_THREAD_ATTACH Sucessed!"));
		}
		break;
	case DLL_THREAD_DETACH:
		{
			OutputDebugString(_T("DLL_THREAD_DETACH Sucessed!"));
		}
		break;
	case DLL_PROCESS_DETACH:
		{
			OutputDebugString(_T("DLL_PROCESS_DETACH Sucessed!"));
			//MessageBox( NULL , _T("dll��ж��") , _T("��ʾ") , MB_OK );
			//UnSetDetoursHook();
		}
		break;
	}
	return TRUE;
}
/*
BOOL SetDetoursHook()
{
	DetourFunctionWithTrampoline((PBYTE)Real_CreateProcessW, (PBYTE)Replace_CreateProcessW);
	DetourFunctionWithTrampoline((PBYTE)Real_CreateProcessA, (PBYTE)Replace_CreateProcessA);
	return TRUE;
}

BOOL UnSetDetoursHook()
{
	DetourRemove( (PBYTE)Real_CreateProcessW,(PBYTE)Replace_CreateProcessW);
	DetourRemove( (PBYTE)Real_CreateProcessA,(PBYTE)Replace_CreateProcessA);
	return TRUE;
}
*/
#ifdef _MANAGED
#pragma managed(pop)
#endif

