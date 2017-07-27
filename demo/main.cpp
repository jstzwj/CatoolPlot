#include<iostream>

int main()
{

	catool::context con;

	if (true)
	{
		auto fo = con.create(form<Dx11_form>{});
		fo.lock([](decltype(fo)::type& ui) {
			ui.create(renderer<defer_renderer>{});
			ui.create(plugin<new_plugin>{});
		});
	}

	con.wait_all_form_close();
	return 0;
}