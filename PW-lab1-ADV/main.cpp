#define UNICODE

#include <tchar.h>
#include <windows.h>
#include <stdlib.h>
#include <mmsystem.h>
#define IDC_TEXT1 101
#define IDC_BUTTON1 102
#define IDC_BUTTON2 103
#define IDC_TEXT2 104
#define IDC_BUTTON3 105
#define IDC_BUTTON4 106


// Windows Procedure Declaration
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

// Class name
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    HWND hwnd;               // window handling for our  
    MSG messages;            // application messages
    WNDCLASSEX windowclass;        // Data structure for the windowclass 

    // The window structure
    windowclass.hInstance = hThisInstance;
    windowclass.lpszClassName = szClassName;
    windowclass.lpfnWndProc = WindowProcedure;      
    windowclass.style = CS_VREDRAW | CS_HREDRAW;                
    windowclass.cbSize = sizeof (WNDCLASSEX);

    // Use default icon and mouse-pointer 
    windowclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    windowclass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    windowclass.hCursor = LoadCursor (NULL, IDC_ARROW);
    windowclass.lpszMenuName = NULL;                 
    windowclass.cbClsExtra = 0;                     
    windowclass.cbWndExtra = 0;                      
    // Window`s bacground
    windowclass.hbrBackground = CreateSolidBrush(RGB(153,153,103)); // SETTING BACKGROUND COLOR

    // Window Registration
    if (!RegisterClassEx (&windowclass))
        return 0;

    // Windpw Creation
    hwnd = CreateWindowEx (
           0,                   
           szClassName,         
           _T("Windows Programming #1"),   
           WS_OVERLAPPEDWINDOW, 
           400,  
           200,       
           550,               
           400,              
           HWND_DESKTOP,     
           NULL,           
           hThisInstance, 
		   NULL
           );

   // Window showing
    ShowWindow (hwnd, nCmdShow);

   
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);        
        DispatchMessage(&messages);
    }
    return messages.wParam;
}


