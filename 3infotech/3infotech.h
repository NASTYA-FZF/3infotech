
// 3infotech.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMy3infotechApp:
// Сведения о реализации этого класса: 3infotech.cpp
//

class CMy3infotechApp : public CWinApp
{
public:
	CMy3infotechApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMy3infotechApp theApp;
