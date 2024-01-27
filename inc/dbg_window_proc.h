#ifndef DBG_WINDOW_PROC_H
#define DBG_WINDOW_PROC_H

#include "window.h"
#include "app_args.h"
#include "game_window_proc.h"
#include "resource.h"

void switch_menu_state(HWND hWnd, WPARAM wParam);

LRESULT debug_window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void launch_debug_msg_loop(HWND hWnd);

#endif // DBG_WINDOW_PROC_H