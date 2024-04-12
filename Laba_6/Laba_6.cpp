// Laba_6.cpp : Определяет точку входа для приложения.
// Яхьяев Данис ДИНРБ-21 Вариант 18 Лабораторная работа №6
// 18. На панели расположить временное окно. В этом окне отобразить клетки для игры в крестики-нолики.
// При первом нажатии левой клавиши мыши в клетке нарисовать крестик, при втором нажатии - нолик.
// Количество клеток задавать с помощью органов управления панели.

#include "framework.h"
#include "Laba_6.h"

#include <windows.h>
#include "Resource.h"
#include "resource1.h"


#define MAX_LOADSTRING 100

// Клетки
const int MAX_SIZE = 5; // Макс кол-во клеток в ряду и столбце

// Глобальные переменные
int board[MAX_SIZE][MAX_SIZE] = { 0 }; // Матрица для хранения состояния клеток: 0 - пустая, 1 - крестик, 2 - нолик
int boardSize = 3; // Размер поля по умолчанию

// Прототипы функций
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawX(HDC hdc, int row, int col, int cellSize);
void DrawO(HDC hdc, int row, int col, int cellSize);
void DrawBoard(HWND hwndDlg, HDC hdc);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
    return 0;
}

int currentCellState = 0; // Переменная для отслеживания текущего состояния клетки (0 - пустая, 1 - крестик, 2 - нолик)

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_INITDIALOG: {
        RECT rect;
        // Устанавливаем максимальное значение для поля ввода
        SendDlgItemMessage(hwndDlg, IDC_EDIT_SIZE, EM_SETLIMITTEXT, 2, 0);

        // Получаем контекст устройства для окна диалога
        HDC hdc = GetDC(GetDlgItem(hwndDlg, IDC_STATIC_BOARD));
        // Получаем размеры клиентской области окна диалога
        GetClientRect(GetDlgItem(hwndDlg, IDC_STATIC_BOARD), &rect);
        // Создаем белую кисть для заливки клеток
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        // Заполняем окно доски белым цветом
        FillRect(hdc, &rect, hBrush);
        // Удаляем кисть
        DeleteObject(hBrush);
        // Освобождаем контекст устройства
        ReleaseDC(GetDlgItem(hwndDlg, IDC_STATIC_BOARD), hdc);

        return TRUE;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDC_BUTTON_OK: {
            currentCellState = 0;
            // Очищаем все клетки, устанавливая их значение в 0
            for (int i = 0; i < MAX_SIZE; ++i) {
                for (int j = 0; j < MAX_SIZE; ++j) {
                    board[i][j] = 0;
                }
            }
            // Сигнализируем о необходимости перерисовки окна
            InvalidateRect(hwndDlg, NULL, TRUE);
            return TRUE;
        }
        case IDC_BUTTON_INC: { // Обработка кнопки увеличения количества клеток
            currentCellState = 0;
            // Очищаем все клетки, устанавливая их значение в 0
            for (int i = 0; i < MAX_SIZE; ++i) {
                for (int j = 0; j < MAX_SIZE; ++j) {
                    board[i][j] = 0;
                }
            }
            if (boardSize < MAX_SIZE) {
                boardSize++;
                InvalidateRect(hwndDlg, NULL, TRUE);
            }
            return TRUE;
        }
        case IDC_BUTTON_DEC: { // Обработка кнопки уменьшения количества клеток
            currentCellState = 0;
            // Очищаем все клетки, устанавливая их значение в 0
            for (int i = 0; i < MAX_SIZE; ++i) {
                for (int j = 0; j < MAX_SIZE; ++j) {
                    board[i][j] = 0;
                }
            }
            if (boardSize > 1) {
                boardSize--;
                InvalidateRect(hwndDlg, NULL, TRUE);
            }
            return TRUE;
        }
        }
        break;
    }
    case WM_LBUTTONDOWN: {
        // Получаем размеры клиентской области окна диалога
        RECT rect;
        GetClientRect(GetDlgItem(hwndDlg, IDC_STATIC_BOARD), &rect);
        int size = rect.right - rect.left;
        int cellSize = size / boardSize;
        // Получаем координаты клика
        int xPos = LOWORD(lParam);
        int yPos = HIWORD(lParam);

        // Определяем номер клетки по координатам
        int row = yPos / cellSize;
        int col = xPos / cellSize;

        // Проверяем, что клик произошел в пределах поля
        if (row >= 0 && row < boardSize && col >= 0 && col < boardSize) {
            // Проверяем, что клетка пустая
            if (board[row][col] == 0) {
                // Устанавливаем крестик или нолик в зависимости от текущего состояния
                if (currentCellState == 0 || currentCellState == 2) {
                    board[row][col] = 1; // Устанавливаем крестик
                    currentCellState = 1; // Обновляем текущее состояние
                }
                else if (currentCellState == 1) {
                    board[row][col] = 2; // Устанавливаем нолик
                    currentCellState = 2; // Обновляем текущее состояние
                }
                // Сигнализируем о необходимости перерисовки окна
                InvalidateRect(hwndDlg, NULL, TRUE);
            }
        }
        return TRUE;
    }
    case WM_CLOSE: {
        EndDialog(hwndDlg, 0);
        return TRUE;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwndDlg, &ps);

        // Отображаем клетки на диалоговом окне
        DrawBoard(hwndDlg, hdc);

        EndPaint(hwndDlg, &ps);
        return TRUE;
    }
    }
    return FALSE;
}


// Функция отрисовки крестика
void DrawX(HDC hdc, int row, int col, int cellSize) {
    int xOffset = cellSize / 100;
    int yOffset = cellSize / 100;
    MoveToEx(hdc, col * cellSize + xOffset, row * cellSize + yOffset, NULL);
    LineTo(hdc, (col + 1) * cellSize - xOffset, (row + 1) * cellSize - yOffset);
    MoveToEx(hdc, col * cellSize + xOffset, (row + 1) * cellSize - yOffset, NULL);
    LineTo(hdc, (col + 1) * cellSize - xOffset, row * cellSize + yOffset);
}

void DrawO(HDC hdc, int row, int col, int cellSize) {
    int xOffset = cellSize / 100;
    int yOffset = cellSize / 100;
    Ellipse(hdc, col * cellSize + xOffset, row * cellSize + yOffset,
        (col + 1) * cellSize - xOffset, (row + 1) * cellSize - yOffset);
}

// Функция отображения клеток на диалоговом окне
void DrawBoard(HWND hwndDlg, HDC hdc) {
    // Получаем размеры клиентской области окна диалога
    RECT rect;
    GetClientRect(GetDlgItem(hwndDlg, IDC_STATIC_BOARD), &rect);
    int size = rect.right - rect.left;
    int cellSize = size / boardSize;

    // Создаем белую кисть для заливки клеток
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));

    // Рисуем клетки
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            RECT cellRect = { j * cellSize, i * cellSize, (j + 1) * cellSize, (i + 1) * cellSize };
            FillRect(hdc, &cellRect, hBrush);
            Rectangle(hdc, cellRect.left, cellRect.top, cellRect.right, cellRect.bottom); // Рисуем границы клеток

            if (board[i][j] == 1) {
                DrawX(hdc, i, j, cellSize);
            }
            else if (board[i][j] == 2) {
                DrawO(hdc, i, j, cellSize);
            }
        }
    }

    // Удаляем кисть
    DeleteObject(hBrush);
}