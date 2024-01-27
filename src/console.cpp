#include "console.h"

bool Console::redirect_io() const
{
    bool result = true;
    FILE *fp;

    // Redirect STDIN if the console has an input handle
    if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE)
    {
        if (freopen_s(&fp, "CONIN$", "r", stdin) != 0)
            result = false;
        else
            setvbuf(stdin, NULL, _IONBF, 0);
    }

    // Redirect STDOUT if the console has an output handle
    if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
    {
        if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
            result = false;
        else
            setvbuf(stdout, NULL, _IONBF, 0);
    }

    // Redirect STDERR if the console has an error handle
    if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
    {
        if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0)
            result = false;
        else
            setvbuf(stderr, NULL, _IONBF, 0);
    }

    // Make C++ standard streams point to console as well.
    std::ios::sync_with_stdio(true);

    // Clear the error state for each of the C++ standard streams.
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::cin.clear();

    return result;
}

void Console::adjust_buffer() const
{
    CONSOLE_SCREEN_BUFFER_INFO conInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo);
    if (conInfo.dwSize.Y < MIN_BUFFER_SIZE)
        conInfo.dwSize.Y = MIN_BUFFER_SIZE;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), conInfo.dwSize);
}

BOOL __stdcall Console::ConsoleCtrlHandler(DWORD dwCtrlType)
{
    switch (dwCtrlType)
    {
    case CTRL_CLOSE_EVENT:
        std::cout << "Консольное окно закрывается!" << std::endl;
        return TRUE;  // предотвратить завершение выполнения программы

    default:
        return FALSE;
    }
}

bool Console::create() const
{
    bool result = false;

    // удалить предыдущую консоль
    destroy();

    // создание консоли
    if (AllocConsole())
    {
        adjust_buffer();
        result = redirect_io();
    }

    if (result && !SetConsoleCtrlHandler(this->ConsoleCtrlHandler, TRUE))
    {
        result = true;
    }

    return result;
}

bool Console::destroy() const
{
    bool result = true;
    FILE *fp;

    // Оключить поток ввода
    if (freopen_s(&fp, "NUL:", "r", stdin) != 0)
        result = false;
    else
        setvbuf(stdin, NULL, _IONBF, 0);

    // Отключить поток вывода
    if (freopen_s(&fp, "NUL:", "w", stdout) != 0)
        result = false;
    else
        setvbuf(stdout, NULL, _IONBF, 0);

    // Отключить поток ошибок
    if (freopen_s(&fp, "NUL:", "w", stderr) != 0)
        result = false;
    else
        setvbuf(stderr, NULL, _IONBF, 0);

    // Уничтожить консоль
    if (!FreeConsole())
        result = false;

    return result;
}

void Console::set_size(INT width, INT height) const
{
    RECT rect;
    HWND hWnd = GetConsoleWindow();
    GetWindowRect(hWnd, &rect);
    MoveWindow(hWnd, rect.left, rect.top, width, height, TRUE);
}

void Console::set_pos(INT x, INT y) const
{
    RECT rect;
    HWND hWnd = GetConsoleWindow();
    GetWindowRect(hWnd, &rect);
    SetWindowPos(
        hWnd, HWND_NOTOPMOST,
        x, y, rect.right - rect.left, rect.bottom - rect.top,
        TRUE);
}

void Console::set_curet_pos(SHORT x, SHORT y) const
{
    HANDLE hWnd = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD new_pos = { x, y };
    SetConsoleCursorPosition(hWnd, new_pos);
}

int Console::get_curet_row(void) const
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    return GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi) ? csbi.dwCursorPosition.Y : -1;
}

int Console::get_curet_col(void) const
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    return GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi) ? csbi.dwCursorPosition.X : -1;
}

void Console::get_curet_pos(int *row, int *col) const
{
    *row = get_curet_row();
    *col = get_curet_col();
}