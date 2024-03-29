﻿
// ImageManipulationsDlg.h: файл заголовка
//

#include "Math.h"
#include <string>

#pragma once


// Диалоговое окно CImageManipulationsDlg
class CImageManipulationsDlg : public CDialogEx
{
// Создание
public:
	CImageManipulationsDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGEMANIPULATIONS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	bool bImageValid = false;

	std::string sInputDirectory;
	std::string sInputPath;
	std::string sOutputPath;

	bool bGUI = true;

	struct ConsoleParams
	{
		int nType = -1;

		int X = 0;

		int Y = 0;

		int iPlant;

		std::string extFrom = "";
		std::string extTo = "";

		int nResWidth = 0;

		int nResHeight = 0;

		bool bCenter = false;

		std::string sPathFrom = "";

		std::string sPathTo = "";

		std::string sProfile = "";

		int angle = 0;
	};

	ConsoleParams pConsoleParams;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCalc();

	afx_msg void OnCbnSelchangeCombo1();

	void NotGUICalculation(std::string sCommandLine);
	
	void SetGUI(bool bGUI) { this->bGUI = bGUI;};

	void ShowGroup(int);
	void SetGroupVisible(int nType, int);
	afx_msg void OnBnClickedR();
	afx_msg void OnBnClickedG();
	afx_msg void OnBnClickedB();
	afx_msg void OnBnClickedOpen();

};

