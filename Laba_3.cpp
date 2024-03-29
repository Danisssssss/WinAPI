// Laba_3.cpp : Определяет точку входа для приложения.
// Яхьяев Данис ДИНРБ-21 Вариант 18 Лабораторная работа №3
// 18. В окне отобразить клетки для игры в крестики-нолики. При нажатии левой клавиши
// мыши в клетке нарисовать крестик, а правой - нолик. Запретить заполнять клетку более
// одного раза. Рекомендация: если при перемещении курсора бьша нажата левая клавиша мыши,
// то параметр wParam содержит значение MK_LBUTTON.

#include "framework.h"
#include "Laba_3.h"

#define MAX_LOADSTRING 100

// Глобальные переменные
const int CELL_SIZE = 100;  // размер клетки
const int BOARD_SIZE = 3;   // размер доски (3 на 3)
int board[BOARD_SIZE][BOARD_SIZE] = { 0 };

// Прототипы функций
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void DrawX(HDC hdc, int row, int col);
void DrawO(HDC hdc, int row, int col);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Регистрация класса окна
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1), NULL, L"MainWindowClass", NULL };
    RegisterClassEx(&wc);

    RECT windowRect = { 0, 0, BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    // Создание окна
    HWND hWnd = CreateWindowEx(0, L"MainWindowClass", L"Крестики-нолики", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);

    // Цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Функция отрисовки крестика
void DrawX(HDC hdc, int row, int col) {
    MoveToEx(hdc, col * CELL_SIZE, row * CELL_SIZE, NULL);
    LineTo(hdc, (col + 1) * CELL_SIZE, (row + 1) * CELL_SIZE);
    MoveToEx(hdc, col * CELL_SIZE, (row + 1) * CELL_SIZE, NULL);
    LineTo(hdc, (col + 1) * CELL_SIZE, row * CELL_SIZE);
}

// Функция отрисовки нолика
void DrawO(HDC hdc, int row, int col) {
    Ellipse(hdc, col * CELL_SIZE, row * CELL_SIZE, (col + 1) * CELL_SIZE, (row + 1) * CELL_SIZE);
}

// Обработчик сообщений
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Отрисовка сетки клеток
        for (int i = 1; i < BOARD_SIZE; ++i) {
            MoveToEx(hdc, i * CELL_SIZE, 0, NULL);
            LineTo(hdc, i * CELL_SIZE, BOARD_SIZE * CELL_SIZE);
            MoveToEx(hdc, 0, i * CELL_SIZE, NULL);
            LineTo(hdc, BOARD_SIZE * CELL_SIZE, i * CELL_SIZE);
        }

        // Отрисовка крестиков и ноликов
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == 1) {
                    DrawX(hdc, i, j);
                }
                else if (board[i][j] == 2) {
                    DrawO(hdc, i, j);
                }
            }
        }

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN: {
        int row = HIWORD(lParam) / CELL_SIZE;
        int col = LOWORD(lParam) / CELL_SIZE;

        if (board[row][col] == 0) {
            board[row][col] = 1;                // Установка крестика
            InvalidateRect(hWnd, NULL, TRUE);   // Перерисовка окна
        }
        break;
    }
    case WM_RBUTTONDOWN: {
        int row = HIWORD(lParam) / CELL_SIZE;
        int col = LOWORD(lParam) / CELL_SIZE;

        if (board[row][col] == 0) {
            board[row][col] = 2;                // Установка нолика
            InvalidateRect(hWnd, NULL, TRUE);   // Перерисовка окна
        }
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