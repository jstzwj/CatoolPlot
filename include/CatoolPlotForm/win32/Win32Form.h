#pragma once
#include <Windows.h>
#include<atomic>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<vector>
#include"Win32Definition.h"

namespace catool
{
	namespace win32
	{


		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wPararm, LPARAM lParam);

		struct FormWin32Style
		{
			DWORD window_style = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
			DWORD ex_window_style = WS_EX_CLIENTEDGE;
		};
		
		struct FormWin32Initial
		{
			std::u16string title = u"PO default title :>";
			int shitf_x = (GetSystemMetrics(SM_CXSCREEN) - 1024) / 2;
			int shift_y = (GetSystemMetrics(SM_CYSCREEN) - 768) / 2;
			int width = 1024;
			int height = 768;
			FormWin32Style style = FormWin32Style();
		};
		struct FormWin32
		{
			HWND raw_handle;
			std::atomic_bool quit;
			~FormWin32();
		};
		
		class FormWin32EventThread
		{
		private:

		public:
			void main_thread()
			{
				MSG msg;
				ZeroMemory(&msg, sizeof(MSG));
				while (msg.message != WM_QUIT)
				{
					if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					else
					{
						//DoSomething
					}
				}
			}
			HRESULT create_window(const FormWin32Initial& wi,FormWin32* ptr)
			{
				HINSTANCE g_instance = GetModuleHandle(NULL);//得到程序实例句柄
				LPCWSTR g_name = T("Hello Win");

				WNDCLASSEX wnd;
				wnd.cbClsExtra = 0;
				wnd.cbSize = sizeof(WNDCLASSEX);
				wnd.cbWndExtra = 0;
				wnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
				wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
				wnd.hIcon = LoadIcon(NULL, IDI_WINLOGO);
				wnd.hIconSm = wnd.hIcon;
				wnd.hInstance = g_instance;
				wnd.lpfnWndProc = WndProc;
				wnd.lpszClassName = g_name;
				wnd.lpszMenuName = g_name;
				wnd.style = CS_HREDRAW | CS_VREDRAW;
				//注册窗口类
				if (!RegisterClassEx(&wnd))
				{
					MessageBox(NULL, T("注册窗口失败！"), T("Hello Win"), 0);
					return false;
				}

				//创建窗口
				HWND g_hwnd = CreateWindowEx(WS_EX_APPWINDOW, g_name, g_name, WS_OVERLAPPEDWINDOW, 300, 200,
					800, 600, NULL, NULL, g_instance, NULL);

				//显示窗口设置其为焦点
				ShowWindow(g_hwnd, SW_SHOW);
				SetForegroundWindow(g_hwnd);
				SetFocus(g_hwnd);

				//隐藏鼠标
				//SetCursor(false);

				main_thread();

				return true;
			}
			void destory_window(HWND& handle)
			{
				SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
				DestroyWindow(handle);
				handle = nullptr;
			}


		};
		
	}
}