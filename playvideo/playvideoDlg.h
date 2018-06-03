
// playvideoDlg.h : 头文件
//

#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/gpu/gpu.hpp"
#include "afxwin.h"
#include "afxdtctl.h"
#include "atlimage.h"
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
// CplayvideoDlg 对话框

class CplayvideoDlg : public CDialogEx
{
// 构造
public:
	CplayvideoDlg(CWnd* pParent = NULL);	// 标准构造函数
	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLAYVIDEO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HANDLE hThread;
	DWORD ThreadID;
	// 生成的消息映射函数
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
<<<<<<< HEAD
	void OnMatch();
=======
	static void ThreadFunc1(void *param);
	static void ThreadFunc2(void *param);
>>>>>>> e6fa2d5f5d3c14c50f1d3111a71fc5a34eb6e614
	afx_msg void OnEnChangeEdit1();
	void AddPitcure();
	CDateTimeCtrl m_dtCtrl;
	afx_msg void OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult);
	CDateTimeCtrl m_cdCtrl;
	CRect m_DlgRect;//存储对话框改变前大小，以供计算控件相应位置及大小时使用

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	void repaint(UINT id, int last_Width, int now_Width, int last_Height, int now_Height);
	afx_msg void OnSize(UINT nType, int cx, int cy);
<<<<<<< HEAD
	
	
	CFont m_font;
	
	//afx_msg void OnBnClickedCancel();
	CStatic m_ctrlPic;
	/*afx_msg void OnStnClickedStatic6();
	afx_msg void OnStnClickedStatic3();
	afx_msg void OnStnClickedStatic8();
	afx_msg void OnStnClickedStatic10();
	afx_msg void OnStnClickedStatic12();*/
	CScrollBar m_horiScrollbar;
	//afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	

	afx_msg void OnStnClickedStatic1();
=======
	CFont m_font;*/
	afx_msg void OnStnClickedStatic6();
	int m_numofall;
	int m_numofyes;
	int m_numofno;
>>>>>>> e6fa2d5f5d3c14c50f1d3111a71fc5a34eb6e614
};
