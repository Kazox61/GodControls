#include "safe_area_expand.h"

#include <godot_cpp/classes/display_server.hpp>

void SafeAreaExpand::set_expand_position(ExpandPosition p_expand_position) {
	if (_expand_position == p_expand_position) {
		return;
	}

	_expand_position = p_expand_position;

	update_minimum_size();
}

Vector2 SafeAreaExpand::_get_minimum_size() const {
	Rect2i safe_area = DisplayServer::get_singleton()->get_display_safe_area();
	Vector2i screen_size = DisplayServer::get_singleton()->screen_get_size();

	Vector2 size;

	switch (_expand_position) {
		case ExpandPosition::TOP:
			size = Vector2(0, safe_area.position.y);
			break;

		case ExpandPosition::BOTTOM:
			size = Vector2(0, screen_size.y - (safe_area.position.y + safe_area.size.y));
			break;

		case ExpandPosition::LEFT:
			size = Vector2(safe_area.position.x, 0);
			break;

		case ExpandPosition::RIGHT:
			size = Vector2(screen_size.x - (safe_area.position.x + safe_area.size.x), 0);
			break;

		default:
			break;
	}

	return size;
}

void SafeAreaExpand::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_expand_position", "expand_position"), &SafeAreaExpand::set_expand_position);
	ClassDB::bind_method(D_METHOD("get_expand_position"), &SafeAreaExpand::get_expand_position);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "expand_position", PROPERTY_HINT_ENUM, "Left,Top,Right,Bottom"), "set_expand_position", "get_expand_position");
}