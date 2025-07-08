#include "full_screen_control.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/project_settings.hpp>


void FullScreenControl::_ready() {
	if (!is_inside_tree()) {
		return;
	}

	SceneTree *tree = get_tree();
	
	Window *window = tree->get_root();

	if (window == nullptr) {
		return;
	}

	if (Engine::get_singleton()->is_editor_hint()) {
		int width = ProjectSettings::get_singleton()->get_setting("display/window/size/viewport_width");
		int height = ProjectSettings::get_singleton()->get_setting("display/window/size/viewport_height");
		set_custom_minimum_size(Vector2(width, height));
		return;
	}

	Vector2 aspect_size = window->get_size();
	real_t aspect_ratio = aspect_size.aspect();

	Vector2 desired_res = window->get_content_scale_size();
	real_t desired_aspect = desired_res.aspect();

	Vector2 minimum_size = Vector2(0, 0);

	if (desired_aspect < aspect_ratio) {
		minimum_size.x = desired_res.y * aspect_ratio;
		minimum_size.y = desired_res.y;
	} 
	else {
		minimum_size.x = desired_res.x;
		minimum_size.y = desired_res.x / aspect_ratio;
	}

	set_custom_minimum_size(minimum_size);
}

void FullScreenControl::_bind_methods() {
	
}