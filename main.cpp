#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "Timer.hpp"
#include "resource.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Add buttons to the window
void addButtons(HWND hWnd);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR szClassName[] = TEXT("Pomodoro");

static HFONT hFont = CreateFont(FONT_SIZE, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("Arial"));
static HFONT hFontButton = CreateFont(FONT_SIZE_BUTTON, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("Arial"));
static Timer timer(25, 0, TimerState::Pomodoro);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE,  LPSTR lpwCmdLine, int nCmdShow)
{
    HWND hWnd;
    MSG msg;
    WNDCLASSEX wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_M));
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = szClassName;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_SM));
    wc.lpszMenuName = nullptr;

    if(!RegisterClassEx(&wc))
    {
        MessageBox(nullptr, TEXT("Failed to register class"), TEXT("Error"), MB_OK);
        return 0;
    }

    hWnd = CreateWindowEx(0,szClassName, TEXT("Pomodoro"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr, nullptr, hInstance, nullptr);

    if(!hWnd)
    {
        MessageBox(nullptr, TEXT("Failed to create window"), TEXT("Error"), MB_OK);
        return 0;
    }

    
    SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, 0);


    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int x = 10, y = 10;
    static char text[255];
    static char state[30];
    HDC hdc;
    PAINTSTRUCT ps;
    RECT clientRect;
    RECT stateRect = {x, y, 290, 100};

    switch (message)
    {
    case WM_CREATE:
        addButtons(hWnd);
        break;
    case WM_COMMAND:
{       
     KillTimer(hWnd, ID_TIMER);
       switch (LOWORD(wParam))
       {
       case ID_TIMER_POMODORO:
           timer.set_time(25, 0, TimerState::Pomodoro);
           break;
       case ID_TIMER_REST:
           timer.set_time(5, 0, TimerState::ShortBreak);
           break;
       }    
       SetTimer(hWnd, ID_TIMER, 1000, nullptr);
       timer.start();
       wsprintf(state, "%s", timer.get_state().c_str());
       InvalidateRect(hWnd, nullptr, TRUE);
       }
        break;
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            SelectObject(hdc, hFont);
            GetClientRect(hWnd, &clientRect);
            SetBkMode(hdc, TRANSPARENT); // Transparent background for text
            DrawText(hdc, text, 5, &clientRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            SelectObject(hdc, hFontButton);            
            DrawText(hdc, state, strlen(state), &stateRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_TIMER:
        timer.update();
        if(!timer.running()) KillTimer(hWnd, ID_TIMER);
        wsprintf(text, "%s", timer.get_time().c_str());
        InvalidateRect(hWnd, nullptr, TRUE);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, ID_TIMER);
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        if(wParam == VK_ESCAPE) PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void addButtons(HWND hWnd)
{

    int x = 25, y = 200;
    HWND hButton;
    HWND hButton2;
    HWND hButton3;

    hButton = CreateWindow(TEXT("BUTTON"), TEXT("Pomodoro"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, x, y, 120, 50, hWnd, (HMENU)ID_TIMER_POMODORO, nullptr, nullptr);
    x += 120;

    hButton2 = CreateWindow(TEXT("BUTTON"), TEXT("Rest"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, x, y, 120, 50, hWnd, (HMENU)ID_TIMER_REST, nullptr, nullptr);

    SendMessage(hButton, WM_SETFONT, (WPARAM)hFontButton, 0);
    SendMessage(hButton2, WM_SETFONT, (WPARAM)hFontButton, 0);
  
    InvalidateRect(hWnd, nullptr, TRUE);


}
