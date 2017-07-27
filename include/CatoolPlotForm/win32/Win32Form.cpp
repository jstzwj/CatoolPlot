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
		{
			HRESULT rst=Singleton<FormWin32EventThread>::getInstance().create_window(init, this);
			
			if (!SUCCEEDED(rst))
			{
				//HRESULT_to_string(rst);
				throw std::runtime_error("error:");
			}
		}
		FormWin32::~FormWin32()
		{
			Singleton<FormWin32EventThread>::getInstance().destory_window(raw_handle);
		}
	}
}


