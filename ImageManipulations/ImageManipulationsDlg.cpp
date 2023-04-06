
// ImageManipulationsDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "ImageManipulations.h"
#include "ImageManipulationsDlg.h"
#include <iostream>
#include <filesystem>
#include <boost/algorithm/string.hpp>
#include "afxdialogex.h"
#include <string>
#include <fstream>
#include "Params.h"
using namespace std;
using namespace boost::algorithm;

#define FUNC_IMAGE_SHIFT			0
#define FUNC_CHANNELS_CHANGE		1
#define FUNC_ALPHA_CHANNEL_DELETE	2
#define FUNC_DRAW_PALETTE			3
#define FUNC_CALIBR_INI				4
#define FUNC_REFORMAT_ALL			5
#define FUNC_SCALE					6
#define FUNC_ROTATION				7
#define FUNC_COUNT					8


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

	pCombo->AddString("Rotation");
	
	pCombo->SetCurSel(0);

	ShowGroup(0);

	bImageValid = false;

	CButton* pButtonR = (CButton*)GetDlgItem(IDC_R);

	pButtonR->SetCheck(true);

	UpdateData(FALSE);
	UpdateWindow();


	CButton* p = (CButton*)GetDlgItem(IDC_IS_INDEXED);

	p->SetCheck(true);

	p = (CButton*)GetDlgItem(IDC_IS_CENTER);

	p->SetCheck(true);

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
		GetDlgItem(IDC_STATIC_1)->ShowWindow(nCmdShow);
		GetDlgItem(IDC_STATIC_2)->ShowWindow(nCmdShow);

		GetDlgItem(IDC_STATIC_1)->SetWindowText("From");
		GetDlgItem(IDC_STATIC_2)->SetWindowText("To");
	}
	if (nType == FUNC_SCALE)
	{
		GetDlgItem(IDC_WIDTH_IN)->ShowWindow(nCmdShow);
		GetDlgItem(IDC_WIDTH_OUT)->ShowWindow(nCmdShow);

		GetDlgItem(IDC_HEIGHT_IN)->ShowWindow(nCmdShow);
		GetDlgItem(IDC_HEIGHT_OUT)->ShowWindow(nCmdShow);

		GetDlgItem(IDC_STATIC_FORMAT2)->ShowWindow(nCmdShow);

		GetDlgItem(IDC_IS_INDEXED)->ShowWindow(nCmdShow);
		GetDlgItem(IDC_IS_CENTER)->ShowWindow(nCmdShow);
		
	}

	if (nType == FUNC_ROTATION)
	{
		GetDlgItem(IDC_FORMAT_FROM_)->ShowWindow(nCmdShow);
		GetDlgItem(IDC_WIDTH_OUT)->ShowWindow(nCmdShow);
		GetDlgItem(IDC_HEIGHT_OUT)->ShowWindow(nCmdShow);
		GetDlgItem(IDC_STATIC_1)->ShowWindow(nCmdShow);
		GetDlgItem(IDC_STATIC_1)->SetWindowText("Angle");

		if (Math::pDaIn.nOffset)
		{
			GetDlgItem(IDC_FORMAT_FROM_)->SetWindowText("0");
			GetDlgItem(IDC_WIDTH_OUT)->SetWindowText(to_string(Math::pDaIn.nWidth).c_str());
			GetDlgItem(IDC_HEIGHT_OUT)->SetWindowText(to_string(Math::pDaIn.nHeight).c_str());
		}
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

	CComboBox* pCombo = bGUI ? static_cast<CComboBox*>(GetDlgItem(IDC_COMBO1)) : nullptr;

	const auto nType = bGUI ? pCombo->GetCurSel() : pConsoleParams.nType;

	if (bGUI && nType != FUNC_REFORMAT_ALL)
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
		if (bGUI)
			SetDlgItemText(IDC_INFO, "No such input image");
		return;
	}

	char szBuffer[256];

	ImageArea pDa;

	if (nType == FUNC_IMAGE_SHIFT)
	{

		int shiftX = bGUI ? GetDlgItemInt(IDC_EDIT_X) : pConsoleParams.X;
		int shiftY = bGUI ? GetDlgItemInt(IDC_EDIT_Y) : pConsoleParams.Y;

		pDa = Math::ShiftImages(shiftX, shiftY);

		if (bGUI)
		{
			if (shiftX == 0 && shiftY == 0)
				SetDlgItemText(IDC_INFO, "Zero shift done");
			else
				SetDlgItemText(IDC_INFO, "Shift done");
		}

	}
	if (nType == FUNC_CHANNELS_CHANGE)
	{
		int nPlant = 0;

		if (bGUI)
		{
			if (IsDlgButtonChecked(IDC_R))
				nPlant = 0;
			if (IsDlgButtonChecked(IDC_G))
				nPlant = 1;
			if (IsDlgButtonChecked(IDC_B))
				nPlant = 2;
		}
		else
			nPlant = pConsoleParams.iPlant;

		pDa = Math::Make_3_1_Converting(nPlant);

		
		const int nInPlant = Math::pDaIn.nPlants;

		if (bGUI)
		{
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

	}
	if (nType == FUNC_ALPHA_CHANNEL_DELETE)
	{
		if (Math::pDaIn.nPlants != 4)
		{
			if (bGUI)
			{
				sprintf_s(szBuffer, 256, "Image does not have alpha channel, nPlants = %d", Math::pDaIn.nPlants);
				SetDlgItemText(IDC_INFO, szBuffer);
			}
		}
		else
		{
			pDa = Math::AlphaChannelDelete();

			if (bGUI)
			{
				SetDlgItemText(IDC_INFO, "Alpha channel deleting done");
			}
		}
	}
	if (nType == FUNC_DRAW_PALETTE)
	{
		if (Math::pDaIn.nPlants != 1)
		{
			if (bGUI)
			{
				SetDlgItemText(IDC_INFO, "Image must have 1 plant");
			}
			return;
		}
		else
		{
			pDa = Math::CalcPalette();
			if (bGUI)
			{
				SetDlgItemText(IDC_INFO, "Palette calculateion done");
			}
		}
	}
	if (nType == FUNC_CALIBR_INI)
	{
		if (bGUI)
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
		}

		pDa = Math::AcceptCalibrIni();

		if (bGUI)
		{
			SetDlgItemText(IDC_INFO, "Calibr.Ini accepted");
		}
	}

	if (nType == FUNC_REFORMAT_ALL)
	{
		CString sFormatFrom;
		CString sFormatTo;
		if (bGUI)
		{
			GetDlgItemText(IDC_FORMAT_FROM_, sFormatFrom);
			GetDlgItemText(IDC_FORMAT_TO_, sFormatTo);
		}

		if (std::filesystem::exists(sInputDirectory) == false)
		{
			if (bGUI)
				SetDlgItemText(IDC_INFO, "No such folder");

			return;
		}

		if (sFormatFrom.GetLength() == 0 || sFormatTo.GetLength() == 0)
		{
			if (bGUI)
				SetDlgItemText(IDC_INFO, "Set image formats");

			return;
		}


		int nCount = Math::ReFormatImages(sInputDirectory, const_cast<char*>(static_cast<LPCSTR>(sFormatFrom)), const_cast<char*>(static_cast<LPCSTR>(sFormatTo)));

		if (bGUI)
		{
			string str = "Reformed ";

			str += to_string(nCount);

			str += " images";

			SetDlgItemText(IDC_INFO, str.c_str());
		}
	}

	if (nType == FUNC_SCALE)
	{
		CString str;

		int nWidth = bGUI ? GetDlgItemInt(IDC_WIDTH_OUT) : pConsoleParams.nResWidth;
		int nHeight = bGUI ? GetDlgItemInt(IDC_HEIGHT_OUT) : pConsoleParams.nResHeight;

		bool bIndexed = bGUI ? ((CButton*)GetDlgItem(IDC_IS_INDEXED))->GetCheck() : pConsoleParams.bIndexed;
		bool bCenter = bGUI ? ((CButton*)GetDlgItem(IDC_IS_CENTER))->GetCheck() : pConsoleParams.bCenter;


		if (bGUI)
		{
			if (nWidth <= 0 || nHeight <= 0 || Math::pDaIn.nPlants > 1 && bIndexed)
			{
				SetDlgItemText(IDC_INFO, "Incorrect intput values");

				return;
			}
		}

		if (nWidth && nHeight)
			pDa = Math::ScaleImage(nWidth, nHeight, bIndexed, bCenter);

		if (bGUI)
		{
			SetDlgItemText(IDC_INFO, "Scaling finished");
		}
	}
	
	if (nType == FUNC_ROTATION)
	{
		char buf[10];

		GetDlgItem(IDC_FORMAT_FROM_)->GetWindowText(buf, 10);

		auto check_values = [this](char* buf)
		{
			if (buf[0] == 0)
			{
				if (bGUI)
					SetDlgItemText(IDC_INFO, "Set not empty values");

				return false;
			}

			bool valid = true;

			for (int i = 0; i < 10; i++)
			{
				if (buf[i] == 0)
					break;

				if (!(buf[i] == '-' || buf[i] >= '0' && buf[i] <= '9'))
				{
					valid = false;
					break;
				}
			}

			if (valid == false)
			{
				if (bGUI)
					SetDlgItemText(IDC_INFO, "Set correct values (numbers)");

				return valid;
			}

			return valid;
		};

		
		if (check_values(buf) == false)
			return;

		const auto angle = stoi(buf);

		GetDlgItem(IDC_WIDTH_OUT)->GetWindowText(buf, 10);

		if (check_values(buf) == false)
			return;

		const auto width = stoi(buf);

		GetDlgItem(IDC_HEIGHT_OUT)->GetWindowText(buf, 10);

		if (check_values(buf) == false)
			return;

		const auto height = stoi(buf);



		pDa = Math::RotateImage(angle, width, height);

		if (bGUI)
		{
			SetDlgItemText(IDC_INFO, "Rotation finished");
		}
	}

	if (nType != FUNC_REFORMAT_ALL)
	{

		std::filesystem::path path = sOutputPath;

		int nRet = 0;

		if (Math::pDaIn.is_palette())
		{
			nRet = pDa.WritePNGRandomPalette(sOutputPath.c_str());
		}
		else
		{
			nRet = pDa.Write(sOutputPath.c_str());
		}

		if (nRet != ImageArea::OK)
		{
			if (bGUI)
			{
				SetDlgItemText(IDC_INFO, "Error saving image");
			}
		}
	}
}



