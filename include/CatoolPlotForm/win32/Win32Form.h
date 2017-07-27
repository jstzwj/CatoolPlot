#pragma once
#include <Windows.h>
#include<atomic>
#include<vector>
struct FormWin32
{
	HWND raw_handle;
	std::atomic_bool quit;
	using tank = std::vector<event>;
	Tool::scope_lock<tank> capture_event_tank;
	tank event_tank;
	FormWin32(const win32_initial& = win32_initial{});
	~FormWin32();
	value_table mapping();
	bool available() const { return !quit; }
	Respond handle_event(event& e);
	void pre_tick(duration da);
};