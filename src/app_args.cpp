#include "app_args.h"

namespace app
{
    bool debugging = true;

    HINSTANCE hInstance = NULL;
    Window game_wnd;
    Console cnsl;

    bool use_realtime_settings_change = false;
    bool game_cycle_work = true;
}
