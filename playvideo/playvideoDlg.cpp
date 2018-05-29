
// playvideoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "playvideo.h"
#include "playvideoDlg.h"
#include "afxdialogex.h"
#include"CvvImage.h"
#include"resource.h"
#include "yolo_v2_class.hpp"
#include <vector>
#include "MyButton.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define DATABASE_NAME "Hat"
#define DATABASE_HOSTNAME "47.101.57.53:3306"
#define DATABASE_USERNAME "root"
#define DATABASE_PWD "123456"
CvCapture* capture; //视频获取结构
CRect rect;//矩形类CRect也为一个没有基类的独立类，封装了RECT结构，有成员变量left、top、right和bottom
CDC *pDC;//视频显示控件设备上下文
HDC hDC;//视频显示控件设备句柄
CWnd *pwnd;
CString FileName;
CStatic *pStc;//标识图像显示的picture 控件
CMyButton m_Btn1;
CMyButton m_Btn2;
CMyButton m_Btn3;
CMyButton m_Btn4;
CMyButton m_Btn5;
CMyButton m_Btn6;
CMyButton m_Btn7;
//boolean b= connection.connect("安全帽检测", "localhost", "zhijian", "123456", 3306);
volatile BOOL m_bRun1;
volatile BOOL m_bRun2;
//boolean b;

using namespace std;
sql::Driver *driver;
sql::Connection *con;
sql::Statement *stmt;
sql::ResultSet *res;
sql::PreparedStatement  *prep_stmt1;
std::string sql1 = "INSERT INTO result (numofall, numofyes, numofno) VALUE (?, ?, ?); ";

std::string cfg_file = "myyolov3-tiny-person.cfg";
std::string weights_file = "myyolov3-tiny-person_54600.weights";
//std::string cfg_file = "myyolov3-tiny.cfg";
//std::string cfg_file = "yolov3-tiny.cfg";
//std::string weights_file = "yolov3-tiny.weights";
//cv::VideoCapture capture(0);
//std::string weights_file = "myyolov3-tiny_62600.weights";
//Detector detector(cfg_file, weights_file); //生成detector
Detector *detector;


cv::Mat frame;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
std::vector<bbox_t> boxs;
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


// CplayvideoDlg 对话框



CplayvideoDlg::CplayvideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PLAYVIDEO_DIALOG, pParent)
	, m_numofall(0)
	, m_numofyes(0)
	, m_numofno(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CplayvideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboWeb);

	DDX_Control(pDX, IDC_COMBO1, m_comboWeb);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dtCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_cdCtrl);
	DDX_Text(pDX, IDC_STATIC6, m_numofall);
	DDX_Text(pDX, IDC_STATIC8, m_numofyes);
	DDX_Text(pDX, IDC_STATIC10, m_numofno);
}

BEGIN_MESSAGE_MAP(CplayvideoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CplayvideoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CplayvideoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CplayvideoDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &CplayvideoDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON5, &CplayvideoDlg::OnBnClickedButton5)
	
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CplayvideoDlg::OnDtnDatetimechangeDatetimepicker1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CplayvideoDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON6, &CplayvideoDlg::OnBnClickedButton6)
	//ON_EN_CHANGE(IDC_EDIT3, &CplayvideoDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT1, &CplayvideoDlg::OnEnChangeEdit1)
	
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &CplayvideoDlg::OnDtnDatetimechangeDatetimepicker2)
	
	//ON_EN_CHANGE(IDC_EDIT2, &CplayvideoDlg::OnEnChangeEdit2)
	ON_WM_CTLCOLOR()
	
	ON_WM_SIZE()
	ON_WM_SIZE()
	ON_STN_CLICKED(IDC_STATIC6, &CplayvideoDlg::OnStnClickedStatic6)
END_MESSAGE_MAP()


// CplayvideoDlg 消息处理程序

