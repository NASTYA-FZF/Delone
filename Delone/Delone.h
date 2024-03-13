
// Delone.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CDeloneApp:
// Сведения о реализации этого класса: Delone.cpp
//

class CDeloneApp : public CWinApp
{
public:
	CDeloneApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CDeloneApp theApp;
