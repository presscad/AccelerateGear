// InjectDlg.h : ͷ�ļ�
//

#pragma once


// CInjectDlg �Ի���
class CInjectDlg : public CDialog
{
// ����
public:
	CInjectDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_INJECT_DIALOG };

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
	typedef BOOL (__stdcall *StartAccelerateFun)(HWND hWnd, BOOL enable);
	StartAccelerateFun StartAccelerate;
	BOOL LoadStartAccelerateFun();

public:
	afx_msg void OnBnClickedOk();
};
