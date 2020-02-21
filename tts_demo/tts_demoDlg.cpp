
// tts_demoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "tts_demo.h"
#include "tts_demoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTTSDemoDlg 对话框



CTTSDemoDlg::CTTSDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TTS_DEMO_DIALOG, pParent)
	, m_strText(_T("科大讯飞语音合成测试"))
	, m_nVolume(50)
	, m_nSpeed(50)
	, m_nPitch(50)
	, m_strOutputFileName(_T("tts_demo"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTTSDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_strText);
	DDX_Control(pDX, IDC_COMBO_VOICE_NAME, m_cbVoiceName);
	DDX_Text(pDX, IDC_EDIT_VOLUME, m_nVolume);
	DDV_MinMaxInt(pDX, m_nVolume, 0, 100);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_nSpeed);
	DDV_MinMaxInt(pDX, m_nSpeed, 0, 100);
	DDX_Text(pDX, IDC_EDIT_PITCH, m_nPitch);
	DDV_MinMaxInt(pDX, m_nPitch, 0, 100);
	DDX_Text(pDX, IDC_EDIT_OUTPUT_FILENAME, m_strOutputFileName);
}

BEGIN_MESSAGE_MAP(CTTSDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_TTS, &CTTSDemoDlg::OnBnClickedButtonTTS)
END_MESSAGE_MAP()


// CTTSDemoDlg 消息处理程序

BOOL CTTSDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitChildCtrls();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTTSDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTTSDemoDlg::OnPaint()
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
HCURSOR CTTSDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTTSDemoDlg::InitChildCtrls()
{
	m_cbVoiceName.AddString(_T("xiaoyan"));
	m_cbVoiceName.AddString(_T("xiaofeng"));
	m_cbVoiceName.SetCurSel(0);
}

std::string CTTSDemoDlg::CStringToStdstring(CString cstr)
{
	CT2CA pszStr(cstr);
	std::string str(pszStr);
	return str;
}

std::string CTTSDemoDlg::getCurExeFilePath()
{
	std::string strPath = "";

#ifdef _MSC_VER
	wchar_t exeFullPath[BUFSIZ] = { '\0' }; // Full path  
	GetModuleFileName(NULL, exeFullPath, BUFSIZ);

	char szString[BUFSIZ] = { '\0' };
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, szString, BUFSIZ, exeFullPath, _TRUNCATE);

	strPath = std::string(szString);    // Get full path of the file  
	int pos = (int)strPath.find_last_of("\\", strPath.length()) + 1;
#else
	char exeFullPath[BUFSIZ] = { '\0' }; // Full path  
	readlink("/proc/self/exe", exeFullPath, BUFSIZ);

	//char szString[BUFSIZ];
	//size_t convertedChars = 0;
	//wcstombs(szString, exeFullPath, BUFSIZ);

	strPath = std::string(exeFullPath);    // Get full path of the file
	int pos = (int)strPath.find_last_of("/", strPath.length()) + 1;
#endif // _MSC_VER

	return strPath.substr(0, pos);  // Return the directory without the file name (with "\\" or "/")
}

void CTTSDemoDlg::OnBnClickedButtonTTS()
{
	UpdateData(TRUE);

	m_tts.readTTSInfo(getCurExeFilePath());
	m_tts.setText(CStringToStdstring(m_strText));

	CString strVoiceName;
	m_cbVoiceName.GetLBText(m_cbVoiceName.GetCurSel(), strVoiceName);
	m_tts.setVoiceName(CStringToStdstring(strVoiceName));
	m_tts.setVolume(m_nVolume);
	m_tts.setSpeed(m_nSpeed);
	m_tts.setPitch(m_nPitch);
	m_tts.setOutputFileName(CStringToStdstring(m_strOutputFileName));

	if (m_tts.doTTS())
		AfxMessageBox(_T("语音合成成功！"));
	else
		AfxMessageBox(_T("语音合成失败！"));
}
