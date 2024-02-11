#include <Windows.h>
#include <string>
#include "definers.h"
#include "game_window_proc.h"
#include "game_loop.h"
#include "app_args.h"
#include "vulkan_renderer.h"
#include "str_converter.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	BOOL err = EXIT_SUCCESS;
	BOOL rtn = 0;
	std::wstring wstr_tmp;

    app::hInstance = hInstance;

	// улучшение DPI
	if (!SetProcessDPIAware())
	{
		rtn = MessageBox(
			NULL,
			L"Не удалось повысить DPI...\nПродолжить?",
			L"SetProcessDPIAware error",
			MB_YESNO | MB_ICONINFORMATION);

		if (rtn == IDNO)
		{
			err = ERR_SETPROCESSDPIAWARE;
		}
	}

	// создание игрового окна ============================
	if (!err && (err = app::game_wnd.create(
		CS_HREDRAW | CS_VREDRAW,
		&game_window_proc,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		L"GAME_WINDOW_CLASS",
		LoadIcon(NULL, IDI_APPLICATION),
		0,
		L"no money - no funny",
		WS_OVERLAPPEDWINDOW,
		800, 600,
		NULL,
		NULL,
		NULL,
		nCmdShow)) != EXIT_SUCCESS)
	{
		MessageBox(
			NULL,
			L"Не удалось создать игровое окно...\n",
			L"app::game_wnd.create() error",
			MB_OK | MB_ICONINFORMATION);
	}
	// игровое окно создано ============================

	// Показать игровое окно
	if (!err && app::game_wnd.show(nCmdShow) != EXIT_SUCCESS)
	{
		MessageBox(
			NULL,
			L"Не удалось показать игровое окно...\n",
			L"app::game_wnd.show() error",
			MB_OK | MB_ICONINFORMATION);

		err = ERR_WND_SHOW;
	}

	// создание экземпляра вулкана
	if (!err && (err = Graphics::vk_renderer.init(false, false, Graphics::debugging)) != EXIT_SUCCESS)
	{
		wstr_tmp = string_to_wstring(Graphics::vk_renderer.get_error_report());

		MessageBox(
			NULL,
			wstr_tmp.c_str(),
			L"Ошибка Vulkan...\n",
			MB_OK | MB_ICONINFORMATION);
	}

    // некий вывод того чего получилось в vulkan'е
	if (!err)
	{
		Graphics::vk_renderer.output_version();
        Graphics::vk_renderer.output_instance_extensions_preperties();
		Graphics::vk_renderer.output_instance_layers_properties();
		Graphics::vk_renderer.output_physical_devices();
        Graphics::vk_renderer.output_choosen_physical_device();
        Graphics::vk_renderer.output_physical_device_queue_family_properties();
	}

	// Запустить игровой цикл
	if (!err)
	{
		// app::game_wnd.set_pos_center();
		launch_game_loop();
	}

	app::game_wnd.destroy();
	app::cnsl.destroy();
	return err;
}
