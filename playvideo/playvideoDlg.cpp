
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
CvCapture* capture; //视频获取结构
CRect rect;//矩形类CRect也为一个没有基类的独立类，封装了RECT结构，有成员变量left、top、right和bottom
CDC *pDC;//视频显示控件设备上下文
HDC hDC;//视频显示控件设备句柄
CWnd *pwnd;
CString FileName;
CStatic *pStc;//标识图像显示的picture 控件
<<<<<<< HEAD
std::string cfg_file = "myyolov2-tiny.cfg";
=======
CMyButton m_Btn1;
CMyButton m_Btn2;
CMyButton m_Btn3;
CMyButton m_Btn4;
CMyButton m_Btn5;
CMyButton m_Btn6;
CMyButton m_Btn7;




std::string cfg_file = "yolov2-tiny.cfg";
>>>>>>> 7024b7c5db4c6c132e58b20661ce6b749a3e528a
//cv::VideoCapture capture(0);
std::string weights_file = "myyolov2-tiny_31300.weights";
//Detector detector(cfg_file, weights_file); //生成detector
Detector *detector;
int m;
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
	ON_EN_CHANGE(IDC_EDIT3, &CplayvideoDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT1, &CplayvideoDlg::OnEnChangeEdit1)
	
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &CplayvideoDlg::OnDtnDatetimechangeDatetimepicker2)
	
	//ON_EN_CHANGE(IDC_EDIT2, &CplayvideoDlg::OnEnChangeEdit2)
	ON_WM_CTLCOLOR()
	
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



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

	// 测试  
	//cv::VideoCapture capture(0);//打开摄像头  
	/*if (!capture.isOpened())
	{
	std::cout<< "读摄像头有误" <<std::endl;
	return ;
	}*/
	// 测试  
	IplImage* m_Frame;
	//cv::Mat frame;
	//capture >> frame;
	//capture.release();
	m_Frame = cvQueryFrame(capture);
	CvvImage m_CvvImage;
	//m=detector->get_net_height();
	//frame = cv::Mat(m_Frame);
	//std::vector<bbox_t> boxs;
	boxs = detector->detect(m_Frame, 0.3);
	//m_Frame = &IplImage(frame);
	//cvRectangle(m_Frame, cvPoint(boxs[0].x, boxs[0].y), cvPoint(boxs[0].x+ boxs[0].w, boxs[0].y+ boxs[0].h), cv::Scalar(0, 255, 255), 5, 1, 0);
	//cvRectangle(m_Frame, cvPoint(100,100), cvPoint(500, 500), cv::Scalar(255, 255, 255), 55, 1, 0);
	for (bbox_t t : boxs) {
		if(t.obj_id==1)
			cvRectangle(m_Frame, cvPoint(t.x, t.y), cvPoint(t.x + t.w, t.y + t.h), cv::Scalar(0,0,255), 5, 1, 0);
		else
			cvRectangle(m_Frame, cvPoint(t.x, t.y), cvPoint(t.x + t.w, t.y + t.h), cv::Scalar(0, 255, 0), 5, 1, 0);
	}
	//m_Frame = &IplImage(frame);
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
		//cvWaitKey(10);
	}

	// 设置计时器,每10ms触发一次事件
	SetTimer(1, 20, NULL);


}

void CplayvideoDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	cvReleaseCapture(&capture);
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
	IplImage* img = cvLoadImage(((LPCTSTR)FileName));
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
	SetTimer(1, 25, NULL);
}
	



void CplayvideoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	IplImage* m_Frame;
	m_Frame = cvQueryFrame(capture);
	CvvImage m_CvvImage;
	boxs = detector->detect(m_Frame, 0.5);
	for (bbox_t t : boxs) {
		if (t.obj_id == 1)
			cvRectangle(m_Frame, cvPoint(t.x, t.y), cvPoint(t.x + t.w, t.y + t.h), cv::Scalar(0, 0, 255), 5, 1, 0);
		else
			cvRectangle(m_Frame, cvPoint(t.x, t.y), cvPoint(t.x + t.w, t.y + t.h), cv::Scalar(0, 255, 0), 5, 1, 0);
	}
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
		//cvWaitKey(10);
	}
	CDialogEx::OnTimer(nIDEvent);
}




void CplayvideoDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	cvReleaseCapture(&capture);
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


void CplayvideoDlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
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


