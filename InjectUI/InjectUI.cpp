// InjectUI.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "InjectUI.h"
#include "InjectUIDlg.h"
#include "Objbase.h"
#import "Getpara.tlb"
#include <comutil.h>
#pragma comment(lib,"comsuppw.lib")
using namespace GetPara;
#include <strsafe.h>
#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>

# pragma comment(lib, "wbemuuid.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInjectUIApp


BEGIN_MESSAGE_MAP(CInjectUIApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CInjectUIApp ����

CInjectUIApp::CInjectUIApp()
{
    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CInjectUIApp ����

CInjectUIApp theApp;


// CInjectUIApp ��ʼ��

BOOL CInjectUIApp::InitInstance()
{
    // ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
    // ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
    //����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
    // �����ؼ��ࡣ
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    AfxEnableControlContainer();

    // ��׼��ʼ��
    // ���δʹ����Щ���ܲ�ϣ����С
    // ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
    // ����Ҫ���ض���ʼ������
    // �������ڴ洢���õ�ע�����
    // TODO: Ӧ�ʵ��޸ĸ��ַ�����
    // �����޸�Ϊ��˾����֯��
    SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

    CInjectUIDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
        //  �Ի���Ĵ���
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
        //  �Ի���Ĵ���
    }

    // ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
    //  ����������Ӧ�ó������Ϣ�á�
    return FALSE;
}


CString  CInjectUIApp::GetProcCommandline(DWORD dwPID)
{
	HRESULT hres;

	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------

	hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (FAILED(hres))
	{
		return _T("");                  // Program has failed.
	}

	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------
	// Note: If you are using Windows 2000, you need to specify -
	// the default authentication credentials for a user by using
	// a SOLE_AUTHENTICATION_LIST structure in the pAuthList ----
	// parameter of CoInitializeSecurity ------------------------

	hres =  CoInitializeSecurity(
		NULL, 
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
		);


	if (FAILED(hres))
	{
		//cout << "Failed to initialize security. Error code = 0x" 
		//	<< hex << hres << endl;
		CoUninitialize();
		return _T("");                    // Program has failed.
	}

	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,             
		0, 
		CLSCTX_INPROC_SERVER, 
		IID_IWbemLocator, (LPVOID *) &pLoc);

	if (FAILED(hres))
	{
		//cout << "Failed to create IWbemLocator object."
		//	<< " Err code = 0x"
		//	<< hex << hres << endl;
		CoUninitialize();
		return _T("");                 // Program has failed.
	}

	// Step 4: -----------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method

	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (for example, Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
		);

	if (FAILED(hres))
	{
		//cout << "Could not connect. Error code = 0x" 
		//	<< hex << hres << endl;
		pLoc->Release();     
		CoUninitialize();
		return _T("");                // Program has failed.
	}

	cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;


	// Step 5: --------------------------------------------------
	// Set security levels on the proxy -------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);

	if (FAILED(hres))
	{
		//cout << "Could not set proxy blanket. Error code = 0x" 
		//	<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();     
		CoUninitialize();
		return _T("");               // Program has failed.
	}

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"), 
		bstr_t("SELECT * FROM Win32_Process"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		cout << "Query for operating system name failed."
			<< " Error code = 0x" 
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return _T("");               // Program has failed.
	}

	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	IWbemClassObject *pclsObj;
	ULONG uReturn = 0;

	CString strAns;
	bool bingo = false;

	while (pEnumerator && !bingo)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
			&pclsObj, &uReturn);

		if(0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;

		// Get the value of the Name property
		
		//wcout << "CommandLine : " << vtProp.bstrVal << endl;
		//if(!FAILED(hr))
		//{
		//	if ((vtProp.vt==VT_NULL) || (vtProp.vt==VT_EMPTY))
		//		wcout << "CommandLine : " << ((vtProp.vt==VT_NULL) ? "NULL" : "EMPTY") << endl;
		//	else
		//		if ((vtProp.vt & VT_ARRAY))
		//			wcout << "CommandLine : " << "Array types not supported (yet)" << endl;
		//		else
		//			wcout << "CommandLine : " << vtProp.bstrVal << endl;
		//}
		//VariantClear(&vtProp);

		hr = pclsObj->Get(L"ProcessId", 0, &vtProp, 0, 0);// Uint32
		if (!FAILED(hr))
		{
			if(vtProp.vt != VT_NULL && vtProp.vt != VT_EMPTY && !(vtProp.vt & VT_ARRAY) && vtProp.uintVal == dwPID)
			{
				VariantClear(&vtProp);
				hr = pclsObj->Get(L"CommandLine", 0, &vtProp, 0, 0);
				if(vtProp.vt != VT_NULL && vtProp.vt != VT_EMPTY && !(vtProp.vt & VT_ARRAY))
				{
					strAns = vtProp.bstrVal;
					bingo = true;
				}
			}
		}
		VariantClear(&vtProp);
	}

	// Cleanup
	// ========

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	pclsObj->Release();
	CoUninitialize();
	return strAns;
}

//<By�ŷ�ѩ> ����
CString CInjectUIApp::GetSEType(DWORD dwPID)
{
    CString strSEType(_T(""));
    TCHAR commandline[MAXLENGTH];
    memset( commandline, 0 , MAXLENGTH);
    //	DWORD T = Util::GetProcCommandline(dwPID, commandline , MAXLENGTH );
    //	TRACE(_T("GetProcCommandline Error: %d"),T);
    CString csCommandline = GetProcCommandline(dwPID);

    int index = csCommandline.Find(_T("-!"));
    if(index != -1)
    {
        CString probSEPara = csCommandline.Mid(index + 3, 1);
        if(probSEPara.Compare(_T("0")) == 0)
            strSEType.Format(_T("%s"), _T("Trident"));
        else if(probSEPara.Compare(_T("1")) == 0)
            strSEType.Format(_T("%s"), _T("WebKit"));
        else if(probSEPara.Compare(_T("2")) == 0)
            strSEType.Format(_T("%s"), _T("WebKitNetwork"));
        else if(probSEPara.Compare(_T("3")) == 0)
            strSEType.Format(_T("%s"), _T("SnapShot"));
        else if(probSEPara.Compare(_T("4")) == 0)
            strSEType.Format(_T("%s"), _T("ImportCookies"));
        else
            strSEType.Format(_T("%s"), _T(""));
    }
    else
        strSEType.Format(_T("%s"), _T(""));
	
	//MessageBox(NULL, strSEType, NULL, MB_OK);
    return strSEType;
}