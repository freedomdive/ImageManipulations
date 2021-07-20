
// ImageManipulations.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CImageManipulationsApp:
// Сведения о реализации этого класса: ImageManipulations.cpp
//

class CImageManipulationsApp : public CWinApp
{
public:
	CImageManipulationsApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CImageManipulationsApp theApp;
