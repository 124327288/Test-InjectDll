// InjectUI.cpp : 定义应用程序的类行为。
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


// CInjectUIApp 构造

CInjectUIApp::CInjectUIApp()
{
    // TODO: 在此处添加构造代码，
    // 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CInjectUIApp 对象

CInjectUIApp theApp;


// CInjectUIApp 初始化

BOOL CInjectUIApp::InitInstance()
{
    // 如果一个运行在 Windows XP 上的应用程序清单指定要
    // 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
    //则需要 InitCommonControlsEx()。否则，将无法创建窗口。
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // 将它设置为包括所有要在应用程序中使用的
    // 公共控件类。
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    AfxEnableControlContainer();

    // 标准初始化
    // 如果未使用这些功能并希望减小
    // 最终可执行文件的大小，则应移除下列
    // 不需要的特定初始化例程
    // 更改用于存储设置的注册表项
    // TODO: 应适当修改该字符串，
    // 例如修改为公司或组织名
    SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

    CInjectUIDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: 在此处放置处理何时用“确定”来关闭
        //  对话框的代码
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: 在此放置处理何时用“取消”来关闭
        //  对话框的代码
    }

    // 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
    //  而不是启动应用程序的消息泵。
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

//<By张方雪> 调用
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