// InjectDlg.h : 头文件
//

#pragma once
#include "EnumProc.h"
#include "afxcmn.h"


// CInjectDlg 对话框
class CInjectDlg : public CDialog
{
// 构造
public:
	CInjectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_INJECT_DIALOG };

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
	typedef BOOL (__stdcall *StartAccelerateFun)(DWORD threadID, BOOL enable);
	StartAccelerateFun StartAccelerate;
	BOOL LoadStartAccelerateFun();

public:

public:
	CEnumProc *m_enumProc;
	CListCtrl m_procListCtrl;

public:
	void InitProcListCtrl();
	void InsertProcListCtrl(vector<ProcInfo> &procVector);
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonInject();
};
