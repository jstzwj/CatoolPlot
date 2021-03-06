#pragma once
#include <Windows.h>
#include<atomic>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<functional>
#include<future>
#include<vector>
#include"Win32Definition.h"
#include"CatoolPlotTool\Singleton.h"

namespace catool
{
	namespace win32
	{


		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wPararm, LPARAM lParam);
		class FormWin32EventThread;
		struct FormWin32;

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
			FormWin32Initial form_win32_initial;
			std::atomic_bool quit;

			FormWin32(const FormWin32Initial& init = FormWin32Initial());
			~FormWin32();
			virtual HRESULT create_window();
			virtual void destory_window();
			virtual void message_process();
		};


		
		class FormWin32EventThread
		{
		private:
			std::thread event_thread;
			std::function<void(void)> delegate_function;
			FormWin32& form;
		public:

			FormWin32EventThread(FormWin32& form)
				:form(form){}
			HRESULT run()
			{
				std::promise<HRESULT> pro;
				std::future<HRESULT> fur = pro.get_future();
				event_thread = std::thread([&] {
					pro.set_value(form.create_window());
					form.message_process();
					form.destory_window();
				});

				fur.wait();
				return fur.get();
			}
			void wait()
			{
				event_thread.join();
			}
		};

		
		
	}
}