#include "full_screen_control.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/project_settings.hpp>

Vector2 FullScreenControl::_get_minimum_size() const {
	if (!is_inside_tree()) {
		return Vector2();
	}

	SceneTree *tree = get_tree();
	
	Window *window = tree->get_root();

	if (!window || Engine::get_singleton()->is_editor_hint()) {
		int width = ProjectSettings::get_singleton()->get_setting("display/window/size/viewport_width");
		int height = ProjectSettings::get_singleton()->get_setting("display/window/size/viewport_height");
		return Vector2(width, height);
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

	return minimum_size;
}

void FullScreenControl::_notification(int p_what) {
	if (p_what == NOTIFICATION_READY) {
		update_minimum_size();
	}
}

void FullScreenControl::_bind_methods() {
	
}