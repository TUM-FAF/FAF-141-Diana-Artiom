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
#include "resource.h"
#include <string.h>
#include <iostream>
#include <stdio.h>
#include "functions.h"

/*  Declaration of Window procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Declare the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");
HBITMAP hBitmap = NULL;
int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nCmdShow)
{
	HWND hwnd;																	/* Handle of the window */
	MSG messages;																/* Declaration of messages struct variable */
	WNDCLASSEX windowclass;														/* Data structure for the windowclass */

																				/* The Window structure */
	windowclass.hInstance = hThisInstance;
	windowclass.lpszClassName = szClassName;									/* Assignment of class name*/
	windowclass.lpfnWndProc = WindowProcedure;									/* This function is called by windows */
	windowclass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;					/* Catching the double-clicks */
	windowclass.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	windowclass.hIcon = LoadIcon(hThisInstance, MAKEINTRESOURCE(IDI_ICON));		/*Using the IDI_ICON for the app*/
	windowclass.hIconSm = LoadIcon(hThisInstance, MAKEINTRESOURCE(IDI_ICON));	
	windowclass.hCursor = LoadCursor(NULL, IDC_ARROW);							/* Load the arrow(default) cursor*/
	windowclass.lpszMenuName = MAKEINTRESOURCE(IDM_MENU);						/* No menu defined */
	windowclass.cbClsExtra = 0;													/* No extra bytes after the window class */
	windowclass.cbWndExtra = 0;												    /* structure or the window instance */
	windowclass.hbrBackground = CreateSolidBrush(RGB(242, 242, 242));			/* Setting the background color */

	/* Window registration. If it fails return 0 */
	if (!RegisterClassEx(&windowclass))
		return 0;

	/* Window creation*/
	hwnd = CreateWindowEx(
		0,                   
		szClassName,															/* Class name */
		_T("Windows Programming #3"),											/* Title */
		WS_OVERLAPPEDWINDOW,													/* Default window */
		420,																	/* Window x position */
		300,																	/* Window y position */
		650,																	/* Window width */
		423,																	/* Window height */
		HWND_DESKTOP,															/* The actual window - child-window to desktop */
		NULL,																	/* No menu defined */
		hThisInstance,															/* Program Instance handler */
		NULL																	/* No Window Creation data */
		);

	/* Make the window visible */
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	/* Message loop - runs until a 0 is in the queue */
	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);											/* Translates messages */
		DispatchMessage(&messages);												/* Sends message to WindowProcedure */
	}

	/* The program return-value = 0 */
	return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;																	/* Handle to device context*/
	PAINTSTRUCT ps;																/* Variable of paint struct type */
	RECT rect = { 140, 0, 600, 300 };											/* Definition of painting rectangle*/
	HPEN hpen;
	/* Declaration of an HPEN object, used as a "pen" element */
	static HWND lineTool, penTool, rectTool, ellipseTool,						/* Declaration of HWND objects used in the window */	
		clearButton, bezierTool, eraserTool, weightArea, okButton,
		colorButton;
	HWND toolList[] = { penTool, lineTool, rectTool, ellipseTool,						/* Declaration of HWND objects used in the window */
		clearButton, bezierTool, eraserTool };
	static BOOL isDrawing = FALSE;												/* "Flag" variable used to determined whether the mouse is drawind or not */
	static int xStart, yStart, xEnd, yEnd;										/* Start and ending positions of the drawn lines*/
	static int mouse_x, mouse_y;												/* Variables used to determine whether the mouse is in the painting area */
	int cxCoord = 0, cyCoord = 0;												/**/
	char buffer[12] = "Set weight";
	char buffer2[12] = "Set color";
	char buffer3[12] = "Fill object";
	char textStore[50];
	POINT bez[4] = { { 60, 80 },{ 88, 112 },{ 210, 67 },{ 160, 20 } };			/* The bezier coordinates */
	static bool bezier = false;					
	static int weight = 4;														/* Weight of the drawn elements */
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE);
	HBITMAP bitmapHBmp;
	BITMAP bitmapBmp;
	bitmapHBmp = (HBITMAP)LoadImage(hInstance, L"bitmap.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(bitmapHBmp, sizeof(bitmapBmp), &bitmapBmp);
	mouse_x = LOWORD(lParam);
	mouse_y = HIWORD(lParam);
	COLORREF colour = RGB(0, 0, 0);												/* Initialization of the color variable */
	HBRUSH hbrush;																/* HBRUSH struc variable, used in case WM_PAINT */
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
		if (mouse_x > 140 && mouse_x < 600 && mouse_y > 0 && mouse_y < 300)					/* Check whether the mouse is in the painting area */
		{
			if (Button_GetCheck(GetDlgItem(hwnd, IDB_GREEN)) == BST_CHECKED)				/* Check whether the button green is checked */
			{
				colour = RGB(0, 255, 0);													/* Set color to green */
			}
			else if (Button_GetCheck(GetDlgItem(hwnd, IDB_BLACK)) == BST_CHECKED)			/* Check whether the button black is checked */
			{
				colour = RGB(0, 0, 0);														/* Set color to black */
			}
			hpen = CreatePen(PS_SOLID, weight, colour);
			hdc = GetDC(hwnd);
			SelectObject(hdc, hpen);
			xStart = LOWORD(lParam);
			yStart = HIWORD(lParam);
			xEnd = LOWORD(lParam);
			yEnd = HIWORD(lParam);
			if (Button_GetCheck(lineTool) == BST_CHECKED) {									/* Check whether the button lineTool is checked */
				SetROP2(hdc, R2_XORPEN);
				MoveToEx(hdc, xStart, yStart, NULL);
				LineTo(hdc, xEnd, yEnd);
				isDrawing = TRUE;
			}
			if (Button_GetCheck(penTool) == BST_CHECKED) {									/* Check whether the button penTool is checked */
				xStart = LOWORD(lParam);
				yStart = HIWORD(lParam);
				isDrawing = TRUE;
			}
			if (Button_GetCheck(rectTool) == BST_CHECKED) {									/* Check whether the button rectTool is checked */
				hbrush = getBrush(hwnd, colour);
				FillRectangle(rectTool, hwnd, hbrush, hdc, xStart, yStart, xEnd, yEnd);
				isDrawing = TRUE;
				DeleteObject(hbrush);
			}			
			if (Button_GetCheck(ellipseTool) == BST_CHECKED) {								/* Check whether the button elipseTool is checked */
				hbrush = getBrush(hwnd, colour);
				FillEllipse(rectTool, hwnd, hbrush, hdc, xStart, yStart, xEnd, yEnd);
				isDrawing = TRUE;
				DeleteObject(hbrush);
			}
			if (Button_GetCheck(bezierTool) == BST_CHECKED)									/* Check whether the button bezierTool is checked */
			{
			}
			DeleteObject(hpen);
			ReleaseDC(hwnd, hdc);
		}
		break;
	}

	case WM_MOUSEMOVE:
	{
		if (mouse_x > 140 && mouse_x < 600 && mouse_y > 0 && mouse_y < 300)	{				/* Check whether the mouse is in the painting area */
			hdc = GetDC(hwnd);
			if (isDrawing == TRUE) {
				if (Button_GetCheck(GetDlgItem(hwnd, IDB_GREEN)) == BST_CHECKED) {			/* Check whether the button green is checked */
					colour = RGB(0, 255, 0);
				}
				else if (Button_GetCheck(GetDlgItem(hwnd, IDB_BLACK)) == BST_CHECKED) {		/* Check whether the button black is checked */
					colour = RGB(0, 0, 0);
				}
				hpen = CreatePen(PS_SOLID, weight, colour);
				SelectObject(hdc, hpen);
				SetROP2(hdc, R2_NOTXORPEN);
				if (Button_GetCheck(lineTool) == BST_CHECKED) {								/* Check whether the button lineTool is checked */
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
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
		HDC hdc_mem = CreateCompatibleDC(hdc);
		BITMAP 			bitmap;
		HGDIOBJ 		oldBitmap;

		oldBitmap = SelectObject(hdc, hBitmap);
		GetObject(hBitmap, sizeof(bitmap), &bitmap);
		BitBlt(hdc, 0, 0, bitmapBmp.bmWidth, bitmapBmp.bmHeight, hdc_mem, 0, 410, SRCCOPY);

		SelectObject(hdc_mem, bitmapHBmp);
		DeleteObject(hdc_mem);
		SetBkMode(hdc, TRANSPARENT);
		//TextOut(hdc, 460, 202, buffer, strlen(buffer));
		//TextOut(hdc, 460, 235, buffer2, strlen(buffer2));
		//TextOut(hdc, 460, 320, buffer3, strlen(buffer3));
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
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		break;
	default:                      /* for messages that we don't deal with */
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return TRUE;
}

