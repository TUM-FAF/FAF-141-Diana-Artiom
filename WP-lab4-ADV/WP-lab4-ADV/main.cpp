#include <iostream>
#include <windows.h>
#include <cmath>
#include <tchar.h>
#include <windowsx.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#pragma once
#include <string>
#include <cmath>
#include "Object.h"
#include <vector>


LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

static Animation *objectCollection[60]; //limitation of balls
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");


int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX windowClass;

    windowClass.hInstance = hThisInstance;
    windowClass.lpszClassName = szClassName;
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.style = CS_DBLCLKS;         // Catching double-clicks
    windowClass.cbSize = sizeof (WNDCLASSEX);
    windowClass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    windowClass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor (NULL, IDC_ARROW);
    windowClass.lpszMenuName = NULL;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hbrBackground = CreateSolidBrush( RGB(255, 255, 255) );


    if (!RegisterClassEx (&windowClass))
        return 0;

       hwnd = CreateWindowEx (
           0,
           szClassName,
		   L"Fancy fuzzy stuff",
           WS_OVERLAPPEDWINDOW,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           700,
           600,
           HWND_DESKTOP,
           NULL,
           hThisInstance,
           NULL
           );

    ShowWindow (hwnd, nCmdShow);

    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc;
    static HDC hdcMem;
    static HBRUSH hBrush;
    static PAINTSTRUCT ps;
    static RECT rect;
    static HANDLE hndOld;
    static int timerSpeed=10;
    static HBITMAP hndbmMem;
    static int nrOfObjects=0;
	
    switch (message)
    {
		case WM_CREATE:

            hdc=GetDC(hwnd);
            GetClientRect(hwnd,&rect);
            hdcMem=CreateCompatibleDC(hdc);
            hndbmMem=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
            hndOld = SelectObject(hdcMem,hndbmMem);
            SetTimer(hwnd,ID_TIMER,timerSpeed,NULL);
            break;

		case WM_PAINT:

            hdc=BeginPaint(hwnd,&ps);
            GetClientRect(hwnd,&rect);

            for(int i=0;i<nrOfObjects-1;i++) {
                for(int j=i+1;j<nrOfObjects;j++) {
                    Interaction(*objectCollection[i],*objectCollection[j]);
                }
            }

            FillRect(hdcMem,&rect,(HBRUSH)GetStockObject(WHITE_BRUSH));


            for(int i=0;i<nrOfObjects;i++) {
                objectCollection[i]->moveB(hdcMem,rect,hBrush);
            }

            BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
            EndPaint(hwnd,&ps);
            break;

        case WM_SIZE:

            SelectObject(hdcMem,hndOld);
            DeleteObject(hndbmMem);
            DeleteDC(hdcMem);
            hdc=GetDC(hwnd);
            GetClientRect(hwnd,&rect);
            hdcMem=CreateCompatibleDC(hdc);
            hndbmMem=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
            hndOld = SelectObject(hdcMem,hndbmMem);
            break;

        case WM_LBUTTONDOWN:

            POINT objectHere; // object to be generate at the current position at which mouse position is set down
            objectHere.x=LOWORD(lParam);
            objectHere.y=HIWORD(lParam);

            objectCollection[nrOfObjects]=new Sphere(objectHere, 4 + objectHere.x % 4); // create new object
            objectCollection[nrOfObjects]->BallColor(RGB(objectHere.x % 255, objectHere.x % 100 + objectHere.y % 100, objectHere.y % 100)); // assign a color to the previously created object
            nrOfObjects++;
            break;
			
        case WM_TIMER:

            InvalidateRect(hwnd,NULL,FALSE);
            break;


        case WM_MOUSEWHEEL:
            
			if((short)HIWORD(wParam)<0) {
                timerSpeed += 10;
            }
			else {
                timerSpeed -= 10;
                if (timerSpeed < 0) timerSpeed = 1;
            }

            KillTimer(hwnd,ID_TIMER);
            SetTimer(hwnd,ID_TIMER,timerSpeed,NULL);
            break;
			
        case WM_DESTROY:

            SelectObject(hdcMem,hndOld);
            DeleteObject(hndbmMem);
            DeleteDC(hdcMem);
            KillTimer(hwnd,ID_TIMER);

            PostQuitMessage (0);
            break;

        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

