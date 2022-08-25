#pragma once
#include <string>

class Params
{
protected:
	static Params inst;
	Params();

	typedef struct
	{
		double Rx[3];
		double Dy[3];
		double kSx[3];
		double kSy[3];

	} Calibr;

	typedef struct
	{
		std::string sProfilePath;
		int nType;
		int nRad;

		double Kx_Orig;
		double Kx_Result;
		double Ky_Orig;

		double Colour_To_Bg_Coef;
		double Bg_To_Colour_Coef;

	} Profile;

	Calibr m_calibr{};
	Profile m_profile{};

	void ReadIni();
	
public:
	static const Params& GetInstanceC() { return inst; }
	static Params& GetInstance() { return inst; }
	static Calibr& GetCalibr() { return inst.m_calibr; }
	static Profile& GetProfile() { return inst.m_profile; }

	inline static bool bInited;
};
