// Laba_2.cpp : Определяет точку входа для приложения.
// Яхьяев Данис ДИНРБ-21 Вариант 18 Лабораторная работа №2
// 18. В рабочей области окна перечислить названия изучаемых в семестре дисциплин
// и пометить названия изучаемых сегодня дисциплин. Выделить' среди изучаемых
// сегодня дисциплин те, по которым нет экзамена.

#include "framework.h"
#include "Laba_2.h"

#include <string>
#include <vector>

#define MAX_LOADSTRING 100

// Глобальные переменные
HWND hWnd;

// Список дисциплин и наличие по ним экзамена
std::vector<std::pair<std::wstring, bool>> subjects = {
    {L"Дисциплина 1", false},
    {L"Дисциплина 2", true},
    {L"Дисциплина 3", false},
    {L"Дисциплина 4", true},
    {L"Дисциплина 5", false},
    {L"Дисциплина 6", true},
    {L"Дисциплина 7", false},
    {L"Дисциплина 8", true}
};

std::vector<int> todaySubjects = { 0, 1, 2, 3 }; // Индексы дисциплин, которые изучаются сегодня
// Прототипы функций
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Регистрация класса окна
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1), NULL, L"MainWindowClass", NULL };
    RegisterClassEx(&wc);

    // Создание окна
    HWND hWnd = CreateWindowEx(0, L"MainWindowClass", L"Список дисциплин", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);

    // Цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Функция рисования текста на окне
void DrawTextOnWindow(HDC hdc, int x, int y, const std::wstring& text, bool highlight) {
    SetTextColor(hdc, highlight ? RGB(0, 128, 0) : RGB(0, 0, 0)); // Красный цвет для выделения, черный для обычного текста
    TextOut(hdc, x, y, text.c_str(), text.size());
}

// Обработчик сообщений
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Отображение списка дисциплин
        int y = 20;
        for (size_t i = 0; i < subjects.size(); ++i) {
            std::wstring subjectText = subjects[i].first;
            bool isToday = std::find(todaySubjects.begin(), todaySubjects.end(), i) != todaySubjects.end();
            bool hasExam = subjects[i].second;

            // Добавляем пометку "Сегодня" к дисциплине, которая изучается сегодня
            if (isToday) {
                subjectText += L" (Сегодня)";
            }

            // Добавляем пометку "Без экзамена" к дисциплине, по которой нет экзамена
            if (!hasExam) {
                subjectText += L" (Без экзамена)";
            }

            // Рисуем текст на окне, выделяя сегодняшнюю дисциплину и те, по которым нет экзамена
            DrawTextOnWindow(hdc, 20, y, subjectText, isToday && !hasExam);

            y += 20; // Переходим на следующую строку
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