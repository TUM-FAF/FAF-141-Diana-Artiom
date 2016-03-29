#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include <string.h>
#include <iostream>
#include <stdio.h>

#define IDB_FILL 121


void setUnchecked(HWND *toolList, HWND tool);
void FillRectangle(HWND tool, HWND hWnd, HBRUSH hBrush, HDC Hdc, int xstart, int ystart,
	int xend, int yend);
void FillEllipse(HWND tool, HWND hWnd, HBRUSH hBrush, HDC Hdc, int xstart, int ystart,
	int xend, int yend);
HBRUSH getBrush(HWND hWnd, COLORREF color);
void UnfillRectangle(HWND tool, HWND hWnd, HBRUSH hBrush, HDC Hdc, int xstart, int ystart,
	int xend, int yend);

void UnfillEllipse(HWND tool, HWND hWnd, HBRUSH hBrush, HDC Hdc, int xstart, int ystart,
	int xend, int yend);
void resize(LPMINMAXINFO winSize);
void release(HDC hdc, int xStart, int yStart, int yEnd, int xEnd, HWND hwnd);