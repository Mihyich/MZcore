#include "game_window_proc.h"

LRESULT game_window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        return EXIT_SUCCESS;

    case WM_SIZE:
        app::game_wnd.update_rect();
        return EXIT_SUCCESS;

    case WM_MOVE:
        app::game_wnd.update_rect();
        return EXIT_SUCCESS;

    case WM_CLOSE:
        app::game_cycle_work = false;
        return EXIT_SUCCESS;

    case WM_DESTROY:
        app::game_cycle_work = false;
        PostQuitMessage(0);
        return EXIT_SUCCESS;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}