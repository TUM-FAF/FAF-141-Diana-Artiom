#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#define IDI_ICON 101
#define IDB_LINE 102
#define IDM_MENU 103
#define ID_EXIT 104
#define IDB_PEN 105
#define IDB_POLYLINE 106
#define IDB_RECT 107
#define IDB_ELLIPSE 108
#define IDB_CLEAR 109
#define HK_CLEAR 110
#define ID_CLEAR 111
#define IDB_BEZIER 112
#define IDB_ERASER 113
#define IDC_WEIGHT 114
#define IDB_OK 115
#define IDB_RED 116
#define IDB_GREEN 117
#define IDB_BLUE 118
#define IDB_BLACK 119
#define IDB_RADIO_B 120

#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include "functions.h"

/*  Declaration of Window procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInst;

/*  Declare the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");
HBITMAP hBitmap = NULL;
int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nCmdShow)
{
	HWND hwnd;																	
	MSG messages;																
	WNDCLASSEX windowclass;														
	hInst = hThisInstance;

																				
	windowclass.hInstance = hThisInstance;
	windowclass.lpszClassName = szClassName;									
	windowclass.lpfnWndProc = WindowProcedure;									
	windowclass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;					
	windowclass.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	windowclass.hIcon = LoadIcon(hThisInstance, MAKEINTRESOURCE(IDI_ICON));		
	windowclass.hIconSm = LoadIcon(hThisInstance, MAKEINTRESOURCE(IDI_ICON));	
	windowclass.hCursor = LoadCursor(NULL, IDC_ARROW);							
	windowclass.lpszMenuName = MAKEINTRESOURCE(IDM_MENU);						
	windowclass.cbClsExtra = 0;													
	windowclass.cbWndExtra = 0;												   
	windowclass.hbrBackground = CreateSolidBrush(RGB(242, 242, 242));			

	/* Window registration. If it fails return 0 */
	if (!RegisterClassEx(&windowclass))
		return 0;

	/* Window creation*/
	hwnd = CreateWindowEx(
		0,                   
		szClassName,															
		_T("Windows Programming #3"),											
		WS_OVERLAPPEDWINDOW,													
		440,																	
		140,																	
		620,																	
		413,																	
		HWND_DESKTOP,															
		NULL,																	
		hThisInstance,															
		NULL																	
		);

	/* Make the window visible */
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	/* Message loop - runs until a 0 is in the queue */
	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);											
		DispatchMessage(&messages);												
	}

	/* The program return-value = 0 */
	return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;																	
	BITMAP bitmap;
	HDC hdcMem;

	HBITMAP hbmppenImage = NULL;
    hbmppenImage = (HBITMAP)LoadImage(hInst, L"pen.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmppenImage, sizeof(bitmap), &bitmap);

	PAINTSTRUCT ps;																
	RECT rect2 = {140, 320, 20, 20 };
	RECT rect = { 140, 0, 600, 300 };											
	HPEN hpen;
	/* Declaration of an HPEN object, used as a "pen" element */
	static HWND lineTool, penTool, rectTool, ellipseTool,						
		clearButton, bezierTool, eraserTool, weightArea, okButton,
		colorButton;
	HWND toolList[] = { penTool, lineTool, rectTool, ellipseTool,						
		clearButton, bezierTool, eraserTool };
	static BOOL isDrawing = FALSE;												
	static int xStart, yStart, xEnd, yEnd;										
	static int mouse_x, mouse_y;												
	int cxCoord = 0, cyCoord = 0;												
	char buffer[12] = "Set weight";
	char buffer2[12] = "Set color";
	char buffer3[12] = "Fill object";
	char textStore[50];
	POINT bez[4] = { { 290, 100 },{ 299, 102 },{ 310, 67 },{ 200, 40 } };			/* The bezier coordinates */
	static bool bezier = false;					
	static int weight = 4;														
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE);
	HBITMAP bitmapHBmp;
	BITMAP bitmapBmp;
	bitmapHBmp = (HBITMAP)LoadImage(hInstance, L"bitmap.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(bitmapHBmp, sizeof(bitmapBmp), &bitmapBmp);
	mouse_x = LOWORD(lParam);
	mouse_y = HIWORD(lParam);
	COLORREF colour = RGB(0, 0, 0);												
	HBRUSH hbrush;																
	switch (message)                  /* handle the messages */
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case IDB_PEN: {
				setUnchecked(toolList, penTool);
				break;
			}
			case IDB_LINE: {
				setUnchecked(toolList, lineTool);
				break;
			}
			case IDB_ELLIPSE: {
				setUnchecked(toolList, ellipseTool);
				break;
			}
			case IDB_RECT: {
				setUnchecked(toolList, rectTool);
				break;
			}
			case IDB_BEZIER: {
				setUnchecked(toolList, bezierTool);
				bezier = true;
				InvalidateRect(hwnd, NULL, TRUE);
				UpdateWindow(hwnd);
				break;
			}
			case IDB_CLEAR: {
				setUnchecked(toolList, clearButton);
				InvalidateRect(hwnd, &rect, FALSE);
				InvalidateRect(hwnd, &rect, TRUE);
				Button_SetCheck(clearButton, BST_UNCHECKED);
				break;
			}
			case IDB_ERASER: {
				setUnchecked(toolList, eraserTool);
				break;
			}
			case ID_CLEAR: {
				SetFocus(hwnd);
				InvalidateRect(hwnd, &rect, FALSE);
				InvalidateRect(hwnd, &rect, TRUE);
				break;
			}
			case IDB_OK: {
				//GetWindowText(weightArea, textStore, 50);
				weight = atoi(textStore);
				printf("%d", weight);
				break;
			}
			case ID_EXIT: {
				PostQuitMessage(0);
				break;
			}
		}
		break;
	}

	case WM_CREATE:
	{
		/* Initialiazation of hwnd objects used in the window */
		penTool = CreateWindowEx(NULL, L"BUTTON", L"Pen",
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
			0, 0, 140, 43,
			hwnd, (HMENU)IDB_PEN, GetModuleHandle(NULL), NULL);
		lineTool = CreateWindowEx(NULL, L"BUTTON", L"Line",
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
			0, 43, 140, 43,
			hwnd, (HMENU)IDB_LINE, GetModuleHandle(NULL), NULL);
		rectTool = CreateWindowEx(NULL, L"BUTTON", L"Rectangle",
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
			0, 86, 140, 43,
			hwnd, (HMENU)IDB_RECT, GetModuleHandle(NULL), NULL);
		ellipseTool = CreateWindowEx(NULL, L"BUTTON", L"Ellipse",
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
			0, 129, 140, 43,
			hwnd, (HMENU)IDB_ELLIPSE, GetModuleHandle(NULL), NULL);
		clearButton = CreateWindowEx(NULL, L"BUTTON", L"Clear",
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
			0, 258, 140, 43,
			hwnd, (HMENU)IDB_CLEAR, GetModuleHandle(NULL), NULL);
		bezierTool = CreateWindowEx(NULL, L"BUTTON", L"Bezier",
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
			 0, 172, 140, 43,
			hwnd, (HMENU)IDB_BEZIER, GetModuleHandle(NULL), NULL);
		eraserTool = CreateWindowEx(NULL, L"BUTTON", L"Eraser",
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
			0, 215, 140, 43,
			hwnd, (HMENU)IDB_ERASER, GetModuleHandle(NULL), NULL);
		weightArea = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
			WS_VISIBLE | WS_CHILD | ES_NUMBER,
			70, 330, 25, 20,
			hwnd, (HMENU)IDC_WEIGHT, GetModuleHandle(NULL), NULL);
		okButton = CreateWindowEx(NULL, L"BUTTON", L"OK",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			110, 330, 30, 20,
			hwnd, (HMENU)IDB_OK, GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, L"BUTTON", L"Green",
			WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
			220, 330, 60, 18, 
			hwnd, (HMENU)IDB_GREEN, GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, L"BUTTON", L"Black",
			WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BST_CHECKED,
			340, 330, 60, 18,
			hwnd, (HMENU)IDB_BLACK, GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, L"BUTTON", L"",
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			440, 330, 60, 18,
			hwnd, (HMENU)IDB_FILL, GetModuleHandle(NULL), NULL);
		Button_SetCheck(GetDlgItem(hwnd, IDB_BLACK), BST_CHECKED);
		RegisterHotKey(hwnd, HK_CLEAR, MOD_CONTROL, 0x4E);
		//bitmapHBmp = (HBITMAP)LoadImage(hInstance, L"bitmap.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBitmap = (HBITMAP)LoadImage(hInstance, L"pen.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		break;
	}

	case WM_LBUTTONDOWN:
	{
		if (mouse_x > 140 && mouse_x < 600 && mouse_y > 0 && mouse_y < 300)					
		{
			if (Button_GetCheck(GetDlgItem(hwnd, IDB_GREEN)) == BST_CHECKED)				
			{
				colour = RGB(0, 255, 0);													
			}
			else if (Button_GetCheck(GetDlgItem(hwnd, IDB_BLACK)) == BST_CHECKED)			
			{
				colour = RGB(0, 0, 0);														
			}
			hpen = CreatePen(PS_SOLID, weight, colour);
			hdc = GetDC(hwnd);
			SelectObject(hdc, hpen);
			xStart = LOWORD(lParam);
			yStart = HIWORD(lParam);
			xEnd = LOWORD(lParam);
			yEnd = HIWORD(lParam);
			if (Button_GetCheck(lineTool) == BST_CHECKED) {									
				SetROP2(hdc, R2_XORPEN);
				MoveToEx(hdc, xStart, yStart, NULL);
				LineTo(hdc, xEnd, yEnd);
				isDrawing = TRUE;
			}
			if (Button_GetCheck(penTool) == BST_CHECKED) {									
				xStart = LOWORD(lParam);
				yStart = HIWORD(lParam);
				isDrawing = TRUE;
			}
			if (Button_GetCheck(rectTool) == BST_CHECKED) {									
				hbrush = getBrush(hwnd, colour);
				FillRectangle(rectTool, hwnd, hbrush, hdc, xStart, yStart, xEnd, yEnd);
				isDrawing = TRUE;
				DeleteObject(hbrush);
			}			
			if (Button_GetCheck(ellipseTool) == BST_CHECKED) {								
				hbrush = getBrush(hwnd, colour);
				FillEllipse(rectTool, hwnd, hbrush, hdc, xStart, yStart, xEnd, yEnd);
				isDrawing = TRUE;
				DeleteObject(hbrush);
			}
			if (Button_GetCheck(bezierTool) == BST_CHECKED)									
			{
			}
			DeleteObject(hpen);
			ReleaseDC(hwnd, hdc);
		}
		break;
	}

	case WM_MOUSEMOVE:
	{
		if (mouse_x > 140 && mouse_x < 600 && mouse_y > 0 && mouse_y < 300)	{				
			hdc = GetDC(hwnd);
			if (isDrawing == TRUE) {
				if (Button_GetCheck(GetDlgItem(hwnd, IDB_GREEN)) == BST_CHECKED) {			
					colour = RGB(0, 255, 0);
				}
				else if (Button_GetCheck(GetDlgItem(hwnd, IDB_BLACK)) == BST_CHECKED) {		
					colour = RGB(0, 0, 0);
				}
				hpen = CreatePen(PS_SOLID, weight, colour);
				SelectObject(hdc, hpen);
				SetROP2(hdc, R2_NOTXORPEN);
				if (Button_GetCheck(lineTool) == BST_CHECKED) {								
					MoveToEx(hdc, xStart, yStart, NULL);
					LineTo(hdc, xEnd, yEnd);
					xEnd = LOWORD(lParam);
					yEnd = HIWORD(lParam);
					MoveToEx(hdc, xStart, yStart, NULL);
					LineTo(hdc, xEnd, yEnd);
				}
				if (Button_GetCheck(penTool) == BST_CHECKED) {
					MoveToEx(hdc, xEnd, yEnd, NULL);
					xEnd = LOWORD(lParam);
					yEnd = HIWORD(lParam);
					LineTo(hdc, xEnd, yEnd);
				}
				if (Button_GetCheck(rectTool) == BST_CHECKED) {
					hbrush = getBrush(hwnd, colour);
					UnfillRectangle(rectTool, hwnd, hbrush, hdc, xStart, yStart, xEnd, yEnd);
					xEnd = LOWORD(lParam);
					yEnd = HIWORD(lParam);
					Rectangle(hdc, xStart, yStart, xEnd, yEnd);
					DeleteObject(hbrush);
				}
				if (Button_GetCheck(ellipseTool) == BST_CHECKED) {
					hbrush = getBrush(hwnd, colour);
					UnfillEllipse(ellipseTool, hwnd, hbrush, hdc, xStart, yStart, xEnd, yEnd);
					xEnd = LOWORD(lParam);
					yEnd = HIWORD(lParam);
					Ellipse(hdc, xStart, yStart, xEnd, yEnd);
					DeleteObject(hbrush);
				}
				if (Button_GetCheck(bezierTool) == BST_CHECKED) {

				}
			}
			DeleteObject(hpen);
			ReleaseDC(hwnd, hdc);
		}
		break;
	}

	case WM_LBUTTONUP: 	{
		if (mouse_x > 140 && mouse_x < 600 && mouse_y > 0 && mouse_y < 300) {
			hdc = GetDC(hwnd);
			xEnd = LOWORD(lParam);
			yEnd = HIWORD(lParam);
			release(hdc, xStart, yStart, yEnd, xEnd, hwnd);
			if (Button_GetCheck(lineTool) == BST_CHECKED) {
				isDrawing = FALSE;
			}
			if (Button_GetCheck(penTool) == BST_CHECKED) {
				isDrawing = FALSE;
			}
			if (Button_GetCheck(rectTool) == BST_CHECKED) {
				isDrawing = FALSE;
			}
			if (Button_GetCheck(ellipseTool) == BST_CHECKED) {
				isDrawing = FALSE;
			}
			break;
		}
	}

	case WM_RBUTTONDOWN: {
		break;
	}

	case WM_HOTKEY: {
		switch (wParam) {
		case HK_CLEAR: {
			SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_CLEAR, 0), 0);
			break;
		}
		default: 
			break;
		}
		break;
	}
	/*case WM_ERASEBKGND: {
		OnEraseBkGnd(hwnd, rect2);
		break;
	}*/

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
		HDC hdc_mem = CreateCompatibleDC(hdc);
		BITMAP 			bitmap;
		HGDIOBJ 		oldBitmap;
		//OnEraseBkGnd(hwnd, rect2);

		oldBitmap = SelectObject(hdc, hBitmap);
		GetObject(hBitmap, sizeof(bitmap), &bitmap);
		BitBlt(hdc, 0, 0, bitmapBmp.bmWidth, bitmapBmp.bmHeight, hdc_mem, 0, 410, SRCCOPY);

		hdcMem = CreateCompatibleDC(hdc);

		SelectObject(hdcMem, hbmppenImage);
        BitBlt(hdc, 20, 390, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

		DeleteDC(hdcMem);

		SelectObject(hdc_mem, bitmapHBmp);
		DeleteObject(hdc_mem);
		SetBkMode(hdc, TRANSPARENT);
		if (bezier) {
			HPEN localhpen = CreatePen(PS_SOLID, weight, colour);
			SelectObject(hdc, localhpen);
			SetFocus(hwnd);
			PolyBezier(hdc, bez, 4);
			bezier = false;
			DeleteObject(localhpen);
		}
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_GETMINMAXINFO: {
		LPMINMAXINFO winSize = (LPMINMAXINFO)lParam;
		resize(winSize);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);       
		break;
	default:                      
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return TRUE;
}
