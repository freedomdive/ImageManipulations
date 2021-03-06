﻿
// ImageManipulationsDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "ImageManipulations.h"
#include "ImageManipulationsDlg.h"
#include "afxdialogex.h"
#include <string>
#include <fstream>
#include "Params.h"
using namespace std;

#define FUNC_IMAGE_SHIFT			0
#define FUNC_CHANNELS_CHANGE		1
#define FUNC_ALPHA_CHANNEL_DELETE	2
#define FUNC_DRAW_PALETTE			3
#define FUNC_CALIBR_INI				4
#define FUNC_COUNT 5


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CImageManipulationsDlg



CImageManipulationsDlg::CImageManipulationsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGEMANIPULATIONS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageManipulationsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageManipulationsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CALC, &CImageManipulationsDlg::OnBnClickedCalc)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CImageManipulationsDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_R, &CImageManipulationsDlg::OnBnClickedR)
	ON_BN_CLICKED(IDC_G, &CImageManipulationsDlg::OnBnClickedG)
	ON_BN_CLICKED(IDC_B, &CImageManipulationsDlg::OnBnClickedB)
	ON_BN_CLICKED(IDC_BUTTON1, &CImageManipulationsDlg::OnBnClickedButton1)
END_MESSAGE_MAP()



// Обработчики сообщений CImageManipulationsDlg
BOOL CImageManipulationsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	
	//dbgCalculation2();


	SetDlgItemInt(IDC_EDIT_X, 0);
	SetDlgItemInt(IDC_EDIT_Y, 0);

	CComboBox * pCombo = static_cast<CComboBox *>(GetDlgItem(IDC_COMBO1));


	pCombo->AddString("Shift");
	
	pCombo->AddString("3->1;1->3");
	
	pCombo->AddString("Alpha delete");
	
	pCombo->AddString("Palette");

	pCombo->AddString("calibr.ini");
	
	pCombo->SetCurSel(0);

	ShowGroup(0);


	CButton* pButtonR = (CButton*)GetDlgItem(IDC_R);

	pButtonR->SetCheck(true);

	UpdateData(FALSE);
	UpdateWindow();

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CImageManipulationsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CImageManipulationsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CImageManipulationsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImageManipulationsDlg::OnBnClickedCalc()
{

	CFileDialog FileDialog(FALSE, 0, "");

	auto nResult = FileDialog.DoModal();


	if (nResult == IDOK)
	{
		CString sFileName = FileDialog.GetPathName();

		calc.SetOutputPath(const_cast<char*>(static_cast<LPCSTR>(sFileName)));
	}
	else
	{
		return;
	}
	


	if (calc.LoadInputImage() != 0)
	{
		SetDlgItemText(IDC_INFO, "No such input image");
		return;
	}

	char szBuffer[256];

	CComboBox * pCombo = static_cast<CComboBox *>(GetDlgItem(IDC_COMBO1));

	int nType = pCombo->GetCurSel();

	if (nType == FUNC_IMAGE_SHIFT)
	{

		int shiftX = GetDlgItemInt(IDC_EDIT_X);
		int shiftY = GetDlgItemInt(IDC_EDIT_Y);

		if (calc.shiftImages(shiftX, shiftY) == 0)
		{
			if (shiftX == 0 && shiftY == 0)
				SetDlgItemText(IDC_INFO, "Zero shift done");
			else
				SetDlgItemText(IDC_INFO, "Shift done");
		}
		else
		{
			SetDlgItemText(IDC_INFO, "Shift error");
		}
	}
	if (nType == FUNC_CHANNELS_CHANGE)
	{
		int nPlant = 0;

		if (IsDlgButtonChecked(IDC_R))
			nPlant = 0;
		if (IsDlgButtonChecked(IDC_G))
			nPlant = 1;
		if (IsDlgButtonChecked(IDC_B))
			nPlant = 2;
		
		if (calc.make_3_1_converting(nPlant) == 0)
		{
			int nInPlant = calc.GetDaInPlants();
			if (nInPlant == 1)
			{
				SetDlgItemText(IDC_INFO, "Transformation 1 -> 3 done");
			}
			if (nInPlant == 3)
			{
				if (nPlant == 0)
				{
					SetDlgItemText(IDC_INFO, "Transformation 3 -> 1 done via Red plant");
				}
				if (nPlant == 1)
				{
					SetDlgItemText(IDC_INFO, "Transformation 3 -> 1 done via Green plant");
				}
				if (nPlant == 2)
				{
					SetDlgItemText(IDC_INFO, "Transformation 3 -> 1 done via Blue plant");
				}
			}
			if (nInPlant != 1 && nInPlant != 3)
			{
				
				sprintf_s(szBuffer, 256, "Transformation error, Input Image Plants = %d", nInPlant);
				SetDlgItemText(IDC_INFO, szBuffer);
			}
		}
		else
		{
			SetDlgItemText(IDC_INFO, "Transformation error");
		}

	}
	if (nType == FUNC_ALPHA_CHANNEL_DELETE)
	{
		int nInPlant = calc.GetDaInPlants();

		if (nInPlant != 4)
		{
			sprintf_s(szBuffer, 256, "Image does not have alpha channel, nPlants = %d", nInPlant);
			SetDlgItemText(IDC_INFO, szBuffer);
		}
		else
		{
			if (calc.alphaChannelDelete() == 0)
			{
				SetDlgItemText(IDC_INFO, "Alpha channel deleting done");
			}
			else
			{
				SetDlgItemText(IDC_INFO, "Alpha channel deleting error");
			}
		}
	}
	if (nType == FUNC_DRAW_PALETTE)
	{
		int nInPlant = calc.GetDaInPlants();

		if (nInPlant != 1)
		{
			SetDlgItemText(IDC_INFO, "Image must have 1 plant");
		}
		else
		{
			if (calc.calcPalette() == 0)
			{
				SetDlgItemText(IDC_INFO, "Palette calculateion done");
			}
			else
			{
				SetDlgItemText(IDC_INFO, "Palette calculateion error");
			}
		}
	}
	if (nType == FUNC_CALIBR_INI)
	{
		if (Params::bInited == false)
		{
			SetDlgItemText(IDC_INFO, "Calibr.Ini not initialized");
			
			return;
		}
		if (calc.GetDaInPlants() != 3 && calc.GetDaInPlants() != 1)
		{
			SetDlgItemText(IDC_INFO, "Invalid Plants, need 1 or 3 plants");

			return;
		}
		
		calc.AcceptCalibrIni();

		SetDlgItemText(IDC_INFO, "Calibr.Ini accepted");
	}

}


