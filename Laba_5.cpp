// Laba_5.cpp : Определяет точку входа для приложения.
// Яхьяев Данис ДИНРБ-21 Вариант 18 Лабораторная работа №5
// 18. О действиях с фигурами и состоянии соответствующих строк меню сообщать в строке состояния.


#include "framework.h"
#include "Laba_5.h"

#include <windows.h>
#include <string>
#include <cmath>
#include <vector>
#include <commctrl.h>
#pragma comment (lib, "comctl32.lib")

#define MAX_LOADSTRING 100

// Глобальные переменные
HWND g_hMainWindow; // Главное окно
HWND g_hFigureDisplay; // Окно для отображения фигур
HWND g_hStatusBar; // Строка состояния
int g_SelectedFigureIndex = -1; // Индекс выбранной фигуры

// Прототипы функций
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CreateMainMenu(HWND hWnd);
void DrawCircle(HDC hdc, int centerX, int centerY, int radius);
void DrawRectangle(HDC hdc, int left, int top, int right, int bottom);
void DrawTriangle(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3);
void UpdateStatusBarText(const wchar_t* text); // Функция для обновления текста в строке состояния

// Определение ID для меню
#define IDM_FIGURE_MENU 101
#define IDM_FIGURE_LIST 102

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1), NULL, L"MainWindowClass", NULL };
    RegisterClassEx(&wc);

    g_hMainWindow = CreateWindowEx(0, L"MainWindowClass", L"Окно с фигурками", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    // Создаем строку состояния
    g_hStatusBar = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, g_hMainWindow, 0);
    UpdateStatusBarText(L""); // Изначально строка состояния пуста

    ShowWindow(g_hMainWindow, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Создание главного меню
void CreateMainMenu(HWND hWnd) {
    HMENU hMenu = CreateMenu();
    HMENU hSubMenu = CreatePopupMenu();

    AppendMenu(hSubMenu, MF_STRING, IDM_FIGURE_LIST, L"Круг");
    AppendMenu(hSubMenu, MF_STRING, IDM_FIGURE_LIST + 1, L"Прямоугольник");
    AppendMenu(hSubMenu, MF_STRING, IDM_FIGURE_LIST + 2, L"Треугольник");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"Фигуры");
    SetMenu(hWnd, hMenu);
}

// Отображение выбранной фигуры
void DisplaySelectedFigure(int index) {
    g_SelectedFigureIndex = index;
    if (g_hFigureDisplay != NULL) {
        DestroyWindow(g_hFigureDisplay);
    }

    switch (g_SelectedFigureIndex) {
    case 0:
        UpdateStatusBarText(L"Выбран круг");
        break;
    case 1:
        UpdateStatusBarText(L"Выбран прямоугольник");
        break;
    case 2:
        UpdateStatusBarText(L"Выбран треугольник");
        break;
    default:
        UpdateStatusBarText(L"");
        break;
    }

    InvalidateRect(g_hMainWindow, NULL, TRUE);
}

// Рисование круга
void DrawCircle(HDC hdc, int centerX, int centerY, int radius) {
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
}

// Рисование прямоугольника
void DrawRectangle(HDC hdc, int left, int top, int right, int bottom) {
    Rectangle(hdc, left, top, right, bottom);
}

// Рисование треугольника
void DrawTriangle(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3) {
    POINT points[3] = { {x1, y1}, {x2, y2}, {x3, y3} };
    Polygon(hdc, points, 3);
}

// Обновление текста в строке состояния
void UpdateStatusBarText(const wchar_t* text) {
    SendMessage(g_hStatusBar, SB_SETTEXT, 0, (LPARAM)text);
}

// Обработчик сообщений
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        CreateMainMenu(hWnd);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_FIGURE_LIST: // Круг
        case IDM_FIGURE_LIST + 1: // Прямоугольник
        case IDM_FIGURE_LIST + 2: // Треугольник
            DisplaySelectedFigure(LOWORD(wParam) - IDM_FIGURE_LIST);
            CheckMenuRadioItem(GetSubMenu(GetMenu(hWnd), 0), IDM_FIGURE_LIST, IDM_FIGURE_LIST + 2, LOWORD(wParam), MF_BYCOMMAND); // Отметить выбранный пункт в меню
            break;
        default:
            break;
        }
        break;
    case WM_PAINT: {
        RECT rect;
        GetClientRect(g_hMainWindow, &rect);

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        switch (g_SelectedFigureIndex) {
        case 0: // Круг
            DrawCircle(hdc, rect.right / 2, rect.bottom / 2, 180);
            break;
        case 1: // Прямоугольник
            DrawRectangle(hdc, rect.right / 2 - 180, rect.bottom / 2 - 180, rect.right / 2 + 180, rect.bottom / 2 + 180);
            break;
        case 2: // Треугольник
            DrawTriangle(hdc, rect.right / 2 - 180, rect.bottom / 2 + 180, rect.right / 2 + 180, rect.bottom / 2 + 180, rect.right / 2, rect.bottom / 2 - 180);
            break;
        default:
            break;
        }
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}