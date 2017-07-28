#pragma once
struct simple_renderer
{
	tex2 back_buffer;
	output_merge_stage om;
	viewports vp;

	void clear_back_buffer(const std::array<float, 4>& bc) {
		clear_render_target(om, bc);
	}

	proxy mapping(std::type_index, adapter_interface& ai);

	void init(value_table&);

	void pre_tick(duration da)
	{
		*this << vp;
	}
};