#ifndef GAME_WINDOW_PROC_H
#define GAME_WINDOW_PROC_H

#include "window.h"
#include "app_args.h"

void update_child_game_window_position(void);

LRESULT game_window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // GAME_WINDOW_PROC_H