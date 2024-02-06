#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <string>
#include "definers.h"

class Window
{
protected:
	HWND hWnd = NULL;
	RECT rect = { 0, 0, 0, 0 };
	std::wstring ClassName;
	HACCEL hAccel = NULL;

    /// @brief Регистрация класса окна по параметрам
    /// @param style стиль класса
    /// @param pWndProc указатель на функцию обработки событий окна
    /// @param hInstance дескриптор приложения
    /// @param hIcon дескриптор иконки
    /// @param hCursor дескриптор иконки мышки
    /// @param hbrBackground цвет фона
    /// @param lpszMenuName имя меню
    /// @param lpszClassName имя класс окна
    /// @param hIconSm дескриптор малой иконки
    /// @return флаг регистрации: TRUE/FALSE
    BOOL reg_wnd_class(
        UINT style,
        LRESULT(__stdcall *pWndProc)(HWND, UINT, WPARAM, LPARAM),
        HINSTANCE hInstance,
        HICON hIcon,
        HCURSOR hCursor,
        HBRUSH hbrBackground,
        LPCWSTR lpszMenuName,
        LPCWSTR lpszClassName,
        HICON hIconSm);

public:
    DWORD error = ERROR_SUCCESS; // последняя ошибка

    /// @brief Создание окна с регистацией его класса,
    /// инициализация переменных
    /// @param style стиль класса окна
    /// @param pWndProc указатель на функцию обработки событий окна
    /// @param hInstance дескриптор приложения
    /// @param hIcon иконка в заголовке окна и в дисп. приложений
    /// @param hCursor иконка курсора
    /// @param hbrBackground заливка фона окна
    /// @param lpszMenuName имя меню окна
    /// @param lpszClassName имя класса окна
    /// @param hIconSm маленькая иконка в заголовке окна и в дисп. приложений
    /// @param dwExStyle расширенные стили окна
    /// @param lpWindowName имя заголовка окна
    /// @param dwStyle стили окна
    /// @param nWidth длина окна
    /// @param nHeight высота окна
    /// @param hWndParent дескриптор родительского окна
    /// @param hMenu дескриптор меню
    /// @param lpParam пользовательское сообщение окну
    /// @param cmdShow режим показа окна
    /// @return код завершения метода, коды описаны в заглавии файла
    BOOL create(
        UINT style,
        LRESULT(__stdcall *pWndProc)(HWND, UINT, WPARAM, LPARAM),
        HINSTANCE hInstance,
        HICON hIcon,
        HCURSOR hCursor,
        HBRUSH hbrBackground,
        LPCWSTR lpszMenuName,
        LPCWSTR lpszClassName,
        HICON hIconSm,
        DWORD dwExStyle,
        LPCWSTR lpWindowName,
        DWORD dwStyle,
        INT nWidth, INT nHeight,
        HWND hWndParent,
        HMENU hMenu,
        LPVOID lpParam,
        INT cmdShow);

    /// @brief Уничтожение окна
    VOID destroy(void);

    /// @brief Обработать все сообщения системы для этого окна
    VOID peek_msgs(void) const;

    /// @brief Запуск обработки сообщений окна с условием выхода из цикла
    VOID get_msgs(bool (*cycle_condition)(void));

    /// @brief Показать окно
    /// @return Код возврата - 0 || 1
    BOOL show(int nCmdShow);

    BOOL update_rect(VOID);

    VOID set_focus(VOID);

    VOID set_pos(INT x, INT y);

    VOID set_pos_center(VOID);

    HWND get_hWnd(VOID) const;

    RECT *get_rect(VOID);

    int get_width(void) const;

    int get_height(void) const;

    int get_menu_height(void) const;

};

#endif // WINDOW_H
