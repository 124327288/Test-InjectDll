// InjectUIDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define MAXLENGTH 4096
#define WM_CRASH WM_USER + 0x010

// CInjectUIDlg 对话框
class CInjectUIDlg : public CDialog
{
// 构造
public:
	CInjectUIDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_INJECTUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ProcInfoList;
	HANDLE m_RemoteHandle;
	//BOOL isSEin;
public:
	afx_msg void OnBnClickedCancel();
	void InitEnvirment();
	DWORD GetProcessIdByName(LPCTSTR lpStrName);
	int InjectDllToRemoteProcess(DWORD dwProcessId, LPCTSTR szDllPath );
	BOOL EnablePrivilege(TCHAR *PrivilegeName,BOOL IsEnable);
public:
	afx_msg void OnBnClickedButtonUpdateprocinfo();
public:
	afx_msg void OnBnClickedButtonCrash();
public:
	CComboBox m_CtrlCrashProg;
public:
	CString m_ProcIdList;
public:
	afx_msg void OnNMClickListProcinfo(NMHDR *pNMHDR, LRESULT *pResult);
};
