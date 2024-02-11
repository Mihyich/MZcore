#ifndef APP_ARGS_H
#define APP_ARGS_H

#include "window.h"
#include "console.h"

namespace app
{
	extern bool debugging;

    extern HINSTANCE hInstance; // дескриптор приложения
	extern Window game_wnd; // игровое окно
	extern Console cnsl; // консоль

	extern bool use_realtime_settings_change;
	extern bool game_cycle_work;
}

#endif // !APP_ARGS_H
