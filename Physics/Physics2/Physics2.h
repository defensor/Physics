
// Physics2.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CPhysics2App:
// О реализации данного класса см. Physics2.cpp
//

class CPhysics2App : public CWinApp
{
public:
	CPhysics2App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CPhysics2App theApp;