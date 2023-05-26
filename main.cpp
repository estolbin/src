#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <io.h>     // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include "Timer.hpp"
#include "resource.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Add buttons to the window
void add_buttons(HWND hWnd);
void loadSettings();
void saveSettings();
bool directoryExists(std::string path);
std::string getFileName();
void timerEnded(HWND hParent, Timer& timer);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR szClassName[] = TEXT("Pomodoro");

int FONT_SIZE = 100;
int FONT_SIZE_BUTTON = 25;
int WINDOW_WIDTH = 300;
int WINDOW_HEIGHT = 300;

static HFONT hFont;
static HFONT hFontButton;
static Timer timer(25, 0, TimerState::Pomodoro);

static std::map<std::string, std::string> settings;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpwCmdLine, int nCmdShow)
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

    loadSettings();

    hFont = CreateFont(FONT_SIZE, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("Arial"));
    hFontButton = CreateFont(FONT_SIZE_BUTTON, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("Arial"));

    if (!RegisterClassEx(&wc))
    {
        MessageBox(nullptr, TEXT("Failed to register class"), TEXT("Error"), MB_OK);
        return 0;
    }

    hWnd = CreateWindowEx(0, szClassName, TEXT("Pomodoro"), WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
                          nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        MessageBox(nullptr, TEXT("Failed to create window"), TEXT("Error"), MB_OK);
        return 0;
    }

    SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, 0);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);

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
        add_buttons(hWnd);
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
        if (!timer.running())
        {
            KillTimer(hWnd, ID_TIMER);
            timerEnded(hWnd, timer);
        }
        wsprintf(text, "%s", timer.get_time().c_str());
        InvalidateRect(hWnd, nullptr, TRUE);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, ID_TIMER);
        saveSettings();
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void add_buttons(HWND hwnd)
{
    int button_x = 25, button_y = 200;
    HWND pomodoro_button;
    HWND rest_button;

    pomodoro_button = CreateWindow(TEXT("BUTTON"), TEXT("Pomodoro"),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, button_x, button_y, 120, 50,
        hwnd, (HMENU)ID_TIMER_POMODORO, nullptr, nullptr);
    button_x += 120;

    rest_button = CreateWindow(TEXT("BUTTON"), TEXT("Rest"),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, button_x, button_y, 120, 50,
        hwnd, (HMENU)ID_TIMER_REST, nullptr, nullptr);

    SendMessage(pomodoro_button, WM_SETFONT, (WPARAM)hFontButton, 0);
    SendMessage(rest_button, WM_SETFONT, (WPARAM)hFontButton, 0);

    InvalidateRect(hwnd, nullptr, TRUE);
}


void loadSettings()
{
    std::string file = getFileName();

    settings.clear();

    std::ifstream iniFile(file);
    if (!iniFile.is_open())
        return;

    std::string line;
    while (std::getline(iniFile, line))
    {
        if (line.empty() || line[0] == ';' || line[0] == '#')
            continue;

        auto delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos)
            continue;

        auto key = line.substr(0, delimiterPos);
        auto value = line.substr(delimiterPos + 1);

        settings[key] = value;
    }

    iniFile.close();

    FONT_SIZE = std::stoi(settings["FONT_SIZE"], nullptr, 10);
    FONT_SIZE_BUTTON = std::stoi(settings["FONT_SIZE_BUTTON"], nullptr, 10);
    WINDOW_HEIGHT = std::stoi(settings["WINDOW_HEIGHT"], nullptr, 10);
    WINDOW_WIDTH = std::stoi(settings["WINDOW_WIDTH"], nullptr, 10);
}

void saveSettings()
{
    std::string file = getFileName();
   
   std::ofstream iniFile(file);
    if (iniFile.is_open())
    {
        iniFile << "FONT_SIZE=" << FONT_SIZE << std::endl;
        iniFile << "FONT_SIZE_BUTTON=" << FONT_SIZE_BUTTON << std::endl;
        iniFile << "WINDOW_HEIGHT=" << WINDOW_HEIGHT << std::endl;
        iniFile << "WINDOW_WIDTH=" << WINDOW_WIDTH << std::endl;
    }
    iniFile.close();
}

bool directoryExists(std::string path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

std::string getFileName()
{
    std::string folder = getenv("USERPROFILE");
    folder += "\\Pomodoro";

    if (!directoryExists(folder))
        CreateDirectory(folder.c_str(), NULL);


    std::string file = folder + "\\settings.ini";

    return file;
}

void timerEnded(HWND hWnd, Timer timer)
{
}