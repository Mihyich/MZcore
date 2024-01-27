#include <Windows.h>
#include <string>
#include "definers.h"
#include "vulkan_renderer.h"
#include "dbg_window_proc.h"
#include "game_window_proc.h"
#include "game_loop.h"
#include "app_args.h"
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

	// окно отладки
	if (!err && app::debugging)
	{
		// Создание консоли
		if (!app::cnsl.create())
		{
			MessageBox(
				NULL,
				L"Не удалось создать консоль...\n",
				L"app::cnsl.create() error",
				MB_OK | MB_ICONINFORMATION);

			err = ERR_CNSL_CREATE;
		}

		// app::cnsl.set_pos(0, 0);
		// app::cnsl.set_size(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

		// создание окна отладки ===========================
		if (!err && (err = app::dbg_wnd.create(
			CS_HREDRAW | CS_VREDRAW,
			&debug_window_proc,
			hInstance,
			LoadIcon(NULL, IDI_APPLICATION),
			LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)(COLOR_WINDOW + 2),
			NULL,
			L"GDB_WINDOW_CLASS",
			LoadIcon(NULL, IDI_APPLICATION),
			0,
			L"MZcore editor",
			WS_OVERLAPPEDWINDOW,
			800, 600,
			NULL,
			NULL,
			NULL,
			nCmdShow)) != EXIT_SUCCESS)
		{
			MessageBox(
				NULL,
				L"Не удалось создать окно отладки...\n",
				L"app::game_wnd.create() error",
				MB_OK | MB_ICONINFORMATION);
		}

		// окно отладки создано  ===========================
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
		app::debugging ? WS_CHILD | WS_VISIBLE : WS_OVERLAPPEDWINDOW,
		400, 300,
		app::debugging ? app::dbg_wnd.get_hWnd() : NULL,
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
	
	// Если нужно то показать окно отладки
	if (app::debugging && !err && app::dbg_wnd.show(nCmdShow) != EXIT_SUCCESS)
	{
		MessageBox(
			NULL,
			L"Не удалось показать окно отладки...\n",
			L"app::dbg_wnd.show() error",
			MB_OK | MB_ICONINFORMATION);

		err = ERR_WND_SHOW;

		app::dbg_wnd.set_focus();
	}

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
	if (!err && (err = app::vk_render.init()) != EXIT_SUCCESS)
	{
		wstr_tmp = string_to_wstring(app::vk_render.get_error_report());

		MessageBox(
			NULL,
			wstr_tmp.c_str(),
			L"Ошибка Vulkan...\n",
			MB_OK | MB_ICONINFORMATION);
	}

	// Поиск физических устройств
	if (!err && (err = app::vk_render.enumerate_physical_devices()) != EXIT_SUCCESS)
	{
		wstr_tmp = string_to_wstring(app::vk_render.get_error_report());

		MessageBox(
			NULL,
			wstr_tmp.c_str(),
			L"Ошибка Vulkan...\n",
			MB_OK | MB_ICONINFORMATION);
	}

	// поиск поддерживаемых расширений
	if (!err && (err = app::vk_render.enumerate_instance_extensions_properties()) != EXIT_SUCCESS)
	{
		wstr_tmp = string_to_wstring(app::vk_render.get_error_report());

		MessageBox(
			NULL,
			wstr_tmp.c_str(),
			L"Ошибка Vulkan...\n",
			MB_OK | MB_ICONINFORMATION);
	}

	// поиск поддерживаемых слоев
	if (!err && (err = app::vk_render.enumerate_instance_layers_properties()) != EXIT_SUCCESS)
	{
		wstr_tmp = string_to_wstring(app::vk_render.get_error_report());

		MessageBox(
			NULL,
			wstr_tmp.c_str(),
			L"Ошибка Vulkan...\n",
			MB_OK | MB_ICONINFORMATION);
	}

	if (!err)
	{
		app::vk_render.output_version();
		app::vk_render.output_instance_layers_properties();
		app::vk_render.output_instance_extensions_preperties();
		app::vk_render.output_physical_devices();
	}

	// Запустить игровой цикл
	if (!err)
	{
		// app::game_wnd.set_pos_center();
		launch_game_loop();
	}

	app::game_wnd.destoy();
	if (app::debugging) app::dbg_wnd.destoy();
	app::cnsl.destroy();
	return err;
}