void CImageManipulationsDlg::ShowGroup(int nType)
{
	for (int i = 0; i < FUNC_COUNT; i++)
	{
		SetGroupVisible(i, SW_HIDE);
	}
	SetGroupVisible(nType, SW_SHOW);
}

void CImageManipulationsDlg::SetGroupVisible(int nType, int nCmdShow)
{
	CWnd * wnd;
	if (nType == FUNC_IMAGE_SHIFT)
	{
		wnd = GetDlgItem(IDC_STATIC_X);
		wnd->ShowWindow(nCmdShow);

		wnd = GetDlgItem(IDC_STATIC_Y);
		wnd->ShowWindow(nCmdShow);

		wnd = GetDlgItem(IDC_EDIT_X);
		wnd->ShowWindow(nCmdShow);

		wnd = GetDlgItem(IDC_EDIT_Y);
		wnd->ShowWindow(nCmdShow);
	}
	if (nType == FUNC_CHANNELS_CHANGE)
	{
		wnd = GetDlgItem(IDC_STATIC_3_1);
		wnd->ShowWindow(nCmdShow);

		wnd = GetDlgItem(IDC_R);
		wnd->ShowWindow(nCmdShow);

		wnd = GetDlgItem(IDC_G);
		wnd->ShowWindow(nCmdShow);

		wnd = GetDlgItem(IDC_B);
		wnd->ShowWindow(nCmdShow);
	}
}

void CImageManipulationsDlg::OnCbnSelchangeCombo1()
{
	CComboBox * pCombo = static_cast<CComboBox *>(GetDlgItem(IDC_COMBO1));

	int nType = pCombo->GetCurSel();

	ShowGroup(nType);
}


void CImageManipulationsDlg::OnBnClickedR()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_G);

	pButton->SetCheck(false);

	pButton = (CButton*)GetDlgItem(IDC_B);

	pButton->SetCheck(false);
}


void CImageManipulationsDlg::OnBnClickedG()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_R);

	pButton->SetCheck(false);

	pButton = (CButton*)GetDlgItem(IDC_B);

	pButton->SetCheck(false);
}


void CImageManipulationsDlg::OnBnClickedB()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_R);

	pButton->SetCheck(false);

	pButton = (CButton*)GetDlgItem(IDC_G);

	pButton->SetCheck(false);
}


void CImageManipulationsDlg::OnBnClickedButton1()
{
	CFileDialog FileDialog(TRUE, 0, "*.*");

	auto nResult = FileDialog.DoModal();

	
	if (nResult == IDOK)
	{
		CString sFileName = FileDialog.GetPathName();
		SetDlgItemText(IDC_EDIT_INPUT, sFileName);

		calc.SetInputPath(const_cast<char*>(static_cast<LPCSTR>(sFileName)));
	}
	
}
