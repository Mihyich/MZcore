#include "game_loop.h"

void launch_game_loop(void)
{
    while (app::game_cycle_work)
    {
        app::game_wnd.peek_msgs();
    }
}
