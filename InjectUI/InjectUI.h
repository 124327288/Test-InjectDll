// InjectUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CInjectUIApp:
// �йش����ʵ�֣������ InjectUI.cpp
//

class CInjectUIApp : public CWinApp
{
public:
	CInjectUIApp();

// ��д
	public:
	virtual BOOL InitInstance();
	CString GetProcCommandline(DWORD dwPID);
	CString  GetSEType(DWORD dwPID);

// ʵ��

	DECLARE_MESSAGE_MAP()
};



extern CInjectUIApp theApp;