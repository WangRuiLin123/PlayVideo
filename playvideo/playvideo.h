
// playvideo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once
#pragma comment(lib,"vfw32.lib")
#pragma comment(lib,"comctl32.lib")
#pragma  comment(lib,"yolo_cpp_dll.lib")
#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CplayvideoApp: 
// �йش����ʵ�֣������ playvideo.cpp
//

class CplayvideoApp : public CWinApp
{
public:

	CplayvideoApp();

	// ��д
public:
	virtual BOOL InitInstance();

	// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CplayvideoApp theApp;