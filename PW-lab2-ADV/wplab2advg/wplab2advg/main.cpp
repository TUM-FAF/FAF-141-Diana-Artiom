#include <tchar.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "dialog.h"
#include <stdlib.h>

// Windows procedure, 
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
// Help procedure, 
BOOL CALLBACK HelpProc(HWND, UINT, WPARAM, LPARAM);
// Function for changing background color
void DrawPixels(HWND hwnd);

// Make the class name into a global variable  
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

//main function
int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;					// Window handle
    MSG messages;				// Messages struct
    WNDCLASSEX wc;				// windowclass datastructure

    // Window structure 
    wc.hInstance = hThisInstance;
    wc.lpszClassName = szClassName;
    wc.lpfnWndProc = WindowProcedure;      // function is called by windows 
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.cbSize = sizeof (WNDCLASSEX);

    // Icon and mouse pointer
    wc.hIcon = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON));
    wc.hIconSm = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON));
    wc.hCursor = LoadCursor (hThisInstance, MAKEINTRESOURCE(ID_CURSOR));
    wc.lpszMenuName = MAKEINTRESOURCE(IDM_MENU);
    wc.cbClsExtra = 0;                      
    wc.cbWndExtra = 0;                      
    // Use Windows's colour as the background of the window - default 
    wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    // Register the window class, if it fails - quit the program 
    if (!RegisterClassEx (&wc))
        return 0;

    
    hwnd = CreateWindowEx (
           0,                   
           szClassName,         // Classname
           _T("Windows Programming #2"),       // Title 
           WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, // default window 
           410,       
           180,       
           520,                 // width 
           330,                 // and height in pixels 
           HWND_DESKTOP,        // The window is a child to desktop 
           NULL,                // No menu declared this way
           hThisInstance,       // Program Instance handler
           NULL                 // No aditional data 
           );

    // Make the window visible on the screen
    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);


    while (GetMessage (&messages, NULL, 0, 0))
    {
        // Translate messages
        TranslateMessage(&messages);
        // Send message to the WindowProcedure 
        DispatchMessage(&messages);
    }

    // The program return-value is 0 - The value that PostQuitMessage() gave 
    return messages.wParam;
}


