
// tts_demo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTTSDemoApp: 
// �йش����ʵ�֣������ tts_demo.cpp
//

class CTTSDemoApp : public CWinApp
{
public:
	CTTSDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTTSDemoApp theApp;