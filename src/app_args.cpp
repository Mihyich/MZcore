#include "app_args.h"

bool app::debugging = true;

Window app::game_wnd;
Window app::dbg_wnd;
Console app::cnsl;

Graphics::Vulkan_Renderer app::vk_render;

bool app::use_realtime_settings_change = false;

bool app::game_cycle_work = true;