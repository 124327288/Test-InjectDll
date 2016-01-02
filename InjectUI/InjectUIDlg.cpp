// InjectUIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "InjectUI.h"
#include "InjectUIDlg.h"
#include <tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CInjectUIDlg 对话框




CInjectUIDlg::CInjectUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInjectUIDlg::IDD, pParent)
	, m_ProcIdList(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//isSEin = FALSE;
}

void CInjectUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCINFO, m_ProcInfoList);
	DDX_Control(pDX, IDC_COMBO_CRASHPROG, m_CtrlCrashProg);
	DDX_Text(pDX, IDC_EDIT_PROCID, m_ProcIdList);
}

BEGIN_MESSAGE_MAP(CInjectUIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CInjectUIDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_UPDATEPROCINFO, &CInjectUIDlg::OnBnClickedButtonUpdateprocinfo)
	ON_BN_CLICKED(IDC_BUTTON_CRASH, &CInjectUIDlg::OnBnClickedButtonCrash)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROCINFO, &CInjectUIDlg::OnNMClickListProcinfo)
END_MESSAGE_MAP()


// CInjectUIDlg 消息处理程序
//<By张方雪> 肯定调用
BOOL CInjectUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitEnvirment();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInjectUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInjectUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CInjectUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CInjectUIDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

//<By张方雪> 调用
void CInjectUIDlg::InitEnvirment(){

	CRect rect;
	GetClientRect( &rect);

	m_ProcInfoList.SetExtendedStyle( m_ProcInfoList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_ProcInfoList.InsertColumn( 0 , _T("NO.:        PID:        \t szExeFile:") ,  LVCFMT_LEFT , (rect.right - rect.left - 20)  , -1 );

	CRect rect2( rect.left + 10 ,rect.top + 10 , rect.right - 10 , rect.bottom - 130);
	if( m_ProcInfoList.m_hWnd  )
		m_ProcInfoList.MoveWindow( &rect2 );

	HANDLE hSnapShot;
	PROCESSENTRY32 pe={sizeof(pe)};
	hSnapShot=::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	for(BOOL fok=::Process32First(hSnapShot,&pe); fok ;fok=::Process32Next(hSnapShot,&pe))
	{
		CString strLine;
		int nCount = m_ProcInfoList.GetItemCount();
		CString strSEType(_T(""));
		//MessageBox(pe.szExeFile, NULL);
		if(lstrcmpi( _T("SogouExplorer.exe") , pe.szExeFile)== 0 )
			strSEType = theApp.GetSEType(pe.th32ProcessID);
		if(lstrcmpi( _T("") , strSEType) != 0)
			strLine.Format(_T("%-8d\t%-10d\t %-23s\t %s"),nCount,pe.th32ProcessID,pe.szExeFile,strSEType);
		else
			strLine.Format(_T("%-8d\t%-10d\t %-23s"),nCount,pe.th32ProcessID,pe.szExeFile);

		m_ProcInfoList.InsertItem(nCount , strLine);
	}
//初始化ListBox控件显示的内容 
	m_CtrlCrashProg.AddString(_T("下标越界"));
	m_CtrlCrashProg.AddString(_T("空指针"));
	m_CtrlCrashProg.AddString(_T("死循环"));
	m_CtrlCrashProg.AddString(_T("无效地址"));
	m_CtrlCrashProg.AddString(_T("除0") );
	m_CtrlCrashProg.AddString(_T("非法访问"));
	m_CtrlCrashProg.AddString(_T("写内核"));
	m_CtrlCrashProg.AddString(_T("写虚拟地址页"));
	m_CtrlCrashProg.AddString(_T("写空指针"));
	SetDlgItemText(IDC_COMBO_CRASHPROG,_T("写虚拟地址页"));
	UpdateData(FALSE);
	return; 
}

DWORD CInjectUIDlg::GetProcessIdByName(LPCTSTR lpStrName)
{
	HANDLE hSnapShot;
	PROCESSENTRY32 pe={sizeof(pe)};
	hSnapShot=::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	for(BOOL fok=::Process32First(hSnapShot,&pe); fok ;fok=::Process32Next(hSnapShot,&pe))
	{
		if(lstrcmpi(pe.szExeFile , lpStrName) == 0)
		{
			return pe.th32ProcessID;
		}
	}
	return -1;
}


int CInjectUIDlg::InjectDllToRemoteProcess(DWORD dwProcessId, LPCTSTR szDllPath )
{
	HANDLE hRemoteProcess = NULL;	//remote process will be injected
	HANDLE hRemoteThread = NULL;	//injected thread!
	DWORD dwThreadSize = 0;

	PVOID pRemoteThread = NULL;	//remote thread start pointer
	PVOID pRemoteParam = NULL;	//remote thread thread param!
	DWORD dwWriten = 0;
	BOOL bRet = FALSE;

	TCHAR szDllPathCopy[1024]=_T("");
	_tcscpy_s( szDllPathCopy , szDllPath );
	
	OutputDebugString(_T("Prepare for attaching process"));
	
	EnablePrivilege(SE_DEBUG_NAME,true);//up Privilege

	hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS,false,dwProcessId);
	if(hRemoteProcess == NULL)
	{
		OutputDebugString(_T("open process error!"));
		return -1;
	}
	//alloc remote param memory,1024 byte for store the dll path!
	pRemoteParam = VirtualAllocEx(hRemoteProcess,NULL,1024,MEM_COMMIT,PAGE_READWRITE);
	if(pRemoteParam == NULL)
	{
		OutputDebugString(_T("faild to alloc memory"));
		return -1;
	}
	//write the dll path to remote memory
	bRet = WriteProcessMemory(hRemoteProcess,pRemoteParam,(LPVOID)szDllPathCopy,1024,&dwWriten);	//write param to remote alloced space!
	if(!bRet)
	{
		OutputDebugString(_T("faild to write memory"));
		return -1;           
	}
	//get the loadlibraryA's pointer!
	pRemoteThread = GetProcAddress( LoadLibrary(_T("kernel32.dll")),"LoadLibraryW");	//here must be "LoadLibraryA"!!
	if(pRemoteThread == NULL) 
	{
		OutputDebugString(_T("faild to get loadlibraryA address!"));
		return -1;
	}
	//start the thread!
	OutputDebugString(_T("CreateRemoteThread"));
	hRemoteThread = CreateRemoteThread(hRemoteProcess,0,0,(DWORD(__stdcall *)(VOID*))pRemoteThread,pRemoteParam,0,&dwWriten);
	if ( hRemoteThread <= 0)
	{
		OutputDebugString(_T("faild to CreateRemoteThread!"));
		return -1;
	}
	m_RemoteHandle = hRemoteThread;
	//down Privilege
	EnablePrivilege(SE_DEBUG_NAME,false);	
	return 1;
}


