
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
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CvCapture* capture; //��Ƶ��ȡ�ṹ
CRect rect;//������CRectҲΪһ��û�л���Ķ����࣬��װ��RECT�ṹ���г�Ա����left��top��right��bottom
CDC *pDC;//��Ƶ��ʾ�ؼ��豸������
HDC hDC;//��Ƶ��ʾ�ؼ��豸���
CWnd *pwnd;
CString FileName;
CStatic *pStc;//��ʶͼ����ʾ��picture �ؼ�
std::string cfg_file = "yolov2-tiny.cfg";
//cv::VideoCapture capture(0);
std::string weights_file = "yolov2-tiny.weights";
//Detector detector(cfg_file, weights_file); //����detector
Detector *detector;
int m;
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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CplayvideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboWeb);

	DDX_Control(pDX, IDC_COMBO1, m_comboWeb);
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
	ON_BN_CLICKED(IDC_BUTTON4, &CplayvideoDlg::OnBnClickedButton4)
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
		CDialogEx::OnPaint();
	}
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

	// ����  
	//cv::VideoCapture capture(0);//������ͷ  
	/*if (!capture.isOpened())
	{
	std::cout<< "������ͷ����" <<std::endl;
	return ;
	}*/
	// ����  
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
		cvRectangle(m_Frame, cvPoint(t.x, t.y), cvPoint(t.x + t.w, t.y + t.h), cv::Scalar(0, 255, 255), 5, 1, 0);
	}
	//m_Frame = &IplImage(frame);
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
		//cvWaitKey(10);
	}

	// ���ü�ʱ��,ÿ10ms����һ���¼�
	SetTimer(1, 20, NULL);


}

void CplayvideoDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cvReleaseCapture(&capture);
	CDC MemDC;
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BITMAP1);
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);
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
		MessageBox(_T("Error��"));
		return;
	}
	SetTimer(1, 25, NULL);
}
	



void CplayvideoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	IplImage* m_Frame;
	m_Frame = cvQueryFrame(capture);
	CvvImage m_CvvImage;
	boxs = detector->detect(m_Frame, 0.5);
	for (bbox_t t : boxs) {
		cvRectangle(m_Frame, cvPoint(t.x, t.y), cvPoint(t.x + t.w, t.y + t.h), cv::Scalar(0, 255, 255), 5, 1, 0);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cvReleaseCapture(&capture);
	CDC MemDC;
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BITMAP1);
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);
	
}




void CplayvideoDlg::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CplayvideoDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CplayvideoDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strText(_T(""));
	CString strText1(_T(""));
	CString strText2(_T(""));
	CString strText3(_T(""));
	CString strText7(_T(""));
	CString strText4(_T("������"));
	CString strText5(_T("��ñ����"));
	CString strText6(_T("δ��ñ����"));
	GetDlgItemText(IDC_COMBO1, strText);
	GetDlgItemText(IDC_EDIT3, strText1);//��ȡedit box�е�����
	GetDlgItemText(IDC_EDIT4, strText2);
	GetDlgItemText(IDC_EDIT5, strText3);
	strText7 = strText + "\r\n " + strText4 +' '+ strText1+"\r\n " + strText5 + ' ' + strText2+ "\r\n "+ strText6 + ' ' + strText3 + "\r\n ";


	try

	{

		CStdioFile   file;

		file.Open(_T("D:\\��Ƶ����.txt"), CFile::modeCreate | CFile::modeWrite | CFile::typeText|CFile::modeNoTruncate);

		//��D�̵�txt�ļ�
		file.SeekToEnd();
		file.WriteString(strText2+ "\r\n");  //д��edit box�е�����
		
		
		file.Close();

	}

	catch (CFileException* e)

	{

		e->ReportError();

		e->Delete();

	}

}


void CplayvideoDlg::OnEnChangeEdit3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CplayvideoDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CplayvideoDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strText(_T(""));
	CString strText1(_T(""));
	CString strText2(_T(""));
	CString strText3(_T(""));
	CString strText7(_T(""));
	CString strText4(_T("������"));
	CString strText5(_T("��ñ����"));
	CString strText6(_T("δ��ñ����"));
	CString strText8(_T(""));
	CString strText9(_T("��ʼʱ��"));
	CString strText10(_T("����ʱ��"));
	GetDlgItemText(IDC_DATETIMEPICKER1, strText);
	GetDlgItemText(IDC_DATETIMEPICKER2, strText8);
	GetDlgItemText(IDC_EDIT2, strText1);//��ȡedit box�е�����
	GetDlgItemText(IDC_EDIT6, strText2);
	GetDlgItemText(IDC_EDIT7, strText3);
	strText7 = strText9+' '+strText + "\r\n " +strText10 + ' ' + strText8 + "\r\n " + strText4 + ' ' + strText1 + "\r\n " + strText5 + ' ' + strText2 + "\r\n " + strText6 + ' ' + strText3 + "\r\n ";


	try

	{

		CStdioFile   file;

		file.Open(_T("D:\\����ͷ����.txt"), CFile::modeCreate | CFile::modeWrite | CFile::typeText | CFile::modeNoTruncate);

		//��D�̵�txt�ļ�
		file.SeekToEnd();
		file.WriteString(strText2 + "\r\n");  //д��edit box�е�����


		file.Close();

	}

	catch (CFileException* e)

	{

		e->ReportError();

		e->Delete();

	}

}
