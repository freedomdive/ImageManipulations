
// ImageManipulationsDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "ImageManipulations.h"
#include "ImageManipulationsDlg.h"

#include <filesystem>

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
#define FUNC_REFORMAT_ALL			5
#define FUNC_SCALE					6
#define FUNC_COUNT					7


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
	ON_BN_CLICKED(IDC_BUTTON1, &CImageManipulationsDlg::OnBnClickedOpen)
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

	pCombo->AddString("Reformat all");

	pCombo->AddString("Scale");
	
	pCombo->SetCurSel(0);

	ShowGroup(0);

	bImageValid = false;

	CButton* pButtonR = (CButton*)GetDlgItem(IDC_R);

	pButtonR->SetCheck(true);

	UpdateData(FALSE);
	UpdateWindow();


	CButton* p = (CButton*)GetDlgItem(IDC_IS_INDEXED);

	p->SetCheck(true);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}


void CImageManipulationsDlg::f()
{
	Math::pDaIn.Read("D:/source/repos/Projects/ColourLayersAutoGUI/x64/Release/123/100/SampleCL_Algo_CLResult.png");

	//ImageArea pDa = Math::CalcPalette();

	//pDa.Write("D:/source/repos/Projects/ColourLayersAutoGUI/x64/Release/123/100/SampleCL_Palette.png");

	ImageArea pDa = Math::ScaleImage(2000, 1000, false);

	//Math::pDaIn = pDa;

	//pDa = Math::CalcPalette();

	pDa.Write("D:/source/repos/Projects/ColourLayersAutoGUI/x64/Release/123/100/SampleCL_Algo_CLResult_.png");
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
	if (nType == FUNC_REFORMAT_ALL)
	{
		GetDlgItem(IDC_STATIC_FORMAT)->ShowWindow(nCmdShow);
		GetDlgItem(IDC_FORMAT_FROM_)->ShowWindow(nCmdShow);
		GetDlgItem(IDC_FORMAT_TO_)->ShowWindow(nCmdShow);
	}
	if (nType == FUNC_SCALE)
	{
		GetDlgItem(IDC_WIDTH_IN)->ShowWindow(nCmdShow);
		GetDlgItem(IDC_WIDTH_OUT)->ShowWindow(nCmdShow);

		GetDlgItem(IDC_HEIGHT_IN)->ShowWindow(nCmdShow);
		GetDlgItem(IDC_HEIGHT_OUT)->ShowWindow(nCmdShow);

		GetDlgItem(IDC_STATIC_FORMAT2)->ShowWindow(nCmdShow);

		GetDlgItem(IDC_IS_INDEXED)->ShowWindow(nCmdShow);
		
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


void CImageManipulationsDlg::OnBnClickedOpen()
{
	CComboBox* pCombo = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO1));

	int nType = pCombo->GetCurSel();

	if (nType == FUNC_REFORMAT_ALL)
	{

		CFolderPickerDialog folderPickerDialog("", OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_ENABLESIZING, this,
			sizeof(OPENFILENAME));

		auto nResult1 = folderPickerDialog.DoModal();

		if (nResult1 == IDOK)
		{
			sInputDirectory = folderPickerDialog.GetPathName();

		}
	}
	else
	{
		CFileDialog FileDialog(TRUE, 0, "*.*");

		auto nResult = FileDialog.DoModal();


		if (nResult == IDOK)
		{
			sInputPath = FileDialog.GetPathName();
			SetDlgItemText(IDC_EDIT_INPUT, sInputPath.c_str());

			if (Math::pDaIn.Read(sInputPath.c_str()) != 0)
			{
				SetDlgItemText(IDC_INFO, "No such input image");

				bImageValid = false;

				return;
			}

			bImageValid = true;

			//if (nType == FUNC_SCALE)
			{
				CString sWidth;
				sWidth.Format("%d", Math::pDaIn.nWidth);

				CString sHeight;
				sHeight.Format("%d", Math::pDaIn.nHeight);

				SetDlgItemText(IDC_WIDTH_IN, sWidth);
				SetDlgItemText(IDC_HEIGHT_IN, sHeight);
			}
			//IDC_IS_INDEXED
		}
	}

}

