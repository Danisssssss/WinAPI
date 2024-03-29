// Laba_1.cpp : Определяет точку входа для приложения.
// Яхьяев Данис ДИНРБ-21 Вариант 18 Лабораторная работа №1
// 18. Окно приложения занимает четверть экрана и расположено в левом верхнем углу.
// Создать временное окно такого же размера в правом нижнем углу экрана. Любое окно
// после нажатия левой клавиши мыши перемещается в свободный по ходу часовой стрелки
// угол.

#include "framework.h"
#include "Laba_1.h"

#define MAX_LOADSTRING 100

// Глобальные переменные
HWND hWndMain, hWndTemp;
RECT screenRect;

// Прототипы функций
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Получение размеров экрана
    screenRect = { 0 };
    GetClientRect(GetDesktopWindow(), &screenRect);

    // Регистрация класса окна
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1), NULL, L"MainWindowClass", NULL };
    RegisterClassEx(&wc);

    // Создание главного окна
    hWndMain = CreateWindowEx(0, L"MainWindowClass", L"Основное окно", WS_OVERLAPPEDWINDOW, 0, 0, screenRect.right / 2, screenRect.bottom / 2, NULL, NULL, hInstance, NULL);
    ShowWindow(hWndMain, nCmdShow);

    // Создание временного окна
    hWndTemp = CreateWindowEx(0, L"MainWindowClass", L"Временное окно", WS_OVERLAPPEDWINDOW, screenRect.right / 2, screenRect.bottom / 2, screenRect.right / 2, screenRect.bottom / 2, NULL, NULL, hInstance, NULL);
    ShowWindow(hWndTemp, nCmdShow);

    // Цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Обработчик сообщений
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        // Получаем текущие координаты окна
        RECT windowRect;
        GetWindowRect(hWnd, &windowRect);

        // Перемещаем окно в свободный угол
        if (windowRect.left == screenRect.left && windowRect.top == screenRect.top) {
            // Окно в левом верхнем углу, перемещаем в правый верхний
            SetWindowPos(hWnd, NULL, screenRect.right / 2, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
        else if (windowRect.left == screenRect.right / 2 && windowRect.top == 0) {
            // Окно в правом верхнем углу, перемещаем в правый нижний
            SetWindowPos(hWnd, NULL, screenRect.right / 2, screenRect.bottom / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
        else if (windowRect.left == screenRect.right / 2 && windowRect.top == screenRect.bottom / 2) {
            // Окно в правом нижнем углу, перемещаем в левый нижний
            SetWindowPos(hWnd, NULL, 0, screenRect.bottom / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
        else {
            // Окно в левом нижнем углу, перемещаем в левый верхний
            SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
