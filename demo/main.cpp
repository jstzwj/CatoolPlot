#include<iostream>
#include"CatoolPlotForm\win32\Win32Form.h"
#include"CatoolPlotForm\dx11\Dx11Form.h"


int main()
{
	catool::dx11::FormDx11Initial form_initial;
	catool::dx11::FormDx11 form(form_initial);
	catool::win32::FormWin32EventThread event_thread(form);
	event_thread.run();
	event_thread.wait();

	return 0;
}