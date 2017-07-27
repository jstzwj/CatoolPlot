#include"Win32Form.h"

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
		FormWin32::~FormWin32()
		{
		}
	}
}