// function called by the Windows

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = NULL;
    PAINTSTRUCT ps;
    RECT rect;
    HFONT fontHelvetica, buttonFont;
    static HWND textArea1, buttonSubmit, buttonClear,
           textArea2, buttonRed, luckyButton;
    HBRUSH textArea1Brush;
    static int cxCoord, cyCoord;
    static HGDIOBJ defaultFont;
    LRESULT textSize;
    char textStore[1000];
    static bool textFlagRed;
    static bool backgFlag;
    static int a, b, c;



    switch (message)                 
    {
        case WM_CREATE:
        {
			
            textArea1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(" "),
                                       WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
                                       0, 0, 0, 0,
                                       hwnd, (HMENU)IDC_TEXT1, GetModuleHandle(NULL), NULL);
            buttonSubmit = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Submit"),
                                       WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                       0, 0, 0, 0,
                                       hwnd, (HMENU)IDC_BUTTON1, GetModuleHandle(NULL), NULL);
            buttonClear =  CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Clear"),
                                       WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                       0, 0, 0, 0,
                                       hwnd, (HMENU)IDC_BUTTON2, GetModuleHandle(NULL), NULL);
            textArea2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(" "),
                                       WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
                                       0, 0, 0, 0,
                                       hwnd, (HMENU)IDC_TEXT1, GetModuleHandle(NULL), NULL);
            buttonRed = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Red"),
                                       WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                       0, 0, 0, 0,
                                       hwnd, (HMENU)IDC_BUTTON3, GetModuleHandle(NULL), NULL);
			luckyButton = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Lucky ME"),
                                       WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                       0, 0, 0, 0,
                                       hwnd, (HMENU)IDC_BUTTON4, GetModuleHandle(NULL), NULL);

            HGDIOBJ defaultFont = GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(textArea1, WM_SETTEXT, NULL, (LPARAM)"Insert text here... ");
            SendMessage(buttonSubmit, WM_SETFONT, (WPARAM)defaultFont, MAKELPARAM(FALSE,0));
            SendMessage(buttonClear, WM_SETFONT, (WPARAM)defaultFont, MAKELPARAM(FALSE,0));
            break;
        }

        case WM_PAINT:
        {
            hdc = BeginPaint(hwnd, &ps);
            fontHelvetica = CreateFont(20, 0, 0, 0, FW_BOLD, false, false, false,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    DEFAULT_QUALITY, FF_DONTCARE, L"Helvetica");
            SetBkMode(hdc, TRANSPARENT);
            GetClientRect(hwnd, &rect);
            DrawText(hdc, L"\nEnter Up something here: ", -1, &rect, DT_TOP | DT_CENTER);
            buttonFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, false, false, false,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    DEFAULT_QUALITY, FF_DONTCARE, L"Consolas");
            SendMessage(buttonRed, WM_SETFONT, WPARAM(buttonFont), TRUE);
			SendMessage(luckyButton, WM_SETFONT, WPARAM(buttonFont), TRUE);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_GETMINMAXINFO:
        {
            LPMINMAXINFO winSize = (LPMINMAXINFO)lParam;
            winSize->ptMinTrackSize.x = 510;
            winSize->ptMinTrackSize.y = 375;
            winSize->ptMaxTrackSize.x = 630;
            winSize->ptMaxTrackSize.y = 425;
            break;
        }

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_BUTTON1:
                {
                    textSize = SendMessage(textArea1, WM_GETTEXT, 100, (LPARAM)textStore); // text size
                    textStore[textSize] = _T('\0'); // initialization with null character
                    SendMessage(textArea2, EM_REPLACESEL, 0, (LPARAM)textStore); // add inputed text to window
                    SendMessage(textArea2, EM_REPLACESEL, 0, (LPARAM)" ");
                    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    fontHelvetica = CreateFont(20, 0, 0, 0, FW_DONTCARE, false, false, false,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    DEFAULT_QUALITY, FF_DONTCARE, L"Helvetica");
                    SendMessage(textArea2, WM_SETFONT, (WPARAM)fontHelvetica, TRUE);
                    break;
                }

                case IDC_BUTTON2:
                {
                    SendMessage(textArea2, WM_SETTEXT, NULL, NULL);
                    break;
                }

                case IDC_BUTTON3:
                {
                    textFlagRed = true;
                    InvalidateRect(textArea2, NULL, TRUE);
                    break;
                }

                case IDC_BUTTON4:
                {
					PlaySound (TEXT ("imany.wav"), NULL, SND_FILENAME | SND_ASYNC) ;
					return 0 ;
                }
                }
        break;

        case WM_SYSCOMMAND:
        {
            switch(wParam)
            {
                case SC_CLOSE:
                 {
                    if(MessageBox(hwnd, L"Do you want to close the program?", L"Alert", MB_YESNO) == IDYES)
                        {
                            exit(1);
                        }
                        break;
                 }

                 default:
                     return DefWindowProc(hwnd, message, wParam, lParam);
                }
            break;

        case WM_SIZE:
        {
            cxCoord = LOWORD(lParam); 
            cyCoord = HIWORD(lParam);
            MoveWindow(textArea1, 15, 45, cxCoord/2-40, cyCoord/2, TRUE);
            MoveWindow(buttonSubmit, 15, (cyCoord-230) + cyCoord/2-55, cxCoord/2-175, cyCoord/2-144, TRUE);
            MoveWindow(buttonClear, 411, (cyCoord-230) + cyCoord/2-55, cxCoord/2-175, cyCoord/2-144, TRUE);
            MoveWindow(textArea2, 280, 45, cxCoord/2-40, cyCoord/2, TRUE);
            MoveWindow(buttonRed, 270, (cyCoord-230) + cyCoord/2-55, cxCoord/2-175, cyCoord/2-144, TRUE);
			MoveWindow(luckyButton, 15, (cyCoord-180) + cyCoord/2-55, cxCoord-35, cyCoord/2-144, TRUE);
            break;
        }

        case WM_CTLCOLOREDIT:
        {
            if(IDC_TEXT1 == GetDlgCtrlID((HWND)lParam))
            {
                textArea1Brush = CreateSolidBrush(RGB(204, 204, 154));
                SetBkMode((HDC)wParam, TRANSPARENT);
                return(INT_PTR)textArea1Brush;
            }
            break;
        }

        case WM_CTLCOLORSTATIC:
        {
            if(textFlagRed == true && (HWND)lParam == textArea2)
            {
                HBRUSH hbr = (HBRUSH) DefWindowProc(hwnd, message, wParam, lParam);
                SetTextColor((HDC) wParam, RGB(255, 0, 0));
                return (BOOL) hbr;
            }
            break;
        }

        case WM_DESTROY:
        {
            PostQuitMessage (0);       // puts 0 in the message queue
            break;
        }

        default:                      
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
} }