
// playvideoDlg.h : ͷ�ļ�
//

#pragma once
#include "opencv2/opencv.hpp"
#include "afxwin.h"
#include "afxdtctl.h"
#include "atlimage.h"
// CplayvideoDlg �Ի���
class CplayvideoDlg : public CDialogEx
{
// ����
public:
	CplayvideoDlg(CWnd* pParent = NULL);	// ��׼���캯��
	
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLAYVIDEO_DIALOG };
#endif

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnBnClickedButton4();
	CButton StopButton;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedplay();
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton6();
	CComboBox m_comboWeb;
	
	afx_msg void OnEnChangeEdit1();
	
	CDateTimeCtrl m_dtCtrl;
	afx_msg void OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult);
	CDateTimeCtrl m_cdCtrl;
	CRect m_DlgRect;//�洢�Ի���ı�ǰ��С���Թ�����ؼ���Ӧλ�ü���Сʱʹ��

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	
	/*void repaint(UINT id, int last_Width, int now_Width, int last_Height, int now_Height);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CFont m_font;*/
};
