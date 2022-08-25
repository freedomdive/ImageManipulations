/*	Аналог DataArea_t
 *	StartAddress на shared_ptr
 *
 *	Всё в виде единого класса
 *	
 *	Зайков М. 2021 г. ООО BIS.
 *
 */
#pragma once

#ifdef IMAGEAREA_EXPORTS
#define IMAGEAREA_API __declspec(dllexport)
#else
#define IMAGEAREA_API __declspec(dllimport)
#endif

#include <memory>
#include <string>


class IMAGEAREA_API ImageArea
{
public:
	int		nWidth;			// Ширина изображения
	int		nHeight;		// Высота изображения
	int		nOffset;		// Длина строки в байтах, по-умолчанию Cols * Plants
	int		nPlants;		// Количество байт на пиксель

	std::shared_ptr<unsigned char[]> pBuffer;	// Содержимое изображения
	
	ImageArea()
	{
		nWidth = 0;
		nHeight = 0;
		nOffset = 0;
		nPlants = 0;
		pBuffer = nullptr;
	}
	
	ImageArea(int nWidth, int nHeight, int nPlants)
	{
		this->nWidth = nWidth;
		this->nHeight = nHeight;
		nOffset = nPlants * nWidth;
		this->nPlants = nPlants;

		if (nOffset * nHeight)
		{
			pBuffer = std::make_shared<unsigned char[]>(nOffset * nHeight);
			memset(pBuffer.get(), unsigned char(0), nOffset * nHeight);
		}
	}

	//Скопировать текущую ImageArea в DaTo с раздельными указателями
	int DeepCopyTo(ImageArea& IaTo);

	//Скопировать из указанной ImageArea DaFrom в текущую с раздельными указателями
	int DeepCopyFrom(const ImageArea& IaFrom);

	//Получить ImageArea размера rc в координатах DaTo, зоны выхода за границы будут nDefault
	int GetSubImageArea(ImageArea& IaTo, const RECT& rc, unsigned char nDefault = 0);
	
	//Удаление альфа канала. Будет работать только с Plants=4
	int AlphaChannelDelete();

	//Преобразование ImageArea в монохромное изображение по конкретной плоскости
	int ConvertToMonochrome(int iPlant = 0);

	//Преобразование монохромной ImageArea в немонохромную с возможным заданием количества итоговых плоскостей
	int ConvertMonochrome(int nPlants = 3);
	

	//Чтение из файла с автоматическим выделением памяти
	int Read(const char* path);

	//Чтение из файла с автоматическим выделением памяти
	int Read(const std::string& path);


	//Запись в файл
	int Write(const char* path);

	//Запись в файл
	int Write(const std::string& path);

	

	//Записать монохромное изображение в .png с отображением пикселей 1, 2, 3, ... -> rnd1, rnd2, rnd3, ...
	int WritePNGRandomPalette(const char* path);

	//Записать монохромное изображение в .png с отображением пикселей 1, 2, 3, ... -> rnd1, rnd2, rnd3, ...
	int WritePNGRandomPalette(const std::string& path);


	enum Errors
	{
		OK,
		NOFILE,
		BADARG,
		RWERROR,
		MEMERROR
	};
	
	/*
	 * Простой способ обхода ImageArea в прямоугольнике RECT rc:
	 *
	 * for (int y = rc.top; y < rc.bottom; y++)
	 * {
	 *		int iShift = Da.GetShift(rc.left, y);
	 * 
	 *		for (int x = rc.left; x < rc.right; x++)
	 *		{
	 *			Da.pBuffer[iShift] - R
	 *			Da.pBuffer[iShift + 1] - G
	 *			...
	 *
	 *
	 *			Da.IncX(iShift);
	 *		}
	 *
	 *		(Либо iShift вызывать до первого for-a и сдвигать iShift после второго for-а на IncY и -= rc.right - rc.left + 1) 
	 * }
	 */
	
	inline int GetShift(int x, int y) const
	{
		return y * nOffset + x * nPlants;
	}

	inline void IncX(int &iShift) const
	{
		iShift += nPlants;
	}

	inline void IncY(int &iShift) const
	{
		iShift += nOffset;
	}

	inline void DexX(int& iShift) const
	{
		iShift -= nPlants;
	}

	inline void DecY(int& iShift) const
	{
		iShift -= nOffset;
	}

	inline bool IsXValid(int x) const
	{
		return x >= 0 && x < nWidth;
	}

	inline bool IsYValid(int y) const
	{
		return y >= 0 && y < nHeight;
	}

	inline bool IsCoordValid(int x, int y) const
	{
		return IsXValid(x) && IsYValid(y);
	}

	//Фиксированные цвета
	enum Colours
	{
		RED,
		GREEN,
		BLUE,
		YELLOW,
		PINK,
		LIGHTBLUE,
		WHITE,
		BLACK,
		GREY
	};

	//Цвета для 3-х плоскостного изображения
	struct Colour
	{
		unsigned char Red;
		unsigned char Green;
		unsigned char Blue;
	};

	//У нижестоящих функций нет проверк на выход за границы для оптимизации. Проверки следует делать в клиенте
	
	//Установка цвета по пресету. Только для 3-х плоскостного изображения
	void PutPix(int iShift, const Colours& eColour);
	//Установка заданного цвета для 3-x плоскостного изображения
	void PutPix(int iShift, const Colour& lColour);
	//Установка заданного цвета для 1-x плоскостного изображения
	void PutPix(int iShift, unsigned char lColour);
	

	//Проверка ImageArea на корректность
	inline bool IsIaValid() const
	{
		return pBuffer && pBuffer.get() && nPlants && nWidth && nHeight && nOffset;
	}

	//Проверка внешней ImageArea на корректность
	static inline bool IsIaValid(const ImageArea& pIa)
	{
		return pIa.pBuffer && pIa.pBuffer.get() && pIa.nPlants && pIa.nWidth && pIa.nHeight && pIa.nOffset;
	}

private:
	//Внутренняя функция чтения
	int DataAreaReadInternal(const char* pszFileName, int nMaxSize = -1);
	//Внутренняя функция записи
	int WriteInternal(const char* path);
	//Записать монохромное изображение в .png с отображением пикселей 1, 2, 3, ... -> rnd1, rnd2, rnd3, ...
	int WritePNGRandomPaletteInner(const char* path);
};

using PImageArea = ImageArea*;

