#include <Windows.h>
#include <winbase.h> // Allows us to use functions like CloseHandle()
#include <winuser.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h> // Allows us to use lower case booleans (bool)
#include <SDKDDKVer.h>
#include <tchar.h>
#include <windows.h>
#include <string.h>
#include <tchar.h>
#include <stdlib.h>
#include "Main.h"
#include "Draw.h"
#include "Addresses.h"
// gcc -o Builds//AssaultCubeExternal.exe Main.c MathUtil.c Addresses.c Draw.c -lgdi32 -lmingw32

/** LINKS
* 1. Permissions: https://learn.microsoft.com/en-us/windows/win32/procthread/process-security-and-access-rights
* 2. OpenProcess() https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess?redirectedfrom=MSDN
* 3. Closing Handles: https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
* 4. Read Process Memory: https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-readprocessmemory?redirectedfrom=MSDN
* 5. Write Process Memory: https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-writeprocessmemory?redirectedfrom=MSDN
*
* 6. Explains Timers: https://learn.microsoft.com/en-us/windows/win32/winmsg/using-timers
* 7. SetTimer: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-settimer
*/

static HWND overlayHWND;
static HINSTANCE currentInst;  // current instance
static const WCHAR overlayTitle[100] = L"AssaultCube Overlay";  // The title bar text
static const LPCSTR targetTitle = "AssaultCube";

// I need to list all functions above the main function and by not putting them in Main.h esentially makes them private
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD);
int pressToClose(bool);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MyRegisterClass(hInstance);
    HWND targetHWND = FindWindow(0, targetTitle);

    DWORD pid;
    GetWindowThreadProcessId(targetHWND, &pid); // Gets AssaultCubes process ID
    printf("PID:%lu \n", pid);
    hProcess = OpenProcess(PROCESS_VM_READ, FALSE, pid); // PROCESS_ALL_ACCESS gives all access, PROCESS_VM_READ gives reading access

    if (!targetHWND) return FALSE;
    
    // This gets the window of our target application
    GetWindowRect(targetHWND, &m_Rect);
    width = m_Rect.right - m_Rect.left;
    height = m_Rect.bottom - m_Rect.top;
    printf("Width:%i Height:%i \n", width, height);

    if (!InitInstance (hInstance, nCmdShow)) return FALSE;
    
    initilize();

    // 16 represents the cool down and 1000ms / 16 equals a bit over 60 fps which is how often the boxes will update
    SetTimer(overlayHWND, WM_TIMER, 16, (TIMERPROC) TimerProc);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {;
        if (msg.message == WM_TIMER) {  // Send WM_TIMER messages to the hwndTimer procedure. 
            msg.hwnd = overlayHWND; 
        } 

        TranslateMessage(&msg);
        DispatchMessage(&msg);
       
        RECT rect;
        GetWindowRect(targetHWND, &rect);
        m_Rect = rect;
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
        MoveWindow(overlayHWND, rect.left, rect.top, width, height, true);
    }
    printf("Break Out \n");
    getchar();
    getchar();
    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW; // Will redraw the whole window if the width or height changes, 
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = 0;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(0, 0, 0)); // Sets the backgraound color to black
    wcex.lpszMenuName   = overlayTitle;
    wcex.lpszClassName  = overlayTitle;
    wcex.hIconSm        = 0;

    return RegisterClassExW(&wcex);
}

/**
* WS_EX_TOPMOST Placed our window above our target window
* WS_EX_TRANSPARENT | WS_EX_LAYERED Ensures our overlay window is transparent 
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    currentInst = hInstance; 
    overlayHWND = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, overlayTitle, overlayTitle, WS_POPUP, 1, 1, width, height, NULL, NULL, hInstance, NULL);

    if (!overlayHWND) return FALSE;
    
    SetLayeredWindowAttributes(overlayHWND, RGB(0, 0, 0), 0, LWA_COLORKEY);
    ShowWindow(overlayHWND, nCmdShow);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_PAINT: // https://learn.microsoft.com/en-us/windows/win32/gdi/wm-paint
        {
            PAINTSTRUCT ps;
            hdc = BeginPaint(hWnd, &ps);

            updateBoxes();
            esp();
          
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        {
            printf("Destroy WM \n");
            deallocate();
            PostQuitMessage(0);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/** https://learn.microsoft.com/en-us/windows/win32/api/winuser/nc-winuser-timerproc */
void CALLBACK TimerProc(HWND hWnd, UINT message, UINT_PTR timerId, DWORD time) { 
    InvalidateRect(hWnd, &m_Rect, true); 
}

int pressToClose(bool success) 
{
    printf("Press any key to close... \n");
    getchar();
    if(success == true) return EXIT_SUCCESS;
    return EXIT_FAILURE;
}
