#include "functions.h"

void setUnchecked(HWND *toolList, HWND tool) {
	for (int i = 0; i < 7; i++) {
		if (toolList[i] != tool)
			Button_SetCheck(toolList[i], BST_UNCHECKED);
	}
}

void FillRectangle(HWND tool, HWND hWnd, HBRUSH hBrush, HDC Hdc, int xstart, int ystart,
	int xend, int yend) {
	SelectObject(Hdc, hBrush);
	SetROP2(Hdc, R2_XORPEN);
	Rectangle(Hdc, xstart, ystart, xend, yend);
}
void UnfillRectangle(HWND tool, HWND hWnd, HBRUSH hBrush, HDC Hdc, int xstart, int ystart,
	int xend, int yend) {
	SelectObject(Hdc, hBrush);
	SetROP2(Hdc, R2_NOTXORPEN);
	Rectangle(Hdc, xstart, ystart, xend, yend);
}


void FillEllipse(HWND tool, HWND hWnd, HBRUSH hBrush, HDC Hdc, int xstart, int ystart,
	int xend, int yend) {

	SelectObject(Hdc, hBrush);
	SetROP2(Hdc, R2_XORPEN);
	Ellipse(Hdc, xstart, ystart, xend, yend);
}

void UnfillEllipse(HWND tool, HWND hWnd, HBRUSH hBrush, HDC Hdc, int xstart, int ystart,
	int xend, int yend) {

	SelectObject(Hdc, hBrush);
	SetROP2(Hdc, R2_NOTXORPEN);
	Ellipse(Hdc, xstart, ystart, xend, yend);
}

HBRUSH getBrush(HWND hWnd, COLORREF color) {
	if (Button_GetCheck(GetDlgItem(hWnd, IDB_FILL)) == BST_CHECKED) {				/* Check whether the button fill is checked */
		return CreateSolidBrush(color);
	}
	else {
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
}


void resize(LPMINMAXINFO winSize) {
	winSize->ptMinTrackSize.x = 600;
	winSize->ptMinTrackSize.y = 420;
	winSize->ptMaxTrackSize.x = 600;
	winSize->ptMaxTrackSize.y = 420;
}

void release(HDC hdc, int xStart, int yStart, int yEnd, int xEnd, HWND hwnd) {
	SetROP2(hdc, R2_XORPEN);
	MoveToEx(hdc, xStart, yStart, NULL);
	LineTo(hdc, xEnd, yEnd);
	ReleaseDC(hwnd, hdc);
}