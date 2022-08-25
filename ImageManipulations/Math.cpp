#include "stdafx.h"
#include "Math.h"
#include "Params.h"
#include <chrono> // для функций из std::chrono
#include <filesystem>
#include <map>
#include <thread>
using namespace std;



class Timer
{
private:
	// Псевдонимы типов используются для удобного доступа к вложенным типам
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1>>;

	inline static std::chrono::time_point<clock_t> m_beg;

public:
	static void Start()
	{
		m_beg = clock_t::now();
	}

	static double End()
	{
		return 1000 * std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};


ImageArea Math::ShiftImages(int shiftX, int shiftY)
{
	ImageArea pDaOut = pDaIn;

	pDaOut.pBuffer = make_shared<unsigned char[]>(pDaIn.nOffset * pDaIn.nHeight);

	int shift = 0;

	for (int y = 0; y < pDaIn.nHeight; y++)
	{
		for (int x = 0; x < pDaIn.nWidth; x++)
		{
			if (x - shiftX >= 0 && x - shiftX < pDaIn.nWidth && y - shiftY >= 0 && y - shiftY < pDaIn.nHeight)
			{
				for (int p = 0; p < pDaIn.nPlants; p++)
				{
					pDaOut.pBuffer[shift + p] = pDaIn.pBuffer[shift - shiftX * pDaIn.nPlants - pDaIn.nOffset * shiftY + p];
				}
			}
			shift += pDaIn.nPlants;
		}
	}


	return pDaOut;
}

ImageArea Math::Make_3_1_Converting(int nPlant)
{
	ImageArea pDaOut = pDaIn;

	pDaOut.DeepCopyFrom(pDaIn);

	if (pDaIn.nPlants == 1)
	{
		pDaOut.ConvertMonochrome();
	}
	else
	{
		pDaOut.ConvertToMonochrome(nPlant);
	}
	return pDaOut;
}

//Потом может быть сделаю отдельный файл для этого, anyway никто кроме меня не будет пользоваться этой утилитой
void Math::CreatePalette()
{
	if (bPaletteCreated)
		return;

	PALETTE[0] = { 0, 0, 0 };
	PALETTE[1] = { 79, 1, 36 };

	for (int i = 2; i < 256; i++)
	{
		PALETTE[i].R = static_cast<unsigned char>((static_cast<int>(PALETTE[i - 1].R) + 179) % 256);
		PALETTE[i].G = static_cast<unsigned char>((static_cast<int>(PALETTE[i - 1].G) + 179) % 256);
		PALETTE[i].B = static_cast<unsigned char>((static_cast<int>(PALETTE[i - 1].B) + 179) % 256);
	}

	bPaletteCreated = true;
}

ImageArea Math::CalcPalette()
{
	CreatePalette();

	ImageArea pDaOut = pDaIn;

	pDaOut.nPlants = 3;
	pDaOut.nOffset = pDaIn.nWidth * 3;

	pDaOut.pBuffer = make_shared<unsigned char[]>(pDaOut.nOffset * pDaOut.nHeight);

	int shift = 0;
	int shift3 = 0;

	for (int y = 0; y < pDaIn.nHeight; y++)
	{
		for (int x = 0; x < pDaIn.nWidth; x++)
		{
			pDaOut.pBuffer[shift3 + 0] = PALETTE[pDaIn.pBuffer[shift]].R;
			pDaOut.pBuffer[shift3 + 1] = PALETTE[pDaIn.pBuffer[shift]].G;
			pDaOut.pBuffer[shift3 + 2] = PALETTE[pDaIn.pBuffer[shift]].B;

			pDaIn.IncX(shift);
			pDaOut.IncX(shift3);
		}
	}


	return pDaOut;
}

ImageArea Math::AlphaChannelDelete()
{
	ImageArea DaOut = pDaIn;

	DaOut.DeepCopyFrom(pDaIn);

	DaOut.AlphaChannelDelete();

	return DaOut;
}

#include <fstream>

ImageArea Math::AcceptCalibrIni()
{
	
	const bool bOriginal = false;
	
	ImageArea DaOut;

	Timer::Start();
	
	if (bOriginal)
	{
		AcceptCalibrIniOriginal(DaOut);
	}
	else
	{
		AcceptCalibrIniOptimal(DaOut);
	}
	
	int nRet = Timer::End();


	return DaOut;
}

void Math::AcceptCalibrIniOriginal(ImageArea& DaOut)
{
	double kx[3], ky[3];

	int nPlants = pDaIn.nPlants;

	for (int iC = 0; iC < nPlants; iC++)
	{
		if (Params::GetCalibr().kSy[iC] >= 1.0)
			ky[iC] = Params::GetCalibr().kSy[iC];
		else
			ky[iC] = 1.0 / Params::GetCalibr().kSy[iC];

		if (Params::GetCalibr().kSx[iC] >= 1.0)
			kx[iC] = Params::GetCalibr().kSx[iC];
		else
			kx[iC] = 1.0 / Params::GetCalibr().kSx[iC];
	}


	for (int i = 0; i < nPlants; i++)
	{
		Params::GetCalibr().kSy[i] = 1.0;
		Params::GetCalibr().Dy[i] = 0.0;
	}

	DaOut = pDaIn;

	DaOut.pBuffer = make_shared<unsigned char[]>(pDaIn.nOffset * pDaIn.nHeight);

	PUCHAR pBuffer = DaOut.pBuffer.get();

	auto pIn = pDaIn.pBuffer.get();

	auto nOutRows = pDaIn.nHeight;
	auto nOutCols = pDaIn.nWidth;

	int nInStepY = nOutCols * nPlants;



	//Вычислить значения смещений в каждой точке
	double dy;

	int** nY0_S = new int* [nOutRows];
	for (int i = 0; i < nOutRows; i++)
	{
		nY0_S[i] = new int[3];

		for (int j = 0; j < 3; j++)
		{
			nY0_S[i][j] = 0;
		}
	}
	//int nY0_S[nOutRows][3] = { 0 };
	int nDY_S[3];	//Шаг на 1 по Y по исходному изображению
	for (int iC = 0; iC < nPlants; iC++)
	{
		for (int x = 0; x < nOutCols; x++)
		{
			dy = Params::GetCalibr().Dy[iC];
			nY0_S[x][iC] = -dy * 65536.0;
		}
		//nDY_S[iC] = (int)(65536.0/fKSY[nCameraNum][iC] + 0.5);

		nDY_S[iC] = (int)(65536.0 * ky[iC] + 0.5);
	}

	int x0_1, x0_2, y0_1, y0_2, nWX, nWY;	//Координаты исходные

	PUCHAR p, p00, p11, p01, p10;
	UCHAR iC, nC = nPlants;

	for (int y = 0; y < nOutRows; y++)
	{
		p = pBuffer + y * nInStepY;
		//Подготовка смещений по X
		int nX0_S[3];
		int nDX_S[3];	//Шаг на 1 по Y по исходному изображению
		for (int iC = 0; iC < nPlants; iC++)
		{
			nX0_S[iC] = -(Params::GetCalibr().Rx[iC]) * 65536.0;
			//nDX_S[iC] = (int)(65536.0/fKSX[nCameraNum][iC] + 0.5);
			nDX_S[iC] = (int)(65536.0 * kx[iC] + 0.5);
		}

		for (int x = 0; x < nOutCols; x++)
		{
			for (int iC = 0; iC < nPlants; iC++)
			{
				y0_1 = nY0_S[x][iC] >> 16;
				y0_2 = y0_1 + 1;
				x0_1 = nX0_S[iC] >> 16;
				x0_2 = x0_1 + 1;

				if (y0_1 >= 0 && y0_2 < nOutRows && x0_1 >= 0 && x0_2 < nOutCols)	//Значение существует, используем его
				{
					nWX = ((nX0_S[iC] & 0xFFFF) + (1 << 7)) >> 8;	//вес определяется расстоянием
					nWY = ((nY0_S[x][iC] & 0xFFFF) + (1 << 7)) >> 8;	//вес определяется расстоянием

					p00 = pIn + x0_1 * nPlants + y0_1 * nInStepY + iC;
					p01 = p00 + nPlants;
					p10 = p00 + nInStepY;
					p11 = p10 + nPlants;
					*p = ((*p11) * nWY * nWX + (*p01) * (256 - nWY) * nWX + (*p10) * nWY * (256 - nWX) + (*p00) * (256 - nWY) * (256 - nWX) + (1 << 15)) >> 16;	//Усреднение по четырем
				}
				else
				{
					//Копирование с края
					if (y0_1 < 0) y0_1 = 0;
					if (y0_2 >= nOutRows) y0_1 = nOutRows - 1;
					if (x0_1 < 0) x0_1 = 0;
					if (x0_2 >= nOutCols) x0_1 = nOutCols - 1;
					//p00 = pIn + x*nPlants + y*nInStepY + iC;
					p00 = pIn + x0_1 * nPlants + y0_1 * nInStepY + iC;
					*p = *p00;	//Просто копирование на краю
				}
				p++;	//Смещение на следующий цвет
				nY0_S[x][iC] += nDY_S[iC];
				nX0_S[iC] += nDX_S[iC];
			}
		}
	}
}
void Math::AcceptCalibrIniOptimal(ImageArea& DaOut)
{
	double kx[3], ky[3];

	int nPlants = pDaIn.nPlants;

	/*for (int i = 0; i < nPlants; i++)
	{
		Params::GetCalibr().kSy[i] = 1.0;
		Params::GetCalibr().Dy[i] = 0.0;
	}*/
	
	for (int iC = 0; iC < nPlants; iC++)
	{
		if (Params::GetCalibr().kSy[iC] >= 1.0)
			ky[iC] = Params::GetCalibr().kSy[iC];
		else
			ky[iC] = 1.0 / Params::GetCalibr().kSy[iC];

		if (Params::GetCalibr().kSx[iC] >= 1.0)
			kx[iC] = Params::GetCalibr().kSx[iC];
		else
			kx[iC] = 1.0 / Params::GetCalibr().kSx[iC];
		

		//kx[iC] = Params::GetCalibr().kSx[iC];
		//ky[iC] = Params::GetCalibr().kSy[iC];
	}

	bool bDyTransformation = false;
	bool bkSyTransformation = false;
	
	for (int i = 0; i < nPlants; i++)
	{
		if (Params::GetCalibr().Dy[i] != 0.0)
		{
			bDyTransformation = true;
		}

		if (Params::GetCalibr().kSy[i] != 1.0)
		{
			bkSyTransformation = true;
		}
	}

	DaOut = pDaIn;

	DaOut.pBuffer = make_shared<unsigned char[]>(pDaIn.nOffset * pDaIn.nHeight);

	PUCHAR pBuffer = DaOut.pBuffer.get();

	auto pIn = pDaIn.pBuffer.get();

	auto nOutRows = pDaIn.nHeight;
	auto nOutCols = pDaIn.nWidth;

	int nInStepY = nOutCols * nPlants;



	int** nY0_S = new int* [nOutRows];
	for (int i = 0; i < nOutRows; i++)
	{
		nY0_S[i] = new int[3];

		for (int j = 0; j < 3; j++)
		{
			nY0_S[i][j] = 0;
		}
	}
	//int nY0_S[nOutRows][3] = { 0 };
	int nDY_S[3];	//Шаг на 1 по Y по исходному изображению
	for (int iC = 0; iC < nPlants; iC++)
	{
		for (int x = 0; x < nOutCols; x++)
		{
			nY0_S[x][iC] = -(Params::GetCalibr().Dy[iC]) * 65536.0;
		}

		nDY_S[iC] = (int)(65536.0 * ky[iC] + 0.5);
	}

	int x0_1, x0_2, y0_1, y0_2, nWX, nWY;	//Координаты исходные

	PUCHAR p, p00, p11, p01, p10;

	bDyTransformation = false;

	bkSyTransformation = false;

	if (bDyTransformation == true || bkSyTransformation == true)
	{

		for (int y = 0; y < nOutRows; y++)
		{
			p = pBuffer + y * nInStepY;
			//Подготовка смещений по X
			int nX0_S[3];
			int nDX_S[3];	//Шаг на 1 по Y по исходному изображению
			for (int iPlant = 0; iPlant < nPlants; iPlant++)
			{
				nX0_S[iPlant] = -(Params::GetCalibr().Rx[iPlant]) * 65536.0;

				nDX_S[iPlant] = (int)(65536.0 * kx[iPlant] + 0.5);
			}

			for (int x = 0; x < nOutCols; x++)
			{
				for (int iPlant = 0; iPlant < nPlants; iPlant++)
				{
					y0_1 = nY0_S[x][iPlant] >> 16;
					y0_2 = y0_1 + 1;
					x0_1 = nX0_S[iPlant] >> 16;
					x0_2 = x0_1 + 1;

					if (y0_1 >= 0 && y0_2 < nOutRows && x0_1 >= 0 && x0_2 < nOutCols)	//Значение существует, используем его
					{
						nWX = ((nX0_S[iPlant] & 0xFFFF) + (1 << 7)) >> 8;	//вес определяется расстоянием
						nWY = ((nY0_S[x][iPlant] & 0xFFFF) + (1 << 7)) >> 8;	//вес определяется расстоянием

						p00 = pIn + x0_1 * nPlants + y0_1 * nInStepY + iPlant;
						p01 = p00 + nPlants;
						p10 = p00 + nInStepY;
						p11 = p10 + nPlants;
						*p = ((*p11) * nWY * nWX + (*p01) * (256 - nWY) * nWX + (*p10) * nWY * (256 - nWX) + (*p00) * (256 - nWY) * (256 - nWX) + (1 << 15)) >> 16;	//Усреднение по четырем
					}
					else
					{
						//Копирование с края
						if (y0_1 < 0) y0_1 = 0;
						if (y0_2 >= nOutRows) y0_1 = nOutRows - 1;
						if (x0_1 < 0) x0_1 = 0;
						if (x0_2 >= nOutCols) x0_1 = nOutCols - 1;

						p00 = pIn + x0_1 * nPlants + y0_1 * nInStepY + iPlant;
						*p = *p00;	//Просто копирование на краю
					}
					p++;	//Смещение на следующий цвет
					nY0_S[x][iPlant] += nDY_S[iPlant];
					nX0_S[iPlant] += nDX_S[iPlant];
				}
			}
		}
	}
	else if (bDyTransformation == false && bkSyTransformation == false)
	{
		Timer::Start();

		int nDX_S[3];	//Шаг на 1 по Y по исходному изображению

		for (int iPlant = 0; iPlant < nPlants; iPlant++)
		{
			nDX_S[iPlant] = (int)(65536.0 * kx[iPlant] + 0.5);
		}

		vector<thread> vThreads;

		auto vThreadFunction = [&nInStepY, &pBuffer, &nOutCols, &nPlants, &nDX_S, &pIn](int nStartY, int nEndY)
		{
			int xLeft[3];

			int xRight[3];

			int nX0_S[3];	//Подготовка смещений по X

			for (int iPlant = 0; iPlant < nPlants; iPlant++)
			{
				nX0_S[iPlant] = -(Params::GetCalibr().Rx[iPlant]) * 65536.0;

				xLeft[iPlant] = (nX0_S[iPlant] >> 16);

				xRight[iPlant] = min(nOutCols - max(0, -xLeft[iPlant]), ((nOutCols << 16) + nX0_S[iPlant]) / (nDX_S[iPlant]) - 1);
			}


			for (int y = nStartY; y < nEndY; y++)
			{
				int iShift = y * nInStepY;

				for (int iPlant = 0; iPlant < nPlants; iPlant++)
				{
					PUCHAR p = pBuffer + iShift + iPlant;

					nX0_S[iPlant] = -(Params::GetCalibr().Rx[iPlant]) * 65536.0;

					for (int i = 0; i < -xLeft[iPlant]; i++)
					{
						PUCHAR p00 = pIn + iShift + iPlant;
						*p = *p00;	//Просто копирование на краю

						p += nPlants;	//Смещение на следующий цвет

						nX0_S[iPlant] += nDX_S[iPlant];
					}

					for (int x = 0; x < xRight[iPlant]; x++)
					{
						int x0_1 = (nX0_S[iPlant] >> 16);

						int nWX = ((nX0_S[iPlant] & 0xFFFF) + (1 << 7)) >> 8;	//вес определяется расстоянием

						PUCHAR p00 = pIn + x0_1 * nPlants + iShift + iPlant;
						PUCHAR p01 = p00 + nPlants;

						*p = ((*p01) * 256 * nWX + (*p00) * 256 * (256 - nWX) + (1 << 15)) >> 16;	//Усреднение по четырем
						

						p += nPlants;	//Смещение на следующий цвет

						nX0_S[iPlant] += nDX_S[iPlant];
					}

					for (int x = xRight[iPlant]  + max(0, - xLeft[iPlant]); x < nOutCols; x++)
					{
						PUCHAR p00 = pIn + (nOutCols - 1) * nPlants + iShift + iPlant;
						*p = *p00;	//Просто копирование на краю

						p += nPlants;	//Смещение на следующий цвет

					}
				}
			}
		};

		/*vThreads.emplace_back(vThreadFunction, 0, nOutRows / 4);
		vThreads.emplace_back(vThreadFunction, nOutRows / 4, nOutRows / 2);
		vThreads.emplace_back(vThreadFunction, nOutRows / 2, 3 * nOutRows / 4);
		vThreads.emplace_back(vThreadFunction, 3 * nOutRows / 4, nOutRows);*/

		vThreads.emplace_back(vThreadFunction, 0, nOutRows);

		for (auto& it : vThreads)
		{
			it.join();
		}


		int nRet = Timer::End();

		ofstream f("./info/info.txt");

		f << nRet << endl;

		f.close();
	}
	
}


int Math::ReFormatImages(const string& sInputDirectory, const std::string& sFormatIn, const std::string& sFormatOut)
{
	int nCount = 0;

	std::string sFormatStringIn = sFormatIn;

	sFormatStringIn = "." + sFormatStringIn;

	using namespace std::filesystem;

	for (auto& sPath : directory_iterator(sInputDirectory))
	{
		if (sPath.is_regular_file())
		{
			auto t = sPath.path().extension();

			if (sPath.path().extension() == sFormatStringIn)
			{
				ImageArea pDa;


				if (pDa.Read(sPath.path().string().c_str()) == ImageArea::OK)
				{
					std::string sFileName = sPath.path().string();

					sFileName = sFileName.substr(0, sFileName.length() - 3);

					sFileName += sFormatOut;

					if (pDa.Write(sFileName.c_str()) == ImageArea::OK)
					{
						nCount++;
					}
					
				}

			}
		}
	}

	return nCount;
}

ImageArea Math::ScaleImage(const int nWidth, const int nHeight, const bool bIndexed)
{
	Prophile pProphile;

	pProphile.Init(Params::GetProfile().sProfilePath);

	ImageArea pDaOut;

	if (bIndexed)
	{
		if (Params::GetProfile().nType == 0)
		{
			Prophile pFake;

			pFake.bInited = true;

			pFake.fProphile[0] = nHeight;

			pDaOut = ScaleImageIndexed(pDaIn, nWidth, nHeight, static_cast<double>(pDaIn.nWidth) / nWidth, pDaIn.nHeight, pFake);
		}
		if (Params::GetProfile().nType == 1)
		{
			pDaOut = ScaleImageIndexed(pDaIn, nWidth, nHeight, Params::GetProfile().Kx_Orig / Params::GetProfile().Kx_Result, Params::GetProfile().Ky_Orig, pProphile);
		}
		if (Params::GetProfile().nType == 2)
		{
			pDaOut = FormMiddleIndexes(pDaIn, Params::GetProfile().nRad);

			pDaOut = ScaleImageNotIndexed(pDaOut, nWidth, nHeight, Params::GetProfile().Kx_Orig / Params::GetProfile().Kx_Result, Params::GetProfile().Ky_Orig, pProphile);
		}
		if (Params::GetProfile().nType == 3)
		{
			
			pDaOut = ScaleImageNotIndexed(pDaIn, nWidth, nHeight, Params::GetProfile().Kx_Orig / Params::GetProfile().Kx_Result, Params::GetProfile().Ky_Orig, pProphile);

			pDaOut = FormMiddleIndexes(pDaOut, Params::GetProfile().nRad);
		}

	}
	else
	{
		pDaOut = ScaleImageNotIndexed(pDaIn, nWidth, nHeight, Params::GetProfile().Kx_Orig / Params::GetProfile().Kx_Result, Params::GetProfile().Ky_Orig, pProphile);
	}
	
	return pDaOut;
}

RGB Math::GetScaledPixel(ImageArea& pDa, double fStartX, double fEndX, double fStartY, double fEndY)
{
	int iStartPixX = static_cast<int>(fStartX);

	int iEndPixX = static_cast<int>(fEndX) + 1;

	double lLeftValue = 1 - (fStartX - iStartPixX);

	double lRightValue = 1 + fEndX - iEndPixX;

	int iStartPixY = static_cast<int>(fStartY);

	int iEndPixY = static_cast<int>(fEndY) + 1;

	double lTopValue = 1 - (fStartY - iStartPixY);

	double lBottomValue = 1 + fEndY - iEndPixY;

	double iValueY[3]{0};

	double nCountY = 0;

	for (int y = iStartPixY; y <= min(iEndPixY, pDaIn.nHeight - 1); y++)
	{
		double iCoef = 1.0;
		if (y == iStartPixY)
			iCoef = lTopValue;
		if (y == iEndPixY)
			iCoef = lBottomValue;

		if (iCoef == 0)
			continue;

		int iShift = y * pDa.nOffset + iStartPixX * pDa.nPlants;

		double iValueX[3]{0};

		for (int iPlant = 0; iPlant < pDa.nPlants; iPlant++)
		{
			iValueX[iPlant] = lLeftValue * pDa.pBuffer[iShift + iPlant] + lRightValue * pDa.pBuffer[iShift + iPlant + (iEndPixX - iStartPixX) * pDaIn.nPlants];

			int nCountX = 0;

			for (int iInner = 0; iInner < iEndPixX - iStartPixX - 1; iInner++)
			{
				iValueX[iPlant] += pDa.pBuffer[iShift + iPlant + (iInner + 1) * pDa.nPlants];

				nCountX++;
			}

			iValueX[iPlant] /= nCountX + lLeftValue + lRightValue;


			iValueY[iPlant] += iCoef * iValueX[iPlant];
		}

		nCountY += iCoef;
	}

	for (int i = 0; i < pDa.nPlants; i++)
	{
		iValueY[i] /= nCountY;
	}

	return {static_cast<unsigned char>(iValueY[0] + 0.5), static_cast<unsigned char>(iValueY[1] + 0.5), static_cast<unsigned char>(iValueY[2] + 0.5) };
}

//Простое снижение
ImageArea Math::ScaleImageNotIndexed(ImageArea& pDaIn, const int nWidth, const int nHeight, const double Cx, const double CyOrig, Prophile& pProphile)
{
	ImageArea pDaOut(nWidth, nHeight, pDaIn.nPlants);

	//const double fStepX = static_cast<double>(pDaIn.nWidth) / pDaOut.nWidth;
	//const double fStepY = static_cast<double>(pDaIn.nHeight) / pDaOut.nHeight;

	const double fStepX = Cx;

	double fStepY = 1.0;

	int iShift = 0;

	double YOrig = 0;

	for (int y = 0; y < pDaOut.nHeight; y++)
	{
		double XOrig = 0;

		if (pProphile.bInited)
		{
			for (auto& it : pProphile.fProphile)
			{
				if (y >= it.first)
				{
					fStepY = CyOrig / it.second;
				}
			}
		}

		for (int x = 0; x < pDaOut.nWidth; x++)
		{
			//const auto lColour = GetScaledPixel(pDaIn, XOrig, XOrig + fStepX, YOrig, YOrig + fStepY);

			RGB lColour = { 0, 0, 0 };

			int mY = (int)(YOrig + fStepY / 2 + 0.5);

			int mX = (int)(XOrig + fStepX / 2 + 0.5);

			if (mY < 0)
				mY = 0;
			if (mY >= pDaIn.nHeight)
				mY = pDaIn.nHeight - 1;

			if (mX < 0)
				mX = 0;
			if (mX >= pDaIn.nWidth)
				mX = pDaIn.nWidth - 1;

			

			if (pDaIn.nPlants == 3)
			{
				auto iShiftLocal = mY * pDaIn.nOffset + mX * pDaIn.nPlants;

				lColour.R = pDaIn.pBuffer[iShiftLocal];
				lColour.G = pDaIn.pBuffer[iShiftLocal + 1];
				lColour.B = pDaIn.pBuffer[iShiftLocal + 2];

				pDaOut.pBuffer[iShift + 0] = lColour.R;
				pDaOut.pBuffer[iShift + 1] = lColour.G;
				pDaOut.pBuffer[iShift + 2] = lColour.B;
			}
			else
			{
				lColour.R = pDaIn.pBuffer[mY * pDaIn.nOffset + mX];
				pDaOut.pBuffer[iShift + 0] = lColour.R;
			}

			XOrig += fStepX;
			pDaOut.IncX(iShift);
		}

		YOrig += fStepY;
	}

	return pDaOut;
}


ImageArea Math::ScaleImageNotIndexed(ImageArea& pDaIn, const int nWidth, const int nHeight)
{
	ImageArea pDaOut(nWidth, nHeight, pDaIn.nPlants);

	const double fStepX = static_cast<double>(pDaIn.nWidth) / pDaOut.nWidth;
	const double fStepY = static_cast<double>(pDaIn.nHeight) / pDaOut.nHeight;

	int iShift = 0;

	double YOrig = 0;

	for (int y = 0; y < pDaOut.nHeight; y++)
	{
		double XOrig = 0;

		for (int x = 0; x < pDaOut.nWidth; x++)
		{
			const auto lColour = GetScaledPixel(pDaIn, XOrig, XOrig + fStepX, YOrig, YOrig + fStepY);

			pDaOut.pBuffer[iShift + 0] = lColour.R;
			pDaOut.pBuffer[iShift + 1] = lColour.G;
			pDaOut.pBuffer[iShift + 2] = lColour.B;

			XOrig += fStepX;
			pDaOut.IncX(iShift);
		}

		YOrig += fStepY;
	}

	return pDaOut;
}

int Math::GetScaledIndexedPixel(ImageArea& pDa, double fStartX, double fEndX, double fStartY, double fEndY)
{
	int iStartPixX = static_cast<int>(fStartX);

	int iEndPixX = static_cast<int>(fEndX) + 1;

	double lLeftValue = 1 - (fStartX - iStartPixX);

	double lRightValue = 1 + fEndX - iEndPixX;

	int iStartPixY = static_cast<int>(fStartY);

	int iEndPixY = static_cast<int>(fEndY) + 1;

	double lTopValue = 1 - (fStartY - iStartPixY);

	double lBottomValue = 1 + fEndY - iEndPixY;

	map<int, double> mFrequancy;

	for (int y = iStartPixY; y <= min(iEndPixY, pDa.nHeight - 1); y++)
	{
		double iCoefY = 1.0;

		if (y == iStartPixY)
			iCoefY = lTopValue;
		if (y == iEndPixY)
			iCoefY = lBottomValue;

		int iShift = y * pDa.nOffset + iStartPixX;

		for (int x = iStartPixX; x <= iEndPixX; x++)
		{
			double iCoefX = 1.0;

			if (x == iStartPixX)
				iCoefX = lLeftValue;
			if (x == iEndPixX)
				iCoefX = lRightValue;

			auto iColour = pDa.pBuffer[iShift];

			mFrequancy[iColour] += iCoefX * iCoefY;

			iShift++;
		}
	}

	double nMaxFrequancy = 0.0;
	int iMaxIndex = 0;

	int nCount = 0;

	for (auto& it : mFrequancy)
	{
		if (nMaxFrequancy < it.second && it.first > 0)
		{
			nMaxFrequancy = it.second;
			iMaxIndex = it.first;
		}

		nCount += it.second;
	}


	if (mFrequancy[0] > Params::GetProfile().Colour_To_Bg_Coef * nCount)
	{
		if (mFrequancy[0] > nMaxFrequancy)
		{
			nMaxFrequancy = mFrequancy[0];
			iMaxIndex = 0;
		}
	}

	if (iMaxIndex == 0)
	{
		if (mFrequancy[0] > Params::GetProfile().Bg_To_Colour_Coef * nCount)
		{
			iMaxIndex = 0;
		}
		else
		{
			nMaxFrequancy = 0.0;
			iMaxIndex = 0;

			for (auto& it : mFrequancy)
			{
				if (it.first != 0 && nMaxFrequancy < it.second)
				{
					nMaxFrequancy = it.second;
					iMaxIndex = it.first;
				}
			}
		}
	}

	return iMaxIndex;
}


ImageArea Math::ScaleImageIndexed(ImageArea& pDaIn, const int nWidth, const int nHeight, const double Cx, const double CyOrig, Prophile& pProphile)
{
	ImageArea pDaOut(nWidth, nHeight, 1);

	//const double fStepX = static_cast<double>(pDaIn.nWidth) / pDaOut.nWidth;
	//const double fStepY = static_cast<double>(pDaIn.nHeight) / pDaOut.nHeight;

	const double fStepX = Cx;

	double fStepY = 1.0;

	int iShift = 0;

	double YOrig = 0;

	for (int y = 0; y < pDaOut.nHeight; y++)
	{
		double XOrig = 0;

		if (pProphile.bInited)
		{
			for (auto& it : pProphile.fProphile)
			{
				if (y >= it.first)
				{
					fStepY = CyOrig / it.second;
				}
			}
		}

		for (int x = 0; x < pDaOut.nWidth; x++)
		{
			const auto iIndex = GetScaledIndexedPixel(pDaIn, XOrig, XOrig + fStepX, YOrig, YOrig + fStepY);

			pDaOut.pBuffer[iShift + 0] = iIndex;

			XOrig += fStepX;
			pDaOut.IncX(iShift);
		}

		YOrig += fStepY;
	}

	return pDaOut;
}


//Пройтись по индексному файлу и выбрать частый
ImageArea Math::FormMiddleIndexes(ImageArea& pDa, int nRad)
{
	ImageArea pDaOut = pDa;

	pDaOut.DeepCopyFrom(pDa);


	for (int y = nRad; y < pDa.nHeight - nRad; y++)
	{
		int iShift = y * pDa.nWidth + nRad;

		for (int x = nRad; x < pDa.nWidth - nRad; x++)
		{

			int nCount = 0;
			map<unsigned char, int> mFrequancy;
			for (int j = -nRad; j <= nRad; j++)
			{
				for (int i = -nRad; i <= nRad; i++)
				{
					auto nPix = pDa.pBuffer[iShift + j * pDa.nWidth + i];
					mFrequancy[nPix] += 2 * nRad - abs(j) - abs(i);

					nCount += 2 * nRad - abs(j) - abs(i);
				}
			}

			unsigned char nIndex = 0;

			int nMaxFrequancy = 0;

			for (auto& it : mFrequancy)
			{
				if (it.second > nMaxFrequancy && it.first > 0)
				{
					nMaxFrequancy = it.second;
					nIndex = it.first;
				}
			}

			if (mFrequancy[0] > Params::GetProfile().Colour_To_Bg_Coef * nCount)
			{
				if (mFrequancy[0] > nMaxFrequancy)
				{
					nMaxFrequancy = mFrequancy[0];
					nIndex = 0;
				}
			}

			if (pDa.pBuffer[iShift] == 0)
			{
				if (mFrequancy[0] > Params::GetProfile().Bg_To_Colour_Coef * nCount)
				{
					nIndex = 0;
				}
			}

			pDaOut.pBuffer[iShift] = nIndex;

			iShift++;
		}
	}

	return pDaOut;
}
