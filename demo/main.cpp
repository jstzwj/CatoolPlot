#include<iostream>
#include"CatoolPlotForm\win32\Win32Form.h"



int main()
{
	catool::win32::FormWin32EventThread event_thread;
	catool::win32::FormWin32 form;
	catool::win32::FormWin32Initial form_initial;
	event_thread.create_window(form_initial,&form);
	event_thread.destory_window(form.raw_handle);
	
	return 0;
}