#include "safe_area_container.h"

#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/os.hpp>

void SafeAreaContainer::configure_full_screen_layout() {
	call("_set_layout_mode", 1);
	set_anchors_preset(Control::PRESET_FULL_RECT);
	set_offset(Side::SIDE_LEFT, 0.0f);
	set_offset(Side::SIDE_TOP, 0.0f);
	set_offset(Side::SIDE_RIGHT, 0.0f);
	set_offset(Side::SIDE_BOTTOM, 0.0f);
}

void SafeAreaContainer::apply_safe_area_margins() {
	if (!OS::get_singleton()->has_feature("mobile")) {
		return;
	}

	Rect2i safe_area = DisplayServer::get_singleton()->get_display_safe_area();
	Vector2i screen_size = DisplayServer::get_singleton()->screen_get_size();

	add_theme_constant_override("margin_top", safe_area.get_position().y);
	add_theme_constant_override("margin_left", safe_area.get_position().x);
	add_theme_constant_override("margin_bottom", screen_size.y - safe_area.get_end().y);
	add_theme_constant_override("margin_right", screen_size.x - safe_area.get_end().x);
}

void SafeAreaContainer::_enter_tree() {
	call_deferred("configure_full_screen_layout");
}

void SafeAreaContainer::_ready() {
	apply_safe_area_margins();
}

void SafeAreaContainer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("configure_full_screen_layout"), &SafeAreaContainer::configure_full_screen_layout);
}