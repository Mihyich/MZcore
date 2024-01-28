#include "app_args.h"

namespace app
{
    bool debugging = true;

    Window game_wnd;
    Window dbg_wnd;
    Console cnsl;

    Graphics::Vulkan_Renderer vk_renderer;

    bool use_realtime_settings_change = false;

    bool game_cycle_work = true;
}