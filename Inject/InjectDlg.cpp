// InjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Inject.h"
#include "InjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char *START_ACCELERATE = "_StartAccelerate@8";

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


// CInjectDlg �Ի���




CInjectDlg::CInjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInjectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROC, m_procListCtrl);
}

BEGIN_MESSAGE_MAP(CInjectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CInjectDlg::OnHdnItemclickListProc)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CInjectDlg::OnBnClickedButtonRefresh)
END_MESSAGE_MAP()


// CInjectDlg ��Ϣ�������

BOOL CInjectDlg::OnInitDialog()
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
	if (!LoadStartAccelerateFun())
	{
		AfxMessageBox("����DLL����ʧ��");
		exit(1);
	}

	m_enumProc = new CEnumProc();
	InitProcListCtrl();
	InsertProcListCtrl(m_enumProc->m_procList);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CInjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CInjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CInjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CInjectDlg::LoadStartAccelerateFun()
{
	BOOL res = FALSE;
	HMODULE h = ::LoadLibrary("AccelerateGear.dll");
	if (h)
	{
		StartAccelerate = (StartAccelerateFun)GetProcAddress(h, START_ACCELERATE);
		if (StartAccelerate)
		{
			res = TRUE;
		}
		else
		{
			FreeLibrary(h);
		}
	}
	return res;
}

void CInjectDlg::OnHdnItemclickListProc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CInjectDlg::InitProcListCtrl()
{
	DWORD exstyle = m_procListCtrl.GetExtendedStyle();
	m_procListCtrl.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CRect rect;
	m_procListCtrl.GetClientRect(&rect);
	m_procListCtrl.InsertColumn(0, "���", LVCFMT_LEFT, rect.Width() / 8);
	m_procListCtrl.InsertColumn(1, "PID", LVCFMT_LEFT, rect.Width() / 4);
	m_procListCtrl.InsertColumn(2, "��������", LVCFMT_LEFT, rect.Width() / 2);
}

void CInjectDlg::InsertProcListCtrl(list<ProcInfo> &procList)
{
	list<ProcInfo>::iterator ir;
	DWORD num = 0;
	for (ir = procList.begin(); ir != procList.end(); ir++)
	{
		CString numStr;
		numStr.Format("%d", num);
		CString procIDStr;
		procIDStr.Format("%d", ir->procID);
		m_procListCtrl.InsertItem(num, "");
		m_procListCtrl.SetItemText(num, 0, numStr);
		m_procListCtrl.SetItemText(num, 1, procIDStr);
		m_procListCtrl.SetItemText(num, 2, ir->procName);
		num++;
	}
}

void CInjectDlg::OnBnClickedButtonRefresh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_procListCtrl.DeleteAllItems();
	m_enumProc->RefreshProc();
	InsertProcListCtrl(m_enumProc->m_procList);
}
