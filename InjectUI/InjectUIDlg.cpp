// InjectUIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "InjectUI.h"
#include "InjectUIDlg.h"
#include <tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CInjectUIDlg �Ի���




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


// CInjectUIDlg ��Ϣ�������
//<By�ŷ�ѩ> �϶�����
BOOL CInjectUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitEnvirment();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CInjectUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CInjectUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CInjectUIDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

//<By�ŷ�ѩ> ����
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
//��ʼ��ListBox�ؼ���ʾ������ 
	m_CtrlCrashProg.AddString(_T("�±�Խ��"));
	m_CtrlCrashProg.AddString(_T("��ָ��"));
	m_CtrlCrashProg.AddString(_T("��ѭ��"));
	m_CtrlCrashProg.AddString(_T("��Ч��ַ"));
	m_CtrlCrashProg.AddString(_T("��0") );
	m_CtrlCrashProg.AddString(_T("�Ƿ�����"));
	m_CtrlCrashProg.AddString(_T("д�ں�"));
	m_CtrlCrashProg.AddString(_T("д�����ַҳ"));
	m_CtrlCrashProg.AddString(_T("д��ָ��"));
	SetDlgItemText(IDC_COMBO_CRASHPROG,_T("д�����ַҳ"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
    m_ProcIdList.Format(_T("%s"),_T(""));
	GetDlgItemText(IDC_EDIT_PROCID,m_ProcIdList);
	if(m_ProcIdList.Compare(_T("")) == 0)
		return;
	
	CString cBSelStr;   
	m_CtrlCrashProg.GetWindowText(cBSelStr);  //��ȡѡ�õı�������

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
	if (oTempF.GetLength() ==0)  //��д��Unicode���ļ�ͷ
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
	//�������Զ��ע��dll,д��ѭ������ʽ�����ö�����̱���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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