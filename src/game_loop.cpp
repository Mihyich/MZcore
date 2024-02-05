#include "game_loop.h"

void launch_game_loop(void)
{
    while (app::game_cycle_work)
    {
        if (app::use_realtime_settings_change)
        {
            app::game_wnd.peek_msgs();
            if (app::debugging) app::dbg_wnd.peek_msgs();
        }
        else
        {
            auto cycle_condition = []() -> bool
            {
                return app::game_cycle_work;
            };

            app::dbg_wnd.get_msgs(cycle_condition);
        }
    }
}