BOOL CplayvideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	detector = new Detector(cfg_file, weights_file, 0);
	// TODO: 在此添加额外的初始化代码
	pwnd = GetDlgItem(IDC_STATIC);//访问控件的ID，即可返回该控件的指针
	//pwnd->MoveWindow(35,30,352,288);
	pDC = pwnd->GetDC();//获取设备上下文
	//pDC =GetDC();
	hDC = pDC->GetSafeHdc();//返回输出设备上下文的句柄
	pwnd->GetClientRect(&rect);//GetClientRect是得到窗口句柄的用户坐标。
	pStc = (CStatic *)GetDlgItem(IDC_STATIC);
	m_comboWeb.AddString(_T("测试视频1"));
	m_comboWeb.AddString(_T("测试视频2"));
	m_comboWeb.AddString(_T("测试视频3"));
	m_comboWeb.AddString(_T("测试视频4"));
	m_comboWeb.AddString(_T("测试视频5"));
	m_comboWeb.SetCurSel(0);
	GetDlgItem(IDC_BUTTON1)->ModifyStyle(0, BS_OWNERDRAW, 0);
	GetDlgItem(IDC_BUTTON2)->ModifyStyle(0,BS_OWNERDRAW,0);
	GetDlgItem(IDC_BUTTON3)->ModifyStyle(0, BS_OWNERDRAW, 0);
	GetDlgItem(IDC_BUTTON5)->ModifyStyle(0, BS_OWNERDRAW, 0);
	GetDlgItem(IDC_BUTTON6)->ModifyStyle(0, BS_OWNERDRAW, 0);
	GetDlgItem(IDCANCEL)->ModifyStyle(0, BS_OWNERDRAW, 0);
	GetDlgItem(IDOK)->ModifyStyle(0, BS_OWNERDRAW, 0);
	m_Btn1.Attach(IDC_BUTTON1, this);
	m_Btn2.Attach(IDC_BUTTON2, this);
	m_Btn3.Attach(IDC_BUTTON3, this);
	m_Btn4.Attach(IDOK, this);
	m_Btn5.Attach(IDC_BUTTON5, this);
	m_Btn6.Attach(IDC_BUTTON6, this);
	m_Btn7.Attach(IDCANCEL, this);
	
	//m_Btn.SetDownColor(RGB(255,0,0));
	m_Btn1.SetUpColor(RGB(222, 156, 83));
	m_Btn2.SetUpColor(RGB(222, 156, 83));
	m_Btn3.SetUpColor(RGB(222, 156, 83));
	m_Btn4.SetUpColor(RGB(222, 156, 83));
	m_Btn5.SetUpColor(RGB(222, 156, 83));
	m_Btn6.SetUpColor(RGB(222, 156, 83));
	m_Btn7.SetUpColor(RGB(222, 156, 83));
	m_Btn1.SetDownColor(RGB(222, 156, 83));
	m_Btn2.SetDownColor(RGB(222, 156, 83));
	m_Btn3.SetDownColor(RGB(222, 156, 83));
	m_Btn4.SetDownColor(RGB(222, 156, 83));
	m_Btn5.SetDownColor(RGB(222, 156, 83));
	m_Btn6.SetDownColor(RGB(222, 156, 83));
	m_Btn7.SetDownColor(RGB(222, 156, 83));
	m_DlgRect.SetRect(0, 0, 0, 0);//初始化对话框大小存储变量 
	
	CString timeFormat1;
	timeFormat1 = "MM/dd/yyyy   hh:mm tt";
	GetDlgItem(IDC_DATETIMEPICKER1)->SendMessage((UINT)DTM_SETFORMAT, (WPARAM)0, (LPARAM)
		(LPCTSTR)timeFormat1);

	CString timeFormat2;
	timeFormat2 = "MM/dd/yyyy   hh:mm tt";
	GetDlgItem(IDC_DATETIMEPICKER2)->SendMessage((UINT)DTM_SETFORMAT, (WPARAM)0, (LPARAM)
		(LPCTSTR)timeFormat2);

	
	try {// 创建连接 
		driver = get_driver_instance();
		con = driver->connect(DATABASE_HOSTNAME, DATABASE_USERNAME, DATABASE_PWD);
		// 连接 MySQL 数据库 test  
		con->setSchema("hat");
		prep_stmt1 = con->prepareStatement(sql1);
		//stmt = con->createStatement();
		//stmt->execute("USE Hat");
		
	}
	catch (sql::SQLException &e) {
		//cout << "# ERR: SQLException in " << __FILE__;
		//cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		//cout << "# ERR: " << e.what();
		//cout << " (MySQL error code: " << e.getErrorCode();
		//cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		return TRUE;
	}
    return TRUE;

	

	
	
	
	
}

void CplayvideoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CplayvideoDlg::OnPaint()
{
	if (IsIconic())
	{
		CRect   rect;
		CPaintDC dc(this); // 用于绘制的设备上下文
		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		
		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		
		GetClientRect(&rect);
		
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDC MemDC;
		CBitmap m_Bitmap1;
		m_Bitmap1.LoadBitmap(IDB_BITMAP1);
		MemDC.CreateCompatibleDC(NULL);
		MemDC.SelectObject(&m_Bitmap1);
		pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, 440, 304, SRCCOPY);
	}
		
		CDialogEx::OnPaint();
	}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CplayvideoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CplayvideoDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bRun1 = FALSE;
	m_bRun2 = FALSE;
	cvReleaseCapture(&capture);
	if (!capture)
	{
		capture = cvCaptureFromCAM(0);
		
	}

	if (!capture)
	{
		MessageBox(_T("无法打开摄像头"));
		return;
	}


	IplImage* m_Frame;
	
	m_Frame = cvQueryFrame(capture);
	CvvImage m_CvvImage;
	
	boxs = detector->detect(m_Frame);
	m_numofall = boxs.size();
	m_numofno = m_numofyes=0;
	for (bbox_t t : boxs) {
		if (t.obj_id == 1)
		{
			m_numofno++;
			cvRectangle(m_Frame, cvPoint(t.x, t.y), cvPoint(t.x + t.w, t.y + t.h), cv::Scalar(0, 0, 255), 5, 1, 0);
		}
		else
		{
			m_numofyes++;
			cvRectangle(m_Frame, cvPoint(t.x, t.y), cvPoint(t.x + t.w, t.y + t.h), cv::Scalar(0, 255, 0), 5, 1, 0);
		}
	}
	//m_Frame = &IplImage(frame);
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
		//cvWaitKey(10);
	}
	UpdateData(false);
	// 设置计时器,每0ms触发一次事件
	SetTimer(1, 0, NULL);
	AfxBeginThread((AFX_THREADPROC)ThreadFunc1, this);


}

void CplayvideoDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	cvReleaseCapture(&capture);
	KillTimer(1);
	m_bRun1 = FALSE;
	m_bRun2 = FALSE;
	CDC MemDC;
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BITMAP1);
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, 440, 304, SRCCOPY);
}


void CplayvideoDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
		FileName = dlg.GetPathName();
	
	SetDlgItemText(IDC_EDIT1, LPCTSTR(FileName));
}


void CplayvideoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bRun1 = FALSE;
	m_bRun2 = FALSE;
	IplImage* img = cvLoadImage(((LPCTSTR)FileName));
	/*boxs = detector->detect(img);
	for (bbox_t t : boxs) {
		if (t.obj_id == 1)
			cvRectangle(img, cvPoint(t.x, t.y), cvPoint(t.x + t.w, t.y + t.h), cv::Scalar(0, 0, 255), 5, 1, 0);
		else
			cvRectangle(img, cvPoint(t.x, t.y), cvPoint(t.x + t.w, t.y + t.h), cv::Scalar(0, 255, 0), 5, 1, 0);
	}*/
	CvvImage cvvImg;
	cvvImg.CopyOf(img);
	cvvImg.DrawToHDC(hDC, &rect);
	//OnOK();
	cvReleaseCapture(&capture);
	if (!capture)
	{
		capture = cvCreateFileCapture(((LPCTSTR)FileName));
		
	}
	if (!capture)
	{
		MessageBox(_T("请先选择视频！"));
		return;
	}
	SetTimer(1, 0, NULL);
	
	
}
	



void CplayvideoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	IplImage* m_Frame;
	m_Frame = cvQueryFrame(capture);
	CvvImage m_CvvImage;
	if (m_Frame != NULL)
	{
		boxs = detector->detect(m_Frame, 0.3);
		m_numofall = boxs.size();
		m_numofno = m_numofyes = 0;
		for (bbox_t t : boxs) {
			if (t.obj_id == 1)
			{
				m_numofno++;
				cvRectangle(m_Frame, cvPoint(t.x, t.y), cvPoint(t.x + t.w, t.y + t.h), cv::Scalar(0, 0, 255), 5, 1, 0);
			}
			else
			{
				m_numofyes++;
				cvRectangle(m_Frame, cvPoint(t.x, t.y), cvPoint(t.x + t.w, t.y + t.h), cv::Scalar(0, 255, 0), 5, 1, 0);
			}
		}
		m_CvvImage.CopyOf(m_Frame, 1);
		if (true)
		{
			m_CvvImage.DrawToHDC(hDC, &rect);
			//cvWaitKey(10);
		}
		UpdateData(false);
	}
	else
	{
		cvReleaseCapture(&capture);
		KillTimer(1);
		CDC MemDC;
		CBitmap m_Bitmap1;
		m_Bitmap1.LoadBitmap(IDB_BITMAP1);
		MemDC.CreateCompatibleDC(NULL);
		MemDC.SelectObject(&m_Bitmap1);
		pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, 440, 304, SRCCOPY);
	}
	
	CDialogEx::OnTimer(nIDEvent);
}




void CplayvideoDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	cvReleaseCapture(&capture);
	KillTimer(1);
	m_bRun1 = FALSE;
	m_bRun2 = FALSE;
	CDC MemDC;
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BITMAP1);
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, 440, 304, SRCCOPY);
	
}




void CplayvideoDlg::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString timeFormat;
	timeFormat = "MM/dd/yyyy   hh:mm:00 tt";
	GetDlgItem(IDC_DATETIMEPICKER1)->SendMessage((UINT)DTM_SETFORMAT, (WPARAM)0, (LPARAM)
		(LPCTSTR)timeFormat);
	*pResult = 0;
}


void CplayvideoDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CplayvideoDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	

}





void CplayvideoDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}





void CplayvideoDlg::OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString timeFormat;
	timeFormat = "MM/dd/yyyy   hh:mm:00 tt";
	GetDlgItem(IDC_DATETIMEPICKER2)->SendMessage((UINT)DTM_SETFORMAT, (WPARAM)0, (LPARAM)
		(LPCTSTR)timeFormat);
	*pResult = 0;
}





/*void CplayvideoDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}*/


HBRUSH CplayvideoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性


	if (nCtlColor == CTLCOLOR_BTN)          //更改按钮颜色  
	{
		//pDC->SetBkMode(TRANSPARENT);  
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(121, 121, 255));
		HBRUSH b=CreateSolidBrush(RGB(121, 121, 255));
		return (HBRUSH)b;
	}
	 else if (nCtlColor == CTLCOLOR_SCROLLBAR)  //  
	{
		//pDC->SetBkMode(TRANSPARENT);  
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(233, 233, 220));
		HBRUSH b=CreateSolidBrush(RGB(233, 233, 220));
		return b;
	}
	else if (nCtlColor == CTLCOLOR_EDIT)   //更改编辑框  
	{
		//pDC->SetBkMode(TRANSPARENT);  
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(246, 246, 246));
		HBRUSH b=CreateSolidBrush(RGB(246, 246, 246));
		return b;
	}
	else if (nCtlColor == CTLCOLOR_STATIC)  //更改静态文本  
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(217, 226, 241));
		HBRUSH b=CreateSolidBrush(RGB(217, 226, 241));
		return b;
	}
	else if (nCtlColor == CTLCOLOR_DLG)   //更改对话框背景色  
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(166, 254, 1));
		HBRUSH b=CreateSolidBrush(RGB(213, 226, 240));
		return b;
	}
	

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

