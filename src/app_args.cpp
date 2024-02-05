#include "app_args.h"

namespace app
{
    bool vk_debug = true;
    bool debugging = true;

    XWindow game_wnd;
    XWindow dbg_wnd;

    Graphics::Vulkan_Renderer vk_renderer;

    bool use_realtime_settings_change = false;

    bool game_cycle_work = true;
}