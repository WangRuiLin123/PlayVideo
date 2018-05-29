
// playvideoDlg.cpp : ʵ���ļ�
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
CvCapture* capture; //��Ƶ��ȡ�ṹ
CRect rect;//������CRectҲΪһ��û�л���Ķ����࣬��װ��RECT�ṹ���г�Ա����left��top��right��bottom
CDC *pDC;//��Ƶ��ʾ�ؼ��豸������
HDC hDC;//��Ƶ��ʾ�ؼ��豸���
CWnd *pwnd;
CString FileName;
CStatic *pStc;//��ʶͼ����ʾ��picture �ؼ�
CMyButton m_Btn1;
CMyButton m_Btn2;
CMyButton m_Btn3;
CMyButton m_Btn4;
CMyButton m_Btn5;
CMyButton m_Btn6;
CMyButton m_Btn7;
//boolean b= connection.connect("��ȫñ���", "localhost", "zhijian", "123456", 3306);
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
//Detector detector(cfg_file, weights_file); //����detector
Detector *detector;


cv::Mat frame;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
std::vector<bbox_t> boxs;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CplayvideoDlg �Ի���



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


// CplayvideoDlg ��Ϣ�������

BOOL CplayvideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	detector = new Detector(cfg_file, weights_file, 0);
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	pwnd = GetDlgItem(IDC_STATIC);//���ʿؼ���ID�����ɷ��ظÿؼ���ָ��
	//pwnd->MoveWindow(35,30,352,288);
	pDC = pwnd->GetDC();//��ȡ�豸������
	//pDC =GetDC();
	hDC = pDC->GetSafeHdc();//��������豸�����ĵľ��
	pwnd->GetClientRect(&rect);//GetClientRect�ǵõ����ھ�����û����ꡣ
	pStc = (CStatic *)GetDlgItem(IDC_STATIC);
	m_comboWeb.AddString(_T("������Ƶ1"));
	m_comboWeb.AddString(_T("������Ƶ2"));
	m_comboWeb.AddString(_T("������Ƶ3"));
	m_comboWeb.AddString(_T("������Ƶ4"));
	m_comboWeb.AddString(_T("������Ƶ5"));
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
	m_DlgRect.SetRect(0, 0, 0, 0);//��ʼ���Ի����С�洢���� 
	
	CString timeFormat1;
	timeFormat1 = "MM/dd/yyyy   hh:mm tt";
	GetDlgItem(IDC_DATETIMEPICKER1)->SendMessage((UINT)DTM_SETFORMAT, (WPARAM)0, (LPARAM)
		(LPCTSTR)timeFormat1);

	CString timeFormat2;
	timeFormat2 = "MM/dd/yyyy   hh:mm tt";
	GetDlgItem(IDC_DATETIMEPICKER2)->SendMessage((UINT)DTM_SETFORMAT, (WPARAM)0, (LPARAM)
		(LPCTSTR)timeFormat2);

	
	try {// �������� 
		driver = get_driver_instance();
		con = driver->connect(DATABASE_HOSTNAME, DATABASE_USERNAME, DATABASE_PWD);
		// ���� MySQL ���ݿ� test  
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CplayvideoDlg::OnPaint()
{
	if (IsIconic())
	{
		CRect   rect;
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		
		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		
		GetClientRect(&rect);
		
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CplayvideoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CplayvideoDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bRun1 = FALSE;
	m_bRun2 = FALSE;
	cvReleaseCapture(&capture);
	if (!capture)
	{
		capture = cvCaptureFromCAM(0);
		
	}

	if (!capture)
	{
		MessageBox(_T("�޷�������ͷ"));
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
	// ���ü�ʱ��,ÿ0ms����һ���¼�
	SetTimer(1, 0, NULL);
	AfxBeginThread((AFX_THREADPROC)ThreadFunc1, this);


}

void CplayvideoDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
		FileName = dlg.GetPathName();
	
	SetDlgItemText(IDC_EDIT1, LPCTSTR(FileName));
}


void CplayvideoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox(_T("����ѡ����Ƶ��"));
		return;
	}
	SetTimer(1, 0, NULL);
	
	
}
	



void CplayvideoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString timeFormat;
	timeFormat = "MM/dd/yyyy   hh:mm:00 tt";
	GetDlgItem(IDC_DATETIMEPICKER1)->SendMessage((UINT)DTM_SETFORMAT, (WPARAM)0, (LPARAM)
		(LPCTSTR)timeFormat);
	*pResult = 0;
}


void CplayvideoDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CplayvideoDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

}





void CplayvideoDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}





void CplayvideoDlg::OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString timeFormat;
	timeFormat = "MM/dd/yyyy   hh:mm:00 tt";
	GetDlgItem(IDC_DATETIMEPICKER2)->SendMessage((UINT)DTM_SETFORMAT, (WPARAM)0, (LPARAM)
		(LPCTSTR)timeFormat);
	*pResult = 0;
}





/*void CplayvideoDlg::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}*/


HBRUSH CplayvideoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����


	if (nCtlColor == CTLCOLOR_BTN)          //���İ�ť��ɫ  
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
	else if (nCtlColor == CTLCOLOR_EDIT)   //���ı༭��  
	{
		//pDC->SetBkMode(TRANSPARENT);  
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(246, 246, 246));
		HBRUSH b=CreateSolidBrush(RGB(246, 246, 246));
		return b;
	}
	else if (nCtlColor == CTLCOLOR_STATIC)  //���ľ�̬�ı�  
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(217, 226, 241));
		HBRUSH b=CreateSolidBrush(RGB(217, 226, 241));
		return b;
	}
	else if (nCtlColor == CTLCOLOR_DLG)   //���ĶԻ��򱳾�ɫ  
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(166, 254, 1));
		HBRUSH b=CreateSolidBrush(RGB(213, 226, 240));
		return b;
	}
	

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

/*void CplayvideoDlg::repaint(UINT id, int last_Width, int now_Width, int last_Height, int now_Height)//���¿ؼ�λ�úʹ�С���������Ը�����Ҫ�����޸�  
{
	CRect rect;
	CWnd *wnd = NULL;
	wnd = GetDlgItem(id);
	if (NULL == wnd)
	{
		MessageBox(_T("��Ӧ�ؼ�������"));
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
        && 0 == m_DlgRect.top && 0 == m_DlgRect.bottom)//��һ�������Ի���ʱ�Ĵ�С�仯��������  
	{
	}
	else
	{
		if (0 == cx && 0 == cy)//����ǰ�������С�����򴥷���������ʱ������Ի�������  
		{
			return;
		}
		CRect rectDlgChangeSize;
		GetClientRect(&rectDlgChangeSize);//�洢�Ի����С�ı��Ի����С����  

		repaint(IDC_STATIC, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());//�ػ溯�������Ը��¶Ի����Ͽؼ���λ�úʹ�С  
		repaint(IDOK, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDCANCEL, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC_1, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());//�ػ溯�������Ը��¶Ի����Ͽؼ���λ�úʹ�С  
		repaint(IDC_STATIC3, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC4, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC5, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());//�ػ溯�������Ը��¶Ի����Ͽؼ���λ�úʹ�С  
		repaint(IDC_STATIC11, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC12, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC13, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());//�ػ溯�������Ը��¶Ի����Ͽؼ���λ�úʹ�С  
		repaint(IDC_STATIC6, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC8, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_STATIC10, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_BUTTON1, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_BUTTON2, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());
		repaint(IDC_BUTTON3, m_DlgRect.Width(), rectDlgChangeSize.Width(), m_DlgRect.Height(), rectDlgChangeSize.Height());//�ػ溯�������Ը��¶Ի����Ͽؼ���λ�úʹ�С  
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
	Invalidate();//���´���  

}*/


void CplayvideoDlg::OnStnClickedStatic6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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