//  This function is called by the Windows function DispatchMessage()  

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HINSTANCE hInstance;
    static int a, b, c, textColor = 0;
    static int cxCoord, cyCoord;
    static bool backgFlag;
    static HWND listBox, buttonAdd, submitBox, scrollBarColor,
                scrollBarWidth, scrollBarHeight;
    LRESULT textSize;
    char textStore[20];
    static int xPos, xMin, xMax;



    switch (message)                  // handle the messages
    {
        case WM_CREATE:
        {
            listBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("LISTBOX"), TEXT(""),
                             WS_VISIBLE | WS_CHILD | LBS_NOTIFY,
                             20, 45, 150, 200,
                             hwnd, (HMENU) IDC_LISTBOX, GetModuleHandle(NULL), NULL );
            buttonAdd = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Add"),
                             WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                             180, 10, 60, 25,
                             hwnd, (HMENU) IDC_ADD, GetModuleHandle(NULL), NULL);
            submitBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(""),
                             WS_VISIBLE | WS_CHILD,
                             20, 10, 150, 25,
                             hwnd, (HMENU)IDC_SUBMIT, GetModuleHandle(NULL), NULL);
            scrollBarColor = CreateWindowEx(NULL, TEXT("SCROLLBAR"), TEXT(""),
                             WS_VISIBLE | WS_CHILD | SBS_HORZ,
                             250, 10, 220, 25,
                             hwnd, (HMENU)IDC_SCROLL_COLOR, GetModuleHandle(NULL), NULL);
            SendMessage(submitBox, EM_LIMITTEXT, WPARAM(10), NULL);
            xPos = 0;
            xMin = 0;
            xMax = 255;
            SetScrollRange(scrollBarColor, SB_CTL, xMin, xMax, FALSE);
            SetScrollPos(scrollBarColor, SB_CTL, xPos, TRUE);
            RegisterHotKey(hwnd, HK_EXIT, MOD_CONTROL, 0x57);
            RegisterHotKey(hwnd, HK_HELP, MOD_CONTROL, 0x49);
            hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
            break;
        }

        case WM_CONTEXTMENU:
        {
            if ((HWND)wParam == listBox)
            {
                if(MessageBox(hwnd, L"Do you want to remove the selected item?", L"Alert", MB_YESNO) == IDYES)
                {
                    int index = SendMessage(GetDlgItem(hwnd, IDC_LISTBOX), LB_GETCURSEL, 0, 0);
                    SendMessage(GetDlgItem(hwnd, IDC_LISTBOX), LB_DELETESTRING, index, 0);
                }
                break;

            }
            break;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case ID_EXIT:
                {
                    exit(1);
                }

                case ID_BCKG:
                {
                    DrawPixels(hwnd);
                    break;
                }

                case ID_HELP:
                {
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), hwnd, HelpProc);
                    break;
                }

                case IDC_ADD:
                {
                    textSize = SendMessage(submitBox, WM_GETTEXT, 100, (LPARAM)textStore);
                    textStore[textSize] = _T('\0');
                    SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)textStore);
                    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    SendMessage(submitBox, WM_SETTEXT, NULL, NULL);
                    break;
                }
                
            }
            break;
        }

        case WM_SIZE:
        {
            cxCoord = LOWORD(lParam);
            cyCoord = HIWORD(lParam);
            MoveWindow(submitBox, cxCoord/18, cyCoord/20, (cxCoord - cxCoord/12 - 3*cxCoord/18), cyCoord/15, TRUE);
            MoveWindow(buttonAdd, (cxCoord - cxCoord/12 - cxCoord/18), cyCoord/20, (cxCoord/12), cyCoord/15, TRUE);
            MoveWindow(scrollBarColor, cxCoord/18, (cyCoord - cyCoord/20 - cyCoord/15), (cxCoord - 2*cxCoord/18), cyCoord/15, TRUE);
            MoveWindow(listBox, cxCoord/18, (2*cyCoord/20 + cyCoord/15), (cxCoord - 2*cxCoord/18), (cyCoord - 3*cyCoord/20 - 2*cyCoord/15), TRUE);
            break;
        }

        case WM_GETMINMAXINFO:
            {
                LPMINMAXINFO winSize = (LPMINMAXINFO)lParam;
                winSize->ptMinTrackSize.x = 490;
                winSize->ptMinTrackSize.y = 365;
                winSize->ptMaxTrackSize.x = 630;
                winSize->ptMaxTrackSize.y = 425;
                break;
            }


        case WM_HSCROLL:
        {
            if((HWND)lParam == scrollBarColor)
            {
                switch(LOWORD(wParam))
                {
                    case SB_LINELEFT:
                    {
                        xPos -= 1;
						DrawPixels(hwnd);
                        break;
                    }

                    case SB_LINERIGHT:
                    {
                        xPos += 1;
						DrawPixels(hwnd);
                        break;
                    }

                    case SB_PAGELEFT:
                    {
                        xPos -= 10;
						DrawPixels(hwnd);
                        break;
                    }

                    case SB_PAGERIGHT:
                    {
                        xPos += 10;
						DrawPixels(hwnd);
                        break;
                    }

                    case SB_THUMBPOSITION:
                    {
						DrawPixels(hwnd);
                        break;
                    }

                    case SB_TOP:
                    {
                        xPos = xMin;
						DrawPixels(hwnd);
                        break;
                    }

                    case SB_BOTTOM:
                    {
                        xPos = xMax;
						DrawPixels(hwnd);
                        break;
                    }

                    case SB_THUMBTRACK:
                    {
                        xPos = HIWORD(wParam);
						DrawPixels(hwnd);
                        break;
                    }

                    default:
                        break;


                }
                SetScrollPos(scrollBarColor, SB_CTL, xPos, TRUE);
                if(xPos == xMax)
                {
					DrawPixels(hwnd);
                    EnableScrollBar(scrollBarColor, SB_CTL, ESB_DISABLE_RIGHT);
                }
                if(xPos == xMin)
                {
					DrawPixels(hwnd);
                    EnableScrollBar(scrollBarColor, SB_CTL, ESB_DISABLE_LEFT);
                }
                InvalidateRect(submitBox, NULL, TRUE);
            }
            break;
        }

        case WM_CTLCOLOREDIT:
        {
            SetTextColor((HDC)wParam, RGB(xPos+1, 123, 255-xPos));
            SetBkMode((HDC)wParam, RGB(255, 0, 100));
            HBRUSH locBrush=(HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
            return(LRESULT) locBrush;
        }

        case WM_SETCURSOR:
        {
            if (LOWORD(lParam) == HTCLIENT)
            {
                SetCursor(LoadCursor(hInstance, MAKEINTRESOURCE(ID_CURSOR)));
                return TRUE;
            }
            break;
        }

        case WM_HOTKEY:
        {
            switch(wParam)
            {
                case HK_EXIT:
                {
                    PostQuitMessage(0);
                    break;
                }

                case HK_HELP:
                {
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), hwnd, HelpProc);
                    break;
                }

                default:
                    break;
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage (0);       
            break;
        default:                      
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

BOOL CALLBACK HelpProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                case IDCANCEL:
                    EndDialog(hDlg, 0);
                    return TRUE;
            }
    }
    return FALSE;
}

void DrawPixels(HWND hwnd) {

	PAINTSTRUCT ps;
	RECT r;
	static int a, b, c, textColor = 0;
	static bool backgFlag;

	GetClientRect(hwnd, &r);

	if (r.bottom == 0)
		return;

	HDC hdc = BeginPaint(hwnd, &ps);
	a = rand() % 255 + 1;
	b = rand() % 255 + 1;
	c = rand() % 255 + 1;
	backgFlag = true;
	if(backgFlag == true)
	{
		SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(a, b, c)));
	}
	InvalidateRect(hwnd, NULL, TRUE);
	
	EndPaint(hwnd, &ps);
}