void CImageManipulationsDlg::NotGUICalculation(string sCommandLine)
{
	vector<string> vRes;

	boost::split(vRes, sCommandLine, [](char c) {return c == ' '; });

	if (vRes.size() % 2)
		return;

	for (int i = 0; i < vRes.size(); i+=2)
	{
		auto& sKey = vRes[i];

		auto& sValue = vRes[i + 1];

		if (to_lower_copy(sKey) == "-type" || to_lower_copy(sKey) == "--type")
		{
			if (sValue == "0" || sValue == "Shift")
			{
				pConsoleParams.nType = 0;
			}
			if (sValue == "1" || sValue == "3->1;1->3")
			{
				pConsoleParams.nType = 1;
			}
			if (sValue == "2" || sValue == "Alpha_delete")
			{
				pConsoleParams.nType = 2;
			}
			if (sValue == "3" || sValue == "Palette")
			{
				pConsoleParams.nType = 3;
			}
			if (sValue == "4" || sValue == "calibr.ini")
			{
				pConsoleParams.nType = 4;
			}
			if (sValue == "5" || sValue == "Reformat_all")
			{
				pConsoleParams.nType = 5;
			}
			if (sValue == "6" || sValue == "Scale")
			{
				pConsoleParams.nType = 6;
			}
		}

		if (to_lower_copy(sKey) == "-x" || to_lower_copy(sKey) == "--x")
		{
			pConsoleParams.X = stoi(sValue);
		}
		if (to_lower_copy(sKey) == "-y" || to_lower_copy(sKey) == "--y")
		{
			pConsoleParams.Y = stoi(sValue);
		}
		if (to_lower_copy(sKey) == "-plant" || to_lower_copy(sKey) == "--plant")
		{
			pConsoleParams.iPlant = stoi(sValue);
		}
		if (to_lower_copy(sKey) == "-extfrom" || to_lower_copy(sKey) == "--extfrom")
		{
			pConsoleParams.extFrom = sValue;
		}
		if (to_lower_copy(sKey) == "-extto" || to_lower_copy(sKey) == "--extto")
		{
			pConsoleParams.extTo = sValue;
		}
		if (to_lower_copy(sKey) == "-width" || to_lower_copy(sKey) == "--width")
		{
			pConsoleParams.nResWidth = stoi(sValue);
		}
		if (to_lower_copy(sKey) == "-height" || to_lower_copy(sKey) == "--height")
		{
			pConsoleParams.nResHeight = stoi(sValue);
		}
		if (to_lower_copy(sKey) == "-indexed" || to_lower_copy(sKey) == "--indexed")
		{
			pConsoleParams.bIndexed = sValue == "1";
		}
		if (to_lower_copy(sKey) == "-center" || to_lower_copy(sKey) == "--center")
		{
			pConsoleParams.bCenter = sValue == "1";
		}
		if (to_lower_copy(sKey) == "-pathfrom" || to_lower_copy(sKey) == "--pathfrom")
		{
			pConsoleParams.sPathFrom = sValue;
		}
		if (to_lower_copy(sKey) == "-pathto" || to_lower_copy(sKey) == "--pathto")
		{
			pConsoleParams.sPathTo = sValue;
		}
		if (to_lower_copy(sKey) == "-profile" || to_lower_copy(sKey) == "--profile")
		{
			pConsoleParams.sProfile = sValue;
		}
	}


	sInputDirectory = pConsoleParams.sPathFrom;
	sInputPath = pConsoleParams.sPathFrom;
	sOutputPath = pConsoleParams.sPathTo;

	if (pConsoleParams.sProfile.length())
		Params::GetProfile().sProfilePath = pConsoleParams.sProfile;

	if (pConsoleParams.nType != FUNC_REFORMAT_ALL)
	{
		if (Math::pDaIn.Read(sInputPath.c_str()) != 0)
		{
			bImageValid = false;
			return;
		}
	}

	bImageValid = true;


	OnBnClickedCalc();

}