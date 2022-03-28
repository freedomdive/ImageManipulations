#include "stdafx.h"
#include "Calculate.h"
#include "Params.h"
#include <chrono> // для функций из std::chrono


class Timer
{
private:
	// Псевдонимы типов используются для удобного доступа к вложенным типам
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1>>;

	std::chrono::time_point<clock_t> m_beg;

public:
	Timer() : m_beg(clock_t::now())
	{
	}

	void reset()
	{
		m_beg = clock_t::now();
	}

	double elapsed() const
	{
		return 1000 * std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

void Calculate::SetInputPath(char* str)
{
	strcpy_s(InputPath, MAX_PATH, str);
}

void Calculate::SetOutputPath(char* str)
{
	strcpy_s(OutputPath, MAX_PATH, str);
}

int Calculate::LoadInputImage()
{
	DataAreaDestroy(&DaIn);
	return DataAreaReadAlloc(&DaIn, InputPath);
}

int Calculate::shiftImages(int shiftX, int shiftY)
{
	DataArea_t DaOut = DaIn;

	DaOut.StartAddress = new UCHAR[DaIn.Offset * DaIn.Rows];

	int shift = 0;

	for (int y = 0; y < DaIn.Rows; y++)
	{
		for (int x = 0; x < DaIn.Cols; x++)
		{
			if (x - shiftX >= 0 && x - shiftX < DaIn.Cols && y - shiftY >= 0 && y - shiftY < DaIn.Rows)
			{
				for (int p = 0; p < DaIn.Plants; p++)
				{
					DaOut.StartAddress[shift + p] = DaIn.StartAddress[shift - shiftX * DaIn.Plants - DaIn.Offset * shiftY + p];
				}
			}
			shift += DaIn.Plants;
		}
	}

	const int ret = DataAreaWrite(&DaOut, OutputPath);

	delete[] DaOut.StartAddress;

	return ret;
}

int Calculate::make_3_1_converting(int nPlant)
{
	if (DaIn.Plants == 1)
	{
		DataArea_t DaOut = DaIn;

		DaOut.Plants = 3;
		DaOut.Offset *= 3;
		DaOut.StartAddress = new UCHAR[DaIn.Cols * DaIn.Rows * 3];

		int shift = 0;
		int shift3 = 0;

		for (int y = 0; y < DaIn.Rows; y++)
		{
			for (int x = 0; x < DaIn.Cols; x++)
			{
				DaOut.StartAddress[shift3] = DaIn.StartAddress[shift];
				DaOut.StartAddress[shift3 + 1] = DaIn.StartAddress[shift];
				DaOut.StartAddress[shift3 + 2] = DaIn.StartAddress[shift];

				shift3 += 3;
				shift++;
			}
		}

		int ret = DataAreaWrite(&DaOut, OutputPath);

		delete[] DaOut.StartAddress;

		return ret;
	}
	if (DaIn.Plants == 3)
	{
		DataArea_t DaOut = DaIn;
		DaOut.Plants = 1;
		DaOut.Offset /= 3;
		DaOut.StartAddress = new UCHAR[DaIn.Cols * DaIn.Rows];

		int shift3 = 0;
		int shift = 0;

		for (int y = 0; y < DaIn.Rows; y++)
		{
			for (int x = 0; x < DaIn.Cols; x++)
			{
				DaOut.StartAddress[shift] = DaIn.StartAddress[shift3 + nPlant];
				shift++;
				shift3 += 3;
			}
		}

		int ret = DataAreaWrite(&DaOut, OutputPath);

		delete[] DaOut.StartAddress;

		return ret;
	}
}

//Потом может быть сделаю отдельный файл для этого, anyway никто кроме меня не будет пользоваться этой утилитой
void Calculate::createPalette()
{
	PALETTE[0] = { 0, 0, 0 };
	PALETTE[1] = { 237, 28, 36 };
	PALETTE[2] = { 60, 28, 238 };
	PALETTE[3] = { 34, 177, 76 };
	PALETTE[4] = { 254, 235, 52 };
	PALETTE[5] = { 163, 73, 164 };
	PALETTE[6] = { 255, 128, 64 };
	PALETTE[7] = { 200, 191, 231 };
	PALETTE[8] = { 112, 146, 190 };
	PALETTE[9] = { 185, 122, 87 };

	for (int i = 10; i < 256; i++)
		PALETTE[i] = { 255, 255, 255 };
}

int Calculate::calcPalette()
{
	createPalette();

	DataArea_t DaOut = DaIn;

	DaOut.Plants = 3;
	DaOut.Offset = DaIn.Cols * 3;

	DaOut.StartAddress = new UCHAR[DaOut.Offset * DaOut.Rows];

	int shift = 0;
	int shift3 = 0;

	for (int y = 0; y < DaIn.Rows; y++)
	{
		for (int x = 0; x < DaIn.Cols; x++)
		{
			DaOut.StartAddress[shift3 + 0] = PALETTE[DaIn.StartAddress[shift]].R;
			DaOut.StartAddress[shift3 + 1] = PALETTE[DaIn.StartAddress[shift]].G;
			DaOut.StartAddress[shift3 + 2] = PALETTE[DaIn.StartAddress[shift]].B;

			shift++;
			shift3 += 3;
		}
	}


	int ret = DataAreaWrite(&DaOut, OutputPath);

	delete[] DaOut.StartAddress;

	return ret;
}

int Calculate::alphaChannelDelete()
{
	DataArea_t DaOut = DaIn;

	DaOut.Plants = 3;
	DaOut.Offset = DaIn.Cols * 3;

	DaOut.StartAddress = new UCHAR[DaOut.Offset * DaOut.Rows];

	int shift4 = 0;
	int shift3 = 0;

	for (int y = 0; y < DaIn.Rows; y++)
	{
		for (int x = 0; x < DaIn.Cols; x++)
		{
			DaOut.StartAddress[shift3 + 0] = DaIn.StartAddress[shift4 + 2];
			DaOut.StartAddress[shift3 + 1] = DaIn.StartAddress[shift4 + 1];
			DaOut.StartAddress[shift3 + 2] = DaIn.StartAddress[shift4 + 0];

			shift3 += 3;
			shift4 += 4;
		}
	}

	int ret = DataAreaWrite(&DaOut, OutputPath);

	delete[] DaOut.StartAddress;

	return ret;
}

int Calculate::AcceptCalibrIni()
{
	
	const bool bOriginal = false;
	
	DataArea_t DaOut;

	Timer T;
	
	if (bOriginal)
	{
		AcceptCalibrIniOriginal(DaOut);
	}
	else
	{
		AcceptCalibrIniOptimal(DaOut);
	}
	
	int nRet = T.elapsed();
	
	DataAreaWrite(&DaOut, OutputPath);
	delete[] DaOut.StartAddress;

	return nRet;
}

void Calculate::AcceptCalibrIniOriginal(DataArea_t& DaOut)
{
	double kx[3], ky[3];

	int nPlants = DaIn.Plants;

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

	DaOut = DaIn;

	DaOut.StartAddress = new UCHAR[DaIn.Offset * DaIn.Rows];

	PUCHAR pBuffer = DaOut.StartAddress;

	auto pIn = DaIn.StartAddress;

	auto nOutRows = DaIn.Rows;
	auto nOutCols = DaIn.Cols;

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
void Calculate::AcceptCalibrIniOptimal(DataArea_t& DaOut)
{
	double kx[3], ky[3];

	int nPlants = DaIn.Plants;

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

	DaOut = DaIn;

	DaOut.StartAddress = new UCHAR[DaIn.Offset * DaIn.Rows];

	PUCHAR pBuffer = DaOut.StartAddress;

	auto pIn = DaIn.StartAddress;

	auto nOutRows = DaIn.Rows;
	auto nOutCols = DaIn.Cols;

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
		for (int y = 0; y < nOutRows; y++)
		{
			int iShift = y * nInStepY;
			
			p = pBuffer + iShift;
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
					x0_1 = nX0_S[iPlant] >> 16;
					x0_2 = x0_1 + 1;

					if (x0_1 >= 0 && x0_2 < nOutCols)	//Значение существует, используем его
					{
						nWX = ((nX0_S[iPlant] & 0xFFFF) + (1 << 7)) >> 8;	//вес определяется расстоянием

						p00 = pIn + x0_1 * nPlants + iShift + iPlant;
						p01 = p00 + nPlants;
						
						*p = ((*p01) * 256 * nWX + (*p00) * 256 * (256 - nWX) + (1 << 15)) >> 16;	//Усреднение по четырем
					}
					else
					{
						//Копирование с края
						if (x0_1 < 0) x0_1 = 0;
						if (x0_2 >= nOutCols) x0_1 = nOutCols - 1;

						p00 = pIn + x0_1 * nPlants + iShift + iPlant;
						*p = *p00;	//Просто копирование на краю
					}
					p++;	//Смещение на следующий цвет

					nX0_S[iPlant] += nDX_S[iPlant];
				}
			}
		}
	}
	
}