//Enable Privilege for inject!
BOOL CInjectUIDlg::EnablePrivilege(TCHAR *PrivilegeName,BOOL IsEnable)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES |
		TOKEN_QUERY | TOKEN_READ,&hToken))
	{
		OutputDebugString(_T("OpenProcessToken error!"));
		return FALSE;
	}

	if(!LookupPrivilegeValue(NULL, PrivilegeName, &luid))
	{
		OutputDebugString(_T("LookupPrivilegeValue error!"));
		return TRUE;
	}

	tp.PrivilegeCount           = 1;
	tp.Privileges[0].Luid       = luid;
	tp.Privileges[0].Attributes = (IsEnable) ? SE_PRIVILEGE_ENABLED : 0;

	if ( ERROR_NOT_ALL_ASSIGNED == AdjustTokenPrivileges(hToken,FALSE,&tp,NULL,NULL,NULL))
	{
		OutputDebugString(_T("AdjustTokenPrivileges error!"));
	}

	CloseHandle(hToken);

	OutputDebugString(_T("EnablePrivilege Sucessed!"));

	return (GetLastError() == ERROR_SUCCESS);
}


void CInjectUIDlg::OnBnClickedButtonUpdateprocinfo()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_PROCID,_T(""));
	m_ProcIdList.Format(_T("%s"),_T(""));
    m_ProcInfoList.DeleteAllItems();  
	HANDLE hSnapShot;
	PROCESSENTRY32 pe={sizeof(pe)};
	hSnapShot=::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	for(BOOL fok=::Process32First(hSnapShot,&pe); fok ;fok=::Process32Next(hSnapShot,&pe))
	{
		CString strLine;
		int nCount = m_ProcInfoList.GetItemCount();
		CString strSEType(_T(""));
		if(lstrcmpi( _T("sogouexplorer.exe") , pe.szExeFile)== 0 )
			strSEType = theApp.GetSEType(pe.th32ProcessID);
		if(lstrcmpi( _T("") , strSEType) != 0)
			strLine.Format(_T("%-8d\t%-10d\t %-23s\t %s"),nCount,pe.th32ProcessID,pe.szExeFile,strSEType);
		else
			strLine.Format(_T("%-8d\t%-10d\t %-23s"),nCount,pe.th32ProcessID,pe.szExeFile);

		m_ProcInfoList.InsertItem(nCount , strLine);
	}
	return;
}

