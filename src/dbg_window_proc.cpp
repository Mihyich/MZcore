#include "dbg_window_proc.h"

void update_child_game_window_position(void)
{
    RECT dbg_wnd_rect;
    POINT top_left;
    POINT bot_right;
    HWND parent_hWnd = app::dbg_wnd.get_hWnd();

    GetClientRect(parent_hWnd, &dbg_wnd_rect);
    // AdjustWindowRect(&dbg_wnd_rect, GetWindowLongPtr(parent_hWnd, GWL_STYLE), TRUE);

    top_left = { dbg_wnd_rect.left, dbg_wnd_rect.top };
    bot_right = { dbg_wnd_rect.right, dbg_wnd_rect.bottom };

    ClientToScreen(parent_hWnd, &top_left);
    ClientToScreen(parent_hWnd, &bot_right);

    INT width = bot_right.x - top_left.x - 10;
    INT height = bot_right.y - top_left.y - 10;

    SetWindowPos(
        app::game_wnd.get_hWnd(), HWND_BOTTOM,
        0, 0,
        width, height,
        SWP_FRAMECHANGED);

    /*UpdateWindow(app::game_wnd.get_hWnd());
    app::game_wnd.peek_msgs();*/
}

void switch_menu_state(HWND hWnd, WPARAM wParam)
{
    if (!hWnd)
        return;

    HMENU hMenu;
    UINT uCheck;

    if ((hMenu = GetMenu(hWnd)) != NULL)
    {
        uCheck = GetMenuState(hMenu, LOWORD(wParam), MF_BYCOMMAND);
        CheckMenuItem(hMenu, LOWORD(wParam), uCheck == MF_CHECKED ? MF_UNCHECKED : MF_CHECKED);
    }
}

LRESULT debug_window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        return EXIT_SUCCESS;

    case WM_SIZE:
        app::dbg_wnd.update_rect();
        if (app::game_wnd.get_hWnd()) update_child_game_window_position();
        return EXIT_SUCCESS;

    case WM_MOVE:
        app::dbg_wnd.update_rect();
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

void launch_debug_msg_loop(HWND hWnd)
{
    MSG msg;

    while (PeekMessageW(&msg, hWnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
