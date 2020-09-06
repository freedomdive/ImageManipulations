#include "Calculate.h"

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

