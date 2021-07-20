#pragma once
#include "DaRW.h"

typedef struct
{
	UCHAR R;
	UCHAR G;
	UCHAR B;
} RGB;

class Calculate
{

public:
	void SetInputPath(char * str);
	void SetOutputPath(char * str);

	int LoadInputImage();

	int shiftImages(int shiftX, int shiftY);

	int make_3_1_converting(int nPlant);

	int alphaChannelDelete();
	
	void createPalette();

	int calcPalette();

	int GetDaInPlants() const { return DaIn.Plants; }

private:
	DataArea_t DaIn;

	char InputPath[MAX_PATH];
	char OutputPath[MAX_PATH];

	RGB PALETTE[256];
};