// InjectUIDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define MAXLENGTH 4096
#define WM_CRASH WM_USER + 0x010

// CInjectUIDlg �Ի���
class CInjectUIDlg : public CDialog
{
// ����
public:
	CInjectUIDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_INJECTUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
