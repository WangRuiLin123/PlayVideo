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
	//������ɫ�ֱ�Ϊ���֣�Button Down�ı�����ɫ��Button Up�ı�����ɫ
	COLORREF m_TextColor, m_DownColor, m_UpColor;
};


