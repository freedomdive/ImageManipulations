
// ImageManipulationsDlg.h: файл заголовка
//

#include "Calculate.h"
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

	bool bExternalCalc;
	int nExternalType;
	
	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCalc();

	Calculate calc;
	afx_msg void OnCbnSelchangeCombo1();

	void ForcedCalc(std::string InputPath, std::string OutputPath);
	
	void ShowGroup(int);
	void SetGroupVisible(int nType, int);
	afx_msg void OnBnClickedR();
	afx_msg void OnBnClickedG();
	afx_msg void OnBnClickedB();
	afx_msg void OnBnClickedButton1();
};