/*void CplayvideoDlg::repaint(UINT id, int last_Width, int now_Width, int last_Height, int now_Height)//更新控件位置和大小函数，可以根据需要自行修改  
{
	CRect rect;
	CWnd *wnd = NULL;
	wnd = GetDlgItem(id);
	if (NULL == wnd)
	{
		MessageBox(_T("相应控件不存在"));
	}
	wnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.left = (long)((double)rect.left / (double)last_Width*(double)now_Width);
	rect.right = (long)((double)rect.right / (double)last_Width*(double)now_Width);
	rect.top = (long)((double)rect.top / (double)last_Height*(double)now_Height);
	rect.bottom = (long)((double)rect.bottom / (double)last_Height*(double)now_Height);
	wnd->MoveWindow(&rect);
	
}




void CplayvideoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 
	if (0 == m_DlgRect.left && 0 == m_DlgRect.right  
        && 0 == m_DlgRect.top && 0 == m_DlgRect.bottom)//第一次启动对话框时的大小变化不做处理  
	{
	}
	else
	{
		if (0 == cx && 0 == cy)//如果是按下了最小化，则触发条件，这时不保存对话框数据  
		{
			return;
		}
		CRect rectDlgChangeSize;
		GetClientRect(&rectDlgChangeSize);//存储对话框大小改变后对话框大小数据  

		repaint(IDC_STATIC, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());//重绘函数，用以更新对话框上控件的位置和大小  
		repaint(IDOK, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDCANCEL, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC_1, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());//重绘函数，用以更新对话框上控件的位置和大小  
		repaint(IDC_STATIC3, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC4, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC5, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());//重绘函数，用以更新对话框上控件的位置和大小  
		repaint(IDC_STATIC11, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC12, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC13, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());//重绘函数，用以更新对话框上控件的位置和大小  
		repaint(IDC_STATIC6, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC8, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC10, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_BUTTON1, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_BUTTON2, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_BUTTON3, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());//重绘函数，用以更新对话框上控件的位置和大小  
		repaint(IDC_BUTTON5, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_BUTTON6, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_EDIT1, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_DATETIMEPICKER1, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_DATETIMEPICKER2, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_COMBO1, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		LOGFONT LogFont;
		GetFont()->GetLogFont(&LogFont);//
		LogFont.lfHeight += LogFont.lfHeight;
		LogFont.lfWidth += LogFont.lfWidth;
		m_font.CreateFontIndirect(&LogFont);
		GetDlgItem(IDC_BUTTON1)->SetFont(&m_font);
		
	}
	GetClientRect(&m_DlgRect); //save size of dialog  
	Invalidate();//更新窗口  

}*/


void CplayvideoDlg::OnStnClickedStatic6()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CplayvideoDlg::ThreadFunc1(void *param)

{

	CplayvideoDlg  *dlg = (CplayvideoDlg  *)param;
	m_bRun1 = TRUE;
	while (m_bRun1)

	{

		
		prep_stmt1->setInt(1, dlg->m_numofall);
		prep_stmt1->setInt(2, dlg->m_numofyes);
		prep_stmt1->setInt(3, dlg->m_numofno);
		prep_stmt1->execute();
		//mysqlpp::Query  query = connection.query(sql);
		//stmt->executeQuery(sql1);
		
		//stmt->execute(sql1);

		Sleep(1000);

	}

}