#include "stdafx.h"
#include "Params.h"
#include <string>
#include <filesystem>
using namespace std;

namespace _Ini
{
	bool _WritePrivateProfileString(std::string&& sSection, std::string&& sKey, std::string&& sValue, const std::filesystem::path& sPath)
	{
		return WritePrivateProfileString(sSection.c_str(), sKey.c_str(), sValue.c_str(), sPath.string().c_str());
	}

	std::string _GetPrivateProfileString(std::string&& sSection, std::string&& sKey, std::string&& sDefault, const std::filesystem::path& sPath)
	{
		static char szBuffer[256];

		GetPrivateProfileString(sSection.c_str(), sKey.c_str(), sDefault.c_str(), szBuffer, sizeof(szBuffer), sPath.string().c_str());

		return szBuffer;
	}

	int _GetPrivateProfileInt(std::string&& sSection, std::string&& sKey, int&& nDefault, const std::filesystem::path& sPath)
	{
		return GetPrivateProfileInt(sSection.c_str(), sKey.c_str(), nDefault, sPath.string().c_str());
	}

	double _GetPrivateProfileDouble(std::string&& sSection, std::string&& sKey, double&& nDefault, const std::filesystem::path& sPath)
	{
		return stod(_GetPrivateProfileString(move(sSection), move(sKey), std::to_string(nDefault), sPath));
	}

	std::vector<std::string> _GetPrivateProfileSectionNames(const std::filesystem::path& sPath)
	{
		static char szBuffer[256];
		GetPrivateProfileSectionNames(szBuffer, sizeof(szBuffer), sPath.string().c_str());

		char* p = szBuffer;

		std::vector<std::string> vRet;

		std::string x = p;

		vRet.push_back(x);

		for (int i = 0; i < 256; i++)
		{
			p++;
			if (*p == 0)
			{
				x = p + 1;

				if (x == "")
					break;

				vRet.push_back(x);
			}
		}

		return vRet;
	}

}

Params Params::inst;


Params::Params()
{
	bInited = false;
	
	ReadIni();
}

void Params::ReadIni()
{
	static char szIniFile[MAX_PATH];
	sprintf_s(szIniFile, "%s.ini", "ImageManipulations");
	_fullpath(szIniFile, szIniFile, MAX_PATH - 1);

	
	if (!filesystem::exists(szIniFile))
	{
		return;
	}

	string sCalibrIniPath = _Ini::_GetPrivateProfileString("all", "CalibrIniPath", ".\\calibr.ini", szIniFile);


	
	static char szCalibrIniFile[MAX_PATH];
	sprintf_s(szCalibrIniFile, "%s", sCalibrIniPath.c_str());
	_fullpath(szCalibrIniFile, szCalibrIniFile, MAX_PATH - 1);

	if (!filesystem::exists(szCalibrIniFile))
	{
		return;
	}

	bInited = true;

	string sScanner = _Ini::_GetPrivateProfileString("all", "Scanner", "VisIr1", szIniFile);
	
	m_calibr.Rx[0] = _Ini::_GetPrivateProfileDouble(sScanner.c_str(), "Rx00", 0.0, szCalibrIniFile);
	m_calibr.Rx[1] = _Ini::_GetPrivateProfileDouble(sScanner.c_str(), "Rx01", 0.0, szCalibrIniFile);
	m_calibr.Rx[2] = _Ini::_GetPrivateProfileDouble(sScanner.c_str(), "Rx02", 0.0, szCalibrIniFile);

	m_calibr.kSx[0] = _Ini::_GetPrivateProfileDouble(sScanner.c_str(), "kSx00", 1.0, szCalibrIniFile);
	m_calibr.kSx[1] = _Ini::_GetPrivateProfileDouble(sScanner.c_str(), "kSx01", 1.0, szCalibrIniFile);
	m_calibr.kSx[2] = _Ini::_GetPrivateProfileDouble(sScanner.c_str(), "kSx02", 1.0, szCalibrIniFile);

	m_calibr.Dy[0] = _Ini::_GetPrivateProfileDouble(sScanner.c_str(), "Dy00", 0.0, szCalibrIniFile);
	m_calibr.Dy[1] = _Ini::_GetPrivateProfileDouble(sScanner.c_str(), "Dy01", 0.0, szCalibrIniFile);
	m_calibr.Dy[2] = _Ini::_GetPrivateProfileDouble(sScanner.c_str(), "Dy02", 0.0, szCalibrIniFile);

	m_calibr.kSy[0] = _Ini::_GetPrivateProfileDouble(sScanner.c_str(), "kSy00", 1.0, szCalibrIniFile);
	m_calibr.kSy[1] = _Ini::_GetPrivateProfileDouble(sScanner.c_str(), "kSy01", 1.0, szCalibrIniFile);
	m_calibr.kSy[2] = _Ini::_GetPrivateProfileDouble(sScanner.c_str(), "kSy02", 1.0, szCalibrIniFile);


	m_profile.sProfilePath = _Ini::_GetPrivateProfileString("all", "Profile", "./123.txt", szIniFile);
	m_profile.nRad = _Ini::_GetPrivateProfileInt("all", "Rad", 1, szIniFile);
	string sType = _Ini::_GetPrivateProfileString("all", "ScaleType", "0", szIniFile);

	m_profile.nType = 0;

	if (sType == "a")
	{
		m_profile.nType = 1;
	}
	if (sType == "b")
	{
		m_profile.nType = 2;
	}
	if (sType == "c")
	{
		m_profile.nType = 3;
	}


	m_profile.Kx_Orig = _Ini::_GetPrivateProfileDouble("all", "Kx_Orig", 1.0, szIniFile);
	m_profile.Kx_Result = _Ini::_GetPrivateProfileDouble("all", "Kx_Result", 1.0, szIniFile);
	m_profile.Ky_Orig = _Ini::_GetPrivateProfileDouble("all", "Ky_Orig", 1.0, szIniFile);


	m_profile.Colour_To_Bg_Coef = _Ini::_GetPrivateProfileDouble("all", "Colour_To_Bg_Coef", 0.9, szIniFile);;
	m_profile.Bg_To_Colour_Coef = _Ini::_GetPrivateProfileDouble("all", "Bg_To_Colour_Coef", 0.7, szIniFile);;
}