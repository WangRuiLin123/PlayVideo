#pragma once
#include"afxwin.h"
class CMyButton :public  CButton
{
public:
	CMyButton();
	virtual~CMyButton();
	void SetDownColor(COLORREF color);
	void SetUpColor(COLORREF color);
	BOOL Attach(const UINT nID, CWnd* pParent);
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	//三种颜色分别为文字，Button Down的背景颜色，Button Up的背景颜色
	COLORREF m_TextColor, m_DownColor, m_UpColor;
};


