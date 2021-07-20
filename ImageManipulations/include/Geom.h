//============================================================================
// ����. ���������� ������� ������ � ��������������� �����������
// Berenov D A - 1995. ����������� �.�. - 1996 - 2000
//============================================================================
#ifndef __RECT_H__
#define __RECT_H__

#ifdef __cplusplus
  extern "C" {
#endif

#ifdef MODULEAPI
  #undef MODULEAPI
#endif

#ifndef DFI_EXPORTS
  #define MODULEAPI __declspec(dllimport)
#else
  #define MODULEAPI __declspec(dllexport)
#endif

#include <windows.h>

//#define IsNull 0.00001
#define L2_NORM(x1, y1, x2, y2) ((int)(sqrt(pow(((x1) - (x2)), 2) + pow(((y1) - (y2)), 2)) + 0.5))

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ������ � ���������
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//  ���������� �������������
MODULEAPI RECT Dfi_RectMake(int left, int top, int right, int bottom);

// �������� ������� (��������� ������)
#define Dfi_RectWithOffsetS(pR, pP) \
        Dfi_RectMake((pR)->left - (pP)->x, (pR)->top - (pP)->y, (pR)->right - (pP)->x, (pR)->bottom - (pP)->y)

// �������� ������� (�������� ������)
#define Dfi_RectWithOffsetA(pR, pP) \
        Dfi_RectMake((pR)->left + (pP)->x, (pR)->top + (pP)->y, (pR)->right + (pP)->x, (pR)->bottom + (pP)->y)

// ���������(���������) ������� 
#define Dfi_RectWithExtension(pR, pREx) \
        Dfi_RectMake((pR)->left + (pREx)->left, (pR)->top + (pREx)->top, (pR)->right + (pREx)->right, (pR)->bottom + (pREx)->bottom)

// ���������(���������) ������� 
#define Dfi_RectResize(pR, Value) \
        Dfi_RectMake((pR)->left - Value, (pR)->top - Value, (pR)->right + Value, (pR)->bottom + Value)

// ��������� ������� �� ������ ��������
MODULEAPI void Dfi_RectWithEven(PRECT pR);

#define Dfi_RectWidth(pR)  ((pR)->right - (pR)->left)
#define Dfi_RectHeight(pR) ((pR)->bottom - (pR)->top)
#define Dfi_RectSquare(pR) (Dfi_RectWidth(pR)*Dfi_RectHeight(pR))
#define Dfi_RectCenter(pR) Dfi_PointMake(((pR)->left + (pR)->right) >> 1, ((pR)->top + (pR)->bottom) >> 1)

// �������� ��������� ��������
#define Dfi_RectsIsEqual(pR1, pR2) (((pR1)->left  == (pR2)->left)  && ((pR1)->top    == (pR2)->top) && \
                                    ((pR1)->right == (pR2)->right) && ((pR1)->bottom == (pR2)->bottom))

// ���������� �������� �� ������ RECT'a (�� <= 0)
// ���������: 1 - ��������, 0 - ���
MODULEAPI int  Dfi_RectIsInvalid(PRECT pR);

// ���������� �� ������������ �� �������
// ���������:
//   0    - �� ������������
//   1,-1 - ��������� ��� ������ ����������
//   2,-2 - ����������� ������ ��� ������ ����������
//   3,-3 - 1 ������� ����� ������� ��������� � ������ �������
MODULEAPI int Dfi_RectsIsIntersection(PRECT b1, PRECT b2);

// ��������� RECT � ������, ���� ������� ����������
MODULEAPI int Dfi_RectReverse(PRECT pR);

// ��������� ������� �������, ������� � ������������� ��������� (���� < 0)
MODULEAPI int Dfi_RectCorrect(PRECT pR);

// �������� ���������� �������������� <a> � ������ ��������
// � ��� ������� � ��� �������
MODULEAPI void Dfi_RectComeBackToArea(RECT *a, int left, int top, int right, int bottom);
#define Dfi_RectComeBackToRect(a, b) \
        Dfi_RectComeBackToArea(a, (b)->left, (b)->top, (b)->right, (b)->bottom)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ������ � �������
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ������� �����
MODULEAPI POINT Dfi_PointMake(int x,int y);

// ��������� ����� �� ������ ��������
MODULEAPI void Dfi_PointWithEven(PPOINT p);

// �������� ��������� ��������
#define Dfi_PointsIsEqual(pP1, pP2) (((pP1)->x == (pP2)->x) && ((pP1)->y == (pP2)->y))

// ���������� ����� � ��������������: BOOL Dfi_PointIsInRect(PPOINT p, PRECT r)
#define Dfi_PointIsInRect(p, r) \
        ((p)->x >= (r)->left && (p)->x <= (r)->right && (p)->y >= (r)->top  && (p)->y <= (r)->bottom)

// ���������� ����� � ��������������: BOOL PointInRect(int p_x, int p_y, PRECT r)
#define Dfi_PointIsInRectEx(p_x, p_y, r) \
        ((p_x) >= (r)->left && (p_x) <= (r)->right && (p_y) >= (r)->top  && (p_y) <= (r)->bottom)

// �������� ���������� ����� � �������������� � �� ������� � ��� �������
MODULEAPI void Dfi_PointComeBackToArea(PPOINT pP, PRECT pR);

// ����� ���� ����� ����� ������� (Laob)
MODULEAPI float Dfi_PointsCalcAngle(POINT a, POINT o, POINT b);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ������ � �������
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// ������� ������������ ����� ����������� ���� ������
// �� �������� ������������� ���������
MODULEAPI POINT Dfi_LinesCrossByEquation(float A1, float B1, int C1, 
                                         float A2, float B2, int C2, int mode);

// ������� ������������ ������������ ��������� �� 2-� ������
MODULEAPI void Dfi_LinesEquationByCross(float *pA, float *pB, int *pC, POINT P1, POINT P2);

// ������� ������������ ����� ����������� ���� ������
// �� �������� ����� ����� (p1, p2) � (p3, p4)
MODULEAPI POINT Dfi_LinesGetPointOfCross(POINT p1, POINT p2, POINT p3, POINT p4);

// ���������� �� ����� �� ������
MODULEAPI int Dfi_DistPointToLine(float x, float y, float a, float b, int c);

#ifdef __cplusplus
  }	// extern "C"
#endif

#endif