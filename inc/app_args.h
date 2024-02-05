#ifndef APP_ARGS_H
#define APP_ARGS_H

#include "window.h"
#include "console.h"
#include "dbg_wnd_data.h"
#include "vulkan_renderer.h"

namespace app
{
    extern bool vk_debug;
	extern bool debugging;

	extern Window game_wnd; // игровое окно
	extern Window dbg_wnd;  // окно отладки
	extern Console cnsl; // консоль

	extern Graphics::Vulkan_Renderer vk_renderer; // графический контекст Vulkan

	extern bool use_realtime_settings_change;

	extern bool game_cycle_work;
}

#endif // !APP_ARGS_H
