#include "window.h"

BOOL Window::reg_wnd_class(
    UINT style,
    LRESULT(__stdcall *pWndProc)(HWND, UINT, WPARAM, LPARAM),
    HINSTANCE hInstance,
    HICON hIcon,
    HCURSOR hCursor,
    HBRUSH hbrBackground,
    LPCWSTR lpszMenuName,
    LPCWSTR lpszClassName,
    HICON hIconSm)
{
    WNDCLASSEX wcx;

    ZeroMemory(&wcx, sizeof(WNDCLASSEX));

    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = style;
    wcx.lpfnWndProc = (WNDPROC)pWndProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = hInstance;
    wcx.hIcon = hIcon;
    wcx.hCursor = hCursor;
    wcx.hbrBackground = hbrBackground;
    wcx.lpszMenuName = lpszMenuName;
    wcx.lpszClassName = lpszClassName;
    wcx.hIconSm = hIconSm;

    return !RegisterClassEx(&wcx) ? FALSE : TRUE;
}

BOOL Window::create(
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
    INT cmdShow)
{
    BOOL rs; // возврат
    WNDCLASSEX wcx; // структура класса окна

    // Проверка на то, что дескриптор окна ни на что не указывает
    rs = IsWindow(hWnd) ? ERR_WND_CPY_HWND : EXIT_SUCCESS;

    // Проверка, зарегистрирован ли класс
    if (rs == EXIT_SUCCESS && GetClassInfoEx(hInstance, this->ClassName.c_str(), &wcx))
        rs = ERR_WND_CPY_CLASS;

    // Попытка регистрации класса окна
    if (rs == EXIT_SUCCESS &&
        !reg_wnd_class(
        style,
        pWndProc,
        hInstance,
        hIcon,
        hCursor,
        hbrBackground,
        lpszMenuName,
        lpszClassName,
        hIconSm)
        )
    {
        // reset_and_free_data();
        error = GetLastError();
        return ERR_WND_GEN_CLASS;
    }

    // класс успешно зарегистрирован
    this->ClassName.assign(lpszClassName);

    // Учет влияния стиля окна на его размеры
    this->rect = { 0, 0, nWidth , nHeight };
    if (rs == EXIT_SUCCESS && !AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle))
    {
        // reset_and_free_data();
        error = GetLastError();
        rs = ERR_WND_ADJ_RECT;
    }

    if (rs == EXIT_SUCCESS)
    {
        // Попытка создания окна
        hWnd = CreateWindowEx(
            dwExStyle,
            lpszClassName,
            lpWindowName,
            dwStyle,
            0, 0,
            nWidth, nHeight,
            hWndParent,
            hMenu,
            hInstance,
            lpParam);

        // Проверка создания окна
        if (!IsWindow(hWnd))
        {
            // reset_and_free_data();
            error = GetLastError();
            rs = ERR_WND_GEN_HWND;
        }
    }

    if (rs == EXIT_SUCCESS)
    {
        // переустановить курсор (чтобы при создании окна, мышь сразу была правильной)
        SetCursor(hCursor);
        this->rect = { 0, 0, nWidth, nHeight };
        this->set_pos(0, 0);
    }

    // успешный возврат
    return rs;
}

VOID Window::destroy(void)
{
    // Уничтожение окна
    if (hWnd != NULL)
    {
        DestroyWindow(hWnd);
        hWnd = NULL;
    }

    // Отмена регистрации класса окна
    if (!ClassName.empty())
    {
        UnregisterClass(ClassName.c_str(), GetModuleHandle(NULL));
        ClassName.clear();
    }

    // Освобождение ускорителя клавиш, если он был создан
    if (hAccel != NULL)
    {
        DestroyAcceleratorTable(hAccel);
        hAccel = NULL;
    }
}

VOID Window::peek_msgs(void) const
{
    MSG msg;

    while (PeekMessageW(&msg, this->hWnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

VOID Window::get_msgs(bool (*cycle_condition)(void))
{
    if (!cycle_condition)
        return;

    MSG msg;

    while (cycle_condition() && GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

BOOL Window::show(int nCmdShow)
{
    BOOL rs = EXIT_SUCCESS;
    ShowWindow(this->hWnd, nCmdShow);
    UpdateWindow(this->hWnd);

    if (GetLastError() != ERROR_SUCCESS)
    {
        // reset_and_free_data();
        error = GetLastError();
        rs = EXIT_FAILURE;
    }

    return rs;
}

BOOL Window::update_rect(VOID)
{
    if (!this->hWnd)
        return EXIT_FAILURE;

    BOOL rs = EXIT_SUCCESS;

    // DWORD wdStyle = GetWindowLongPtr(this->hWnd, GWL_STYLE);
    // DWORD dwExStyle = GetWindowLongPtr(this->hWnd, GWL_EXSTYLE);

    if (!GetWindowRect(this->hWnd, &this->rect))
        rs = EXIT_FAILURE;

    /*if (rs != EXIT_FAILURE && !AdjustWindowRectEx(&this->rect, wdStyle, FALSE, dwExStyle))
        rs = EXIT_FAILURE;*/

    return rs;
}

VOID Window::set_focus(VOID)
{
    if (this->hWnd)
        SetFocus(this->hWnd);
}

VOID Window::set_pos(INT x, INT y)
{
    int wnd_width = this->get_width();
    int wnd_height = this->get_height();

    this->rect.left = x;
    this->rect.top = y;

    SetWindowPos(
        hWnd, HWND_NOTOPMOST,
        this->rect.left, this->rect.top,
        wnd_width, wnd_height,
        SWP_FRAMECHANGED
    );
}

VOID Window::set_pos_center(VOID)
{
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    int wnd_width = this->get_width();
    int wnd_height = this->get_height();

    this->rect.left = (screen_width - wnd_width) / 2;
    this->rect.top = (screen_height - wnd_height) / 2;

    this->rect.right = this->rect.left + wnd_width;
    this->rect.bottom = this->rect.top + wnd_height;

    SetWindowPos(
        hWnd, HWND_NOTOPMOST,
        this->rect.left, this->rect.top,
        wnd_width, wnd_height,
        SWP_FRAMECHANGED
    );

    return VOID();
}

HWND Window::get_hWnd(VOID) const
{
    return this->hWnd;
}

RECT *Window::get_rect(VOID)
{
    return &this->rect;
}

int Window::get_width(void) const
{
    return this->rect.right - this->rect.left;
}

int Window::get_height(void) const
{
    return this->rect.bottom - this->rect.top;
}

int Window::get_menu_height(void) const
{
    int height = 0;
    HMENU hMenu = GetMenu(this->hWnd);
    MENUINFO menuInfo;

    if (hMenu != nullptr)
    {
        GetMenuInfo(hMenu, &menuInfo);
        height = menuInfo.cyMax;
    }

    return height;
}