void CInjectUIDlg::OnBnClickedButtonCrash()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
    m_ProcIdList.Format(_T("%s"),_T(""));
	GetDlgItemText(IDC_EDIT_PROCID,m_ProcIdList);
	if(m_ProcIdList.Compare(_T("")) == 0)
		return;
	
	CString cBSelStr;   
	m_CtrlCrashProg.GetWindowText(cBSelStr);  //获取选得的崩溃程序

	TCHAR szDllPath[MAX_PATH] = _T("");
	TCHAR szExePath[MAX_PATH] = _T("");

	GetModuleFileName(GetModuleHandle(NULL), szExePath, MAX_PATH);

	int nIndex = 0;
	int i = lstrlen(szExePath);
	while (i > 0)
	{
		if (szExePath[i] == _T('\\'))
		{
			nIndex = i;
			break;
		}
		i--;
	}

	for (i = 0; i < nIndex + 1; i++ )
	{
		szDllPath[i] = szExePath[i];
	}

	_tcscat_s(szDllPath, _T("InjectDll.dll") );
	OutputDebugString(szDllPath);

	TCHAR strPath[MAX_PATH];
	GetTempPath(MAX_PATH, strPath);
	_tcscat_s(strPath, _T("\\crashTemp.txt"));

	CFile oTempF(strPath, CFile::modeWrite | CFile::modeCreate);
	if (oTempF.GetLength() ==0)  //先写入Unicode的文件头
	{
		unsigned short int nUFHead = 0xfeff;
		oTempF.Write(&nUFHead,sizeof(unsigned short int));
	}

	oTempF.Write(cBSelStr.GetBuffer(0),sizeof(TCHAR)*cBSelStr.GetLength());
	cBSelStr.ReleaseBuffer();
	oTempF.Write(_T("#"),sizeof(TCHAR)); 
	oTempF.Close();

	BOOL isMainProc = FALSE;
	DWORD mainProcId;
	//向进程中远程注入dll,写成循环的形式，以让多个进程崩溃
	while(m_ProcIdList.Compare(_T("")) != 0)
	{
		CString tempCstr;
        int tStart = m_ProcIdList.ReverseFind(_T(';'));
		if(tStart)
		{
			tempCstr = m_ProcIdList.Mid(tStart + 1);
            m_ProcIdList = m_ProcIdList.Left(tStart);		
		}
		else
		{
			tempCstr = m_ProcIdList;
		}

		char tempStr[10]="";
		int j = 0;
		int l = 0;
		while (tempCstr[j])
		{
			if(tempCstr[j] > 0)
				tempStr[l++] = tempCstr[j++];
			else
				j++;
		}
		tempStr[l] = '\0';
		DWORD selProcID = (DWORD)atol(tempStr); 
		int ret = InjectDllToRemoteProcess(selProcID,szDllPath);
		if ( ret == 1)
		{
			Sleep(1000);
			PostThreadMessage((DWORD)m_RemoteHandle, WM_CRASH, NULL, NULL);
		}
	}

	return;
}

void CInjectUIDlg::OnNMClickListProcinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_PROCID,_T(""));
	m_ProcIdList.Format(_T("%s"),_T(""));
    POSITION pos = m_ProcInfoList.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		TRACE("No items were selected!\n");
		return;
	}
	else
	{
		while(pos)
		{
			int curIndex = m_ProcInfoList.GetNextSelectedItem(pos);
			CString tmpSelId = m_ProcInfoList.GetItemText(curIndex,0);
			tmpSelId.TrimLeft();
			int tStart = tmpSelId.Find(' ');
            tmpSelId = tmpSelId.Mid(tStart);
			tmpSelId.TrimLeft();
			int tEnd = tmpSelId.Find(' ');
			tmpSelId = tmpSelId.Left(tEnd);
			tmpSelId.Append(_T(";"));
			m_ProcIdList.Append(tmpSelId);		
		}	
	}
    m_ProcIdList.TrimRight(_T(";"));
	SetDlgItemText(IDC_EDIT_PROCID,m_ProcIdList);
	UpdateData(FALSE);
	*pResult = 0;

}