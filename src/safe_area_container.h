#pragma once

#include <godot_cpp/classes/margin_container.hpp>

using namespace godot;

class SafeAreaContainer : public MarginContainer {
	GDCLASS(SafeAreaContainer, MarginContainer);

protected:
	static void _bind_methods();
	void _notification(int p_what);

private:
	void configure_full_screen_layout();
	void apply_safe_area_margins();
};