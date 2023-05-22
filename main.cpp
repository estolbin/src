#include <windows.h>
#include "PomodoroTime.h"
#include "Pomodoro.h"

#define ID_TIMER 100

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

Pomodoro pomodoro = Pomodoro(1, TaskType::WORK);
RECT clientRect;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE,  LPSTR lpwCmdLine, int nCmdShow)
{
    HWND hWnd;
    MSG msg;
    WNDCLASSEX wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "Pomodoro";
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.lpszMenuName = nullptr;

    if(!RegisterClassEx(&wc))
    {
        MessageBox(nullptr, "Failed to register class", "Error", MB_OK);
        return 0;
    }

    hWnd = CreateWindowEx(0,"Pomodoro", "Pomodoro", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    GetClientRect(hWnd, &clientRect);

    SetTimer(hWnd, ID_TIMER, 1000, NULL);

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc; 
    PAINTSTRUCT ps; 
    char szBuffer[256];
    bool running = false;

    switch (message)
    {
        
    case WM_CREATE:

        break;
    case WM_PAINT:
        {
            if(!running)
            {
                running = true;
                pomodoro.startTimer();
            }
            hdc = BeginPaint (hWnd, &ps); 
            wsprintf (szBuffer, "Pomodoro %s %s", pomodoro.typePomodoro().c_str(),pomodoro.getPomodoroTime().c_str()); 
            DrawText (hdc, szBuffer, -1, &clientRect, DT_LEFT); 
            EndPaint (hWnd, &ps)  ;  
        }
        break;

    case WM_TIMER:
        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_DESTROY:
        KillTimer(hWnd, ID_TIMER);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
