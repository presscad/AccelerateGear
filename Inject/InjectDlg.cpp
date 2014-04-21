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
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CInjectDlg::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_INJECT, &CInjectDlg::OnBnClickedButtonInject)
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

#define LIST_ID			0
#define LIST_PID		1
#define LIST_WND		2
#define LIST_NAME		3
#define LIST_IDSTR		"���"
#define LIST_PIDSTR		"PID"
#define LIST_WNDSTR		"����"
#define LIST_NAMESTR	"��������"

void CInjectDlg::InitProcListCtrl()
{
	DWORD exstyle = m_procListCtrl.GetExtendedStyle();
	m_procListCtrl.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CRect rect;
	m_procListCtrl.GetClientRect(&rect);
	m_procListCtrl.InsertColumn(LIST_ID, LIST_IDSTR, LVCFMT_LEFT, rect.Width() / 10);
	m_procListCtrl.InsertColumn(LIST_PID, LIST_PIDSTR, LVCFMT_LEFT, rect.Width() * 2 / 10);
	m_procListCtrl.InsertColumn(LIST_WND, LIST_WNDSTR, LVCFMT_LEFT, rect.Width() * 2 / 10);
	m_procListCtrl.InsertColumn(LIST_NAME, LIST_NAMESTR, LVCFMT_LEFT, rect.Width() * 4 / 10);
}

void CInjectDlg::InsertProcListCtrl(vector<ProcInfo> &procList)
{
	vector<ProcInfo>::iterator ivec;
	DWORD num = 0;
	for (ivec = procList.begin(); ivec != procList.end(); ivec++)
	{
		CString numStr;
		CString procIDStr;
		numStr.Format("%d", num);
		procIDStr.Format("%d", ivec->procID);
		m_procListCtrl.InsertItem(num, "");
		m_procListCtrl.SetItemText(num, LIST_ID, numStr);
		m_procListCtrl.SetItemText(num, LIST_PID, procIDStr);
		m_procListCtrl.SetItemText(num, LIST_WND, ivec->wndName);
		m_procListCtrl.SetItemText(num, LIST_NAME, ivec->procName);
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

void CInjectDlg::OnBnClickedButtonInject()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_procListCtrl.GetFirstSelectedItemPosition();
	if (pos)
	{
		DWORD selectItem = m_procListCtrl.GetNextSelectedItem(pos);
	}
	else
	{
		AfxMessageBox("��ѡ��һ������");
	}
}