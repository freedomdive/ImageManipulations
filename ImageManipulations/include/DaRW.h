#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DARW_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DARW_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DARW_EXPORTS
#define DARW_API __declspec(dllexport)
#else
#define DARW_API __declspec(dllimport)
#endif

#include "dataarea.h"

//Результаты
#define DARW_OK			0
#define DARW_NOFILE		1
#define DARW_BADARG		2
#define DARW_RWERROR	3
#define DARW_MEMERROR	4


//Чтение области из файла
DARW_API int DataAreaRead(PDataArea_t pDa, char *pszFileName, int nMaxSize);
//Чтение области из файла
//Область будет автоматически выделена с требуемым размером
//В таком случае после использования нужно будет вызывать для нее DataAreaDestroy
DARW_API int DataAreaReadAlloc(PDataArea_t pDa, char *pszFileName);
//Запись области в файл
DARW_API int DataAreaWrite(PDataArea_t pDa, char *pszFileName);
//Освобождение области, выделенной внутри DataAreaReadAlloc
DARW_API void DataAreaDestroy(PDataArea_t pDa);


