//============================================================================
// ����. ���������� �������, ���������� � �������� GRB
// ������� - ������ 1995.
// ��������� ������������ ������ � �������� �������: ������� 1997.
// ����������� �.�., 12.98
//                          <������������ ����>
//============================================================================

#ifndef __DATAAREA_H__
#define __DATAAREA_H__

#ifdef __cplusplus
  extern "C" {
#endif

#include "Geom.h"

#ifdef MODULEAPI
  #undef MODULEAPI
#endif

#ifndef DFI_EXPORTS
  #define MODULEAPI __declspec(dllimport)
#else
  #define MODULEAPI __declspec(dllexport)
#endif

//----------------------------------------------------------------------------
// ����������� ��� ������� ������������
//----------------------------------------------------------------------------
typedef struct 
{
 
  float k;
  int   b;

} LOOKUPKB, *PLOOKUPKB;

typedef struct
{

  int k;
  int b;

} LOOKUPKB_SOFT, *PLOOKUPKB_SOFT;

//----------------------------------------------------------------------------
// ��������� ����������� GRB �����������
//----------------------------------------------------------------------------
typedef struct 
{

 PUCHAR    StartAddress;
 int       ImageFormat;  // ������� ����������� (GRB_...)
 int       Cols;         // ���������� �������� � ������
 int       Rows;         // ���������� �������� �� ���������
 int       Offset;       // ����� ������ (c ������ ����������) � ������
 int       PixSize;      // �������� ������� � ������, ���� 0-������� �����
 USHORT    Plants;       // ���������� ����������
 USHORT    PixOnPlant;   // ���������� ������ ������ �������� � ���������

 USHORT    PixBit;       // ���������� ������������ ���
 USHORT    PixFormat;    // ������ ������� (0-Intel, 1-��������)
 double    Scale_x;      // ������������������ ��� �����������
 double    Scale_y;

 short     *pRowsShift;
 PLOOKUPKB pLookUp;      // ������� ������������ (��� ���������� = NULL)
                         // pLookUp :== LOOKUPKB[Cols][Plants];

} DataArea_t, *PDataArea_t;

#define DA_ByteOnPixel(pDa) ((pDa)->PixSize*(pDa)->Plants*(pDa)->PixOnPlant)
#define DA_Size(pDa)        ((pDa)->Offset*(pDa)->Rows)
#define DA_BinGetPixOffset(pDa, x, y, iPlant) \
        ((y)*(pDa)->Offset + (pDa)->PixSize*((pDa)->PixOnPlant*((x)/(pDa)->PixOnPlant*(pDa)->Plants + iPlant) + (x) % (pDa)->PixOnPlant))

//----------------------------------------------------------------------------
// ���������� ������� GRB �����������
//----------------------------------------------------------------------------
#define NCOLOR 256
typedef struct
{ 

 unsigned char B;
 unsigned char G;
 unsigned char R;
 unsigned char IR;

} pix, *Ppix;

//----------------------------------------------------------------------------
// ������� �����������
//----------------------------------------------------------------------------
// Normal resolution scanner sizes
#define GRB_NR_H          256                   // number of pixels in row
#define GRB_NR_V          512                   // number of pixels in column
#define GRB_NRT_SIZE      (GRB_NR_H*GRB_NR_V*2) // data size of tone image
#define GRB_NRB_SIZE      (GRB_NRT_SIZE/8)      // data size of b/w  image
// Hi(2N) resolution scanner sizes
//#define GRB_HR_H          512                   // number of pixels in row
#define GRB_HR_V          2048                  // number of pixels in column
#define GRB_HR_H          1024                   // number of pixels in row
//#define GRB_HR_V          512                  // number of pixels in column
#define GRB_HRT_SIZE      (GRB_HR_H*GRB_HR_V*3) // data size of tone image
#define GRB_HRB_SIZE      (GRB_HRT_SIZE/8)      // data size of b/w  image
// �������� �����������
#define GRB_T08P2         0 // ������� �����������,  8 ��� �� ������� ���������, 2 ���������
#define GRB_T10PX         1 // ������� �����������, 10 ��� �� ������� ���������, n ����������
#define GRB_B01P2         2 // ������� �����������,  1 ��� �� ������� ���������, 2 ���������
#define GRB_T08P3         3 // ������� �����������,  8 ��� �� ������� ���������, 3 ��������� (������� RGB)
// ��������� �� ��������� ��� ������ �����������
extern MODULEAPI DataArea_t BinHeader;
extern MODULEAPI DataArea_t GrbHeader;
extern MODULEAPI DataArea_t BitHeader;
extern MODULEAPI DataArea_t RGBHeader; // ������ RGB
//----------------------------------------------------------------------------
// ����� ������ ���������������� ����������� �� �����
//----------------------------------------------------------------------------
// ��� 2-� ������������ ������
#define VISUAL_PLANT     0
#define IR_PLANT         1
#define COLOR_V_PLANT    2
#define TWOINONE_V_PLANT 3
// ��� 3-� ������������(RGB) ������
#define R_PLANT          0
#define G_PLANT          1
#define B_PLANT          2
#define RGB_PLANT      3

//��������� �����������
#define INDEX_IMAGE 11
//----------------------------------------------------------------------------
// ����� ������� GRB
//----------------------------------------------------------------------------
extern MODULEAPI pix
     // dark colors
     Black    ,//{0,     0,   0},
     Blue     ,//{128,   0,   0},
     Green    ,//{0,   128,   0},
     Cyan     ,//{128, 128,   0},
     Red      ,//{0,     0, 128},
     Magenta  ,//{128,   0, 128},
     Brown    ,//{0,   128, 128},
     Gray     ,//{128, 128, 128},
     // light colors
     LBlue    ,//{255,   0,   0},
     LGreen   ,//{0,   255,   0},
     LCyan    ,//{255, 255,   0},
     LRed     ,//{0,     0, 255},
     LMagenta ,//{255,   0, 255},
     Yellow   ,//{0,   255, 255},
     White    ;//{255, 255, 255};

//----------------------------------------------------------------------------
// Draw functions for Data Area
//----------------------------------------------------------------------------
MODULEAPI pix DA_PixMake(unsigned char B, unsigned char G, unsigned char R);

MODULEAPI void DA_Open(PDataArea_t pDa, PUCHAR pBuffer, PDataArea_t pDaHdr);
#define   DA_COPYPARAMS(pDAdst,pDAsrc) ( DA_Open(pDAdst,(pDAdst)->StartAddress,pDAsrc))

// �������� �������� DataArea � ����������� ������������ DataArea
MODULEAPI void DA_OpenChild(PDataArea_t pDa_child, PDataArea_t pDa_parent, 
                       PUCHAR StartAddress, int Rows, int Cols);

typedef union
{ 

 UCHAR Plant[2];
 SHORT All;

} pixF, *PpixF;

MODULEAPI void DA_PutPix   (PDataArea_t pDa, int x, int y, pix p);
MODULEAPI pix  DA_GetPix   (PDataArea_t pDa, int x, int y);
MODULEAPI int  DA_GetVal   (PDataArea_t pDa, int x, int y, int Plant);
#define DA_GetPPix(pDa, x, y) ((pDa)->StartAddress + (y)*(pDa)->Offset + (x)*(pDa)->Plants)

MODULEAPI void DA_Line     (PDataArea_t pDa, int x1, int y1, int x2, int y2, char width, pix c);
MODULEAPI void DA_Rectangle(PDataArea_t pDa, int x1, int y1, int x2, int y2, char width, pix c);
MODULEAPI void DA_Bar      (PDataArea_t pDa, int x1, int y1, int x2, int y2, char width, pix c);

#define DA_LineEx(pDa, Box, BoxColor) \
       (void)DA_Line(pDa, Box.left, Box.top, Box.right, Box.bottom, 1, BoxColor)

#define DA_RectangleEx(pDa, Box, BoxColor) \
       (void)DA_Rectangle(pDa, Box.left, Box.top, Box.right, Box.bottom, 1, BoxColor)

#define DA_BarEx(pDa, Box, BoxColor) \
       (void)DA_Bar(pDa, Box.left, Box.top, Box.right, Box.bottom, 1, BoxColor)

//----------------------------------------------------------------------------
// �������� ������, ������ � �����������
//----------------------------------------------------------------------------
// ����������� ������ �������� � ����
MODULEAPI int DA_SaveToFile  (PDataArea_t pDa, char *namefile);
// ������ ����������� �� �����
MODULEAPI int DA_ReadFromFile(PDataArea_t pDa, char *namefile);
// ����������� ������� ������ ����������� � ������
MODULEAPI void DA_Copy(PDataArea_t pDa_src, PDataArea_t pDa_dest, PRECT pZone);
MODULEAPI void DA_CopyEx(PDataArea_t pDa_src, PDataArea_t pDa_dest, POINT SrcPos, POINT DestPos, POINT Size);
// ����������� ������� ������ ����������� � ������ �������
MODULEAPI void DA_Copy_In_Begin(PDataArea_t pDa_src, PDataArea_t pDa_dest, PRECT pZone);

#ifdef __cplusplus
  }	// extern "C"
#endif

#endif
