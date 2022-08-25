#pragma once
#include "ImageArea.h"
#include <filesystem>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <map>
#include <vector>
using namespace std;

typedef struct
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
} RGB;

class Prophile
{
public:
	bool bInited = false;

	void Init(std::filesystem::path pFileName)
	{
		if (exists(pFileName))
		{
			ifstream f(pFileName);

			string str;

			while (getline(f, str))
			{
				vector<string> vRes;

				boost::split(vRes, str, [](char c) {return c == '\t'; });

				if (vRes.size() == 2)
				{
					fProphile[stoi(vRes[0])] = stod(vRes[1]);
				}
			}

			f.close();
			bInited = true;
		}
	}


	map<int, double> fProphile;
};	

class Math
{

public:

	static ImageArea ShiftImages(int shiftX, int shiftY);

	static ImageArea Make_3_1_Converting(int nPlant);

	static ImageArea AlphaChannelDelete();
	
	static void CreatePalette();

	static ImageArea CalcPalette();

	static ImageArea AcceptCalibrIni();

	static void AcceptCalibrIniOriginal(ImageArea& DaOut);
	
	static void AcceptCalibrIniOptimal(ImageArea& DaOut);

	static int ReFormatImages(const std::string& sInputDirectory, const std::string& sFormatIn, const std::string& sFormatOut);

	static ImageArea ScaleImage(int nWidth, int nHeight, bool bIndexed);

	static ImageArea ScaleImageNotIndexed(ImageArea& pDaIn, int nWidth, int nHeight);


	static RGB GetScaledPixel(ImageArea& pDa, double fStartX, double fEndX, double fStartY, double fEndY);

	inline static ImageArea pDaIn;

	static int GetScaledIndexedPixel(ImageArea& pDa, double fStartX, double fEndX, double fStartY, double fEndY);

	static ImageArea ScaleImageIndexed(ImageArea& pDaIn, const int nWidth, const int nHeight, const double Cx, const double CyOrig, Prophile& pProphile);

	static ImageArea FormMiddleIndexes(ImageArea& pDa, int nRad);

	static ImageArea ScaleImageNotIndexed(ImageArea& pDaIn, const int nWidth, const int nHeight, const double Cx, const double CyOrig, Prophile& pProphile);
private:

	inline static RGB PALETTE[256];

	inline static bool bPaletteCreated = false;
};