void CImageManipulationsDlg::OnBnClickedCalc()
{

	CComboBox* pCombo = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO1));

	const auto nType = pCombo->GetCurSel();

	if (nType != FUNC_REFORMAT_ALL)
	{
		CFileDialog FileDialog(FALSE, 0, "");

		auto nResult = FileDialog.DoModal();

		if (nResult == IDOK)
		{
			sOutputPath = FileDialog.GetPathName();
		}
		else
		{
			return;
		}
	}

	if (bImageValid == false && nType != FUNC_REFORMAT_ALL)
	{
		SetDlgItemText(IDC_INFO, "No such input image");
		return;
	}

	char szBuffer[256];

	ImageArea pDa;

	if (nType == FUNC_IMAGE_SHIFT)
	{

		int shiftX = GetDlgItemInt(IDC_EDIT_X);
		int shiftY = GetDlgItemInt(IDC_EDIT_Y);

		pDa = Math::ShiftImages(shiftX, shiftY);

		if (shiftX == 0 && shiftY == 0)
			SetDlgItemText(IDC_INFO, "Zero shift done");
		else
			SetDlgItemText(IDC_INFO, "Shift done");
		

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

		pDa = Math::Make_3_1_Converting(nPlant);

		
		const int nInPlant = Math::pDaIn.nPlants;

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

			return;
		}
		

	}
	if (nType == FUNC_ALPHA_CHANNEL_DELETE)
	{
		if (Math::pDaIn.nPlants != 4)
		{
			sprintf_s(szBuffer, 256, "Image does not have alpha channel, nPlants = %d", Math::pDaIn.nPlants);
			SetDlgItemText(IDC_INFO, szBuffer);
		}
		else
		{
			pDa = Math::AlphaChannelDelete();

			SetDlgItemText(IDC_INFO, "Alpha channel deleting done");
		}
	}
	if (nType == FUNC_DRAW_PALETTE)
	{
		if (Math::pDaIn.nPlants != 1)
		{
			SetDlgItemText(IDC_INFO, "Image must have 1 plant");

			return;
		}
		else
		{
			pDa = Math::CalcPalette();

			SetDlgItemText(IDC_INFO, "Palette calculateion done");
		}
	}
	if (nType == FUNC_CALIBR_INI)
	{
		if (Params::bInited == false)
		{
			SetDlgItemText(IDC_INFO, "Calibr.Ini not initialized");

			return;
		}
		if (Math::pDaIn.nPlants != 3 && Math::pDaIn.nPlants != 1)
		{
			SetDlgItemText(IDC_INFO, "Invalid Plants, need 1 or 3 plants");

			return;
		}

		pDa = Math::AcceptCalibrIni();


		SetDlgItemText(IDC_INFO, "Calibr.Ini accepted");
	}

	if (nType == FUNC_REFORMAT_ALL)
	{
		CString sFormatFrom;
		CString sFormatTo;

		GetDlgItemText(IDC_FORMAT_FROM_, sFormatFrom);
		GetDlgItemText(IDC_FORMAT_TO_, sFormatTo);

		if (std::filesystem::exists(sInputDirectory) == false)
		{
			SetDlgItemText(IDC_INFO, "No such folder");

			return;
		}

		if (sFormatFrom.GetLength() == 0 || sFormatTo.GetLength() == 0)
		{
			SetDlgItemText(IDC_INFO, "Set image formats");

			return;
		}


		int nCount = Math::ReFormatImages(sInputDirectory, const_cast<char*>(static_cast<LPCSTR>(sFormatFrom)), const_cast<char*>(static_cast<LPCSTR>(sFormatTo)));

		string str = "Reformed ";

		str += to_string(nCount);

		str += " images";

		SetDlgItemText(IDC_INFO, str.c_str());
	}

	if (nType == FUNC_SCALE)
	{
		CString str;

		int nWidth = GetDlgItemInt(IDC_WIDTH_OUT);
		int nHeight = GetDlgItemInt(IDC_HEIGHT_OUT);

		bool bIndexed = ((CButton*)GetDlgItem(IDC_IS_INDEXED))->GetCheck();



		if (nWidth <= 0 || nHeight <= 0 || Math::pDaIn.nPlants > 1 && bIndexed)
		{
			SetDlgItemText(IDC_INFO, "Incorrect intput values");

			return;
		}

		pDa = Math::ScaleImage(nWidth, nHeight, bIndexed);

		SetDlgItemText(IDC_INFO, "Scaling finished");
	}


	if (nType != FUNC_REFORMAT_ALL)
	{
		if (pDa.Write(sOutputPath.c_str()) != ImageArea::OK)
		{
			SetDlgItemText(IDC_INFO, "Error saving image");
		}
	}
}