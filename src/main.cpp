#include <string>
#include "definers.h"
#include "vulkan_renderer.h"
// #include "dbg_window_proc.h"
// #include "game_window_proc.h"
// #include "game_loop.h"
#include "app_args.h"
#include "str_converter.h"

int main(void)
{
	int err = EXIT_SUCCESS;
	int rtn = 0;

	// создание игрового окна ============================
	if ((err = app::game_wnd.create(800, 600, "MZcore") != EXIT_SUCCESS))
	{
		printf("Ошибка создания окна");
	}
	// игровое окно создано ==============================

	// создание экземпляра вулкана
	if (!err && (err = app::vk_renderer.init(false, false, app::vk_debug)) != EXIT_SUCCESS)
		printf("Vulkan error:\n%s\n", app::vk_renderer.get_error_report()->data());

    // некий вывод того чего получилось в vulkane
	if (!err)
	{
		app::vk_renderer.output_version();
        app::vk_renderer.output_instance_extensions_preperties();
		app::vk_renderer.output_instance_layers_properties();
		app::vk_renderer.output_physical_devices();
	}

	// Запустить игровой цикл
	if (!err)
	{
		// app::game_wnd.set_pos_center();
		app::game_wnd.event_loop();
	}

	app::game_wnd.destroy();
	return err;
}