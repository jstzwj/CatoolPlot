#include"Win32Form.h"
#include<stdexcept>

namespace catool
{
	namespace win32
	{
		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wPararm, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
				break;
			}

			return DefWindowProc(hwnd, msg, wPararm, lParam);
		}
		FormWin32::FormWin32(const FormWin32Initial& init)
			:form_win32_initial(init)
		{
		}
		FormWin32::~FormWin32()
		{
			//destory_window();
		}
		HRESULT FormWin32::create_window()
		{
			HINSTANCE g_instance = GetModuleHandle(NULL);//得到程序实例句柄
			LPCWSTR g_name = NTS("Hello Win");

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
				MessageBox(NULL, NTS("注册窗口失败！"), NTS("Hello Win"), 0);
				return false;
			}

			//创建窗口
			raw_handle = CreateWindowEx(WS_EX_APPWINDOW, g_name, g_name, WS_OVERLAPPEDWINDOW, 300, 200,
				800, 600, NULL, NULL, g_instance, NULL);

			//显示窗口设置其为焦点
			ShowWindow(raw_handle, SW_SHOW);
			SetForegroundWindow(raw_handle);
			SetFocus(raw_handle);

			//隐藏鼠标
			//SetCursor(false);
			//HRESULT rst = create_window();

			//if (!SUCCEEDED(rst))
			//{
			//	//HRESULT_to_string(rst);
			//	throw std::runtime_error("error:");
			//}
			return S_OK;
		}
		void FormWin32::destory_window()
		{
			SetWindowLongPtrW(raw_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
			DestroyWindow(raw_handle);
			raw_handle = nullptr;
		}
		void FormWin32::message_process()
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
					std::this_thread::yield();
				}
			}
		}
	}
}


