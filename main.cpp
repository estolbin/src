#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#define ID_TIMER 100

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR szClassName[] = TEXT("Pomodoro");

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE,  LPSTR lpwCmdLine, int nCmdShow)
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
    wc.lpszClassName = szClassName;
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
    static char * symbol = new char[100]; 
    static int size = 0;
    static int time = 0;
    static char text[255];
    HDC hdc;
    PAINTSTRUCT ps;
    RECT clientRect;
    RECT rect{x, y, x + 1000, y + 1000};
    HFONT hFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("Arial"));

    switch (message)
    {
    case WM_PAINT:
        {

            hdc = BeginPaint (hWnd, &ps); 
            SelectObject(hdc, hFont);
            GetClientRect(hWnd, &clientRect);
            if(size > 0)
                DrawText(hdc, symbol, size, &clientRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            DrawText(hdc, text, 15, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            EndPaint (hWnd, &ps)  ;  
        }
        break;
    case WM_TIMER:
        time++;
        wsprintf(text, "Time: %d", time);
        //SetWindowText(hWnd, text);
        InvalidateRect(hWnd, nullptr, TRUE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SETFONT:
        SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, 0);
        break;
    case WM_KEYDOWN:
        if(wParam == VK_ESCAPE)
                PostQuitMessage(0);
        break;
    case WM_CHAR:
        if(wParam == VK_RETURN)
        {
            size = 0;
            SetTimer(hWnd, ID_TIMER, 1000, nullptr);            
        }
        else if (wParam == VK_BACK)
        {
            if(size > 0)
                size--;
            InvalidateRect(hWnd, nullptr, TRUE);
        }
        else 
        {
            symbol[size] = (wchar_t)wParam;
            size++;
        }
        InvalidateRect(hWnd, nullptr, TRUE);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
