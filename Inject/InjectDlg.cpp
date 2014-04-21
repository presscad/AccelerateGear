// InjectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Inject.h"
#include "InjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char *START_ACCELERATE = "_StartAccelerate@8";

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


// CInjectDlg 对话框




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


// CInjectDlg 消息处理程序

BOOL CInjectDlg::OnInitDialog()
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
	if (!LoadStartAccelerateFun())
	{
		AfxMessageBox("加载DLL函数失败");
		exit(1);
	}

	m_enumProc = new CEnumProc();
	InitProcListCtrl();
	InsertProcListCtrl(m_enumProc->m_procList);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CInjectDlg::InitProcListCtrl()
{
	DWORD exstyle = m_procListCtrl.GetExtendedStyle();
	m_procListCtrl.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CRect rect;
	m_procListCtrl.GetClientRect(&rect);
	m_procListCtrl.InsertColumn(0, "编号", LVCFMT_LEFT, rect.Width() / 8);
	m_procListCtrl.InsertColumn(1, "PID", LVCFMT_LEFT, rect.Width() / 4);
	m_procListCtrl.InsertColumn(2, "进程名称", LVCFMT_LEFT, rect.Width() / 2);
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
	// TODO: 在此添加控件通知处理程序代码
	m_procListCtrl.DeleteAllItems();
	m_enumProc->RefreshProc();
	InsertProcListCtrl(m_enumProc->m_procList);
}
