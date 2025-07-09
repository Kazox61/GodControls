#include "mobile_scroll_container.h"

#include <godot_cpp/classes/property_tweener.hpp>

void MobileScrollContainer::set_direction(Direction p_direction) {
	_direction = p_direction;
}
void MobileScrollContainer::_handle_screen_touch_start(InputEventScreenTouch *p_touch) {
	_scroll_started = false;
	_min_drag_distance_reached = false;
	_drag_distance = Vector2();
}

void MobileScrollContainer::_handle_screen_touch_end(InputEventScreenTouch *p_touch) {
	if (!_scroll_started) {
		return;
	}

	switch (_direction) {
		case Direction::VERTICAL:
			_clamp_scroll_view_vertical();
			break;
		case Direction::HORIZONTAL:
			_clamp_scroll_view_horizontal();
			break;
	}
}

void MobileScrollContainer::_handle_screen_drag(InputEventScreenDrag *p_drag) {
	_drag_distance += p_drag->get_relative();

	if (_drag_distance.length() >= 25.0f && !_min_drag_distance_reached) {
		_min_drag_distance_reached = true;
		
		switch (_direction) {
			case Direction::HORIZONTAL:
				if (Math::abs(_drag_distance.y) > Math::abs(_drag_distance.x)) {
					return;
				}
				break;
			case Direction::VERTICAL:
				if (Math::abs(_drag_distance.x) > Math::abs(_drag_distance.y)) {
					return;
				}
				break;
		}

		_scroll_started = true;
		emit_signal("scroll_started");
	}

	if (!_scroll_started) {
		return;
	}

	_drag_velocity = p_drag->get_velocity();

	switch (_direction) {
		case Direction::VERTICAL:
			_scroll_offset.y += p_drag->get_relative().y;
			break;
		case Direction::HORIZONTAL:
			_scroll_offset.x += p_drag->get_relative().x;
			break;
	}

	queue_sort();
 }

void MobileScrollContainer::_clamp_scroll_view_vertical() {
	Vector2 scroll_view_rect_size = _scroll_view->get_rect().size;
	Vector2 container_rect_size = get_rect().size;

	float max_scroll_y = - scroll_view_rect_size.y + container_rect_size.y;

	max_scroll_y = MIN(0.0f, max_scroll_y);

	float tween_position_y;

	if (_scroll_view->get_position().y > 0) {
		tween_position_y = 0.0f;
	}
	else if (_scroll_view->get_position().y < max_scroll_y) {
		tween_position_y = max_scroll_y;
	}
	else {
		tween_position_y = _scroll_view->get_position().y + _drag_velocity.y * 0.25f;
		tween_position_y = Math::clamp(tween_position_y, max_scroll_y, 0.0f);
	}

	_scroll_offset.y = tween_position_y;

	Ref<Tween> tween = create_tween()
			->set_ease(Tween::EASE_OUT)
			->set_trans(Tween::TRANS_CUBIC);

		tween->tween_property(_scroll_view, "position:y", tween_position_y, 0.25f);
		tween->play();

}

void MobileScrollContainer::_clamp_scroll_view_horizontal() {
	Vector2 scroll_view_rect_size = _scroll_view->get_rect().size;
	Vector2 container_rect_size = get_rect().size;

	float max_scroll_x = - scroll_view_rect_size.x + container_rect_size.x;

	max_scroll_x = MIN(0.0f, max_scroll_x);

	float tween_position_x;

	if (_scroll_view->get_position().x > 0) {
		tween_position_x = 0.0f;
	}
	else if (_scroll_view->get_position().x < max_scroll_x) {
		tween_position_x = max_scroll_x;
	}
	else {
		tween_position_x = _scroll_view->get_position().x + _drag_velocity.x * 0.25f;
		tween_position_x = Math::clamp(tween_position_x, max_scroll_x, 0.0f);
	}

	_scroll_offset.x = tween_position_x;

	Ref<Tween> tween = create_tween()
			->set_ease(Tween::EASE_OUT)
			->set_trans(Tween::TRANS_CUBIC);

		tween->tween_property(_scroll_view, "position:x", tween_position_x, 0.25f);
		tween->play();
}

void MobileScrollContainer::_reposition_children() {
	if (!_scroll_view || !_scroll_view->is_visible_in_tree()) {
		return;
	}

	Size2 container_size = get_size();
	Size2 content_size = _scroll_view->get_combined_minimum_size();

	Rect2 rect;
	rect.position = _scroll_offset;
	rect.size = content_size;

	if (_scroll_view->get_h_size_flags().has_flag(SIZE_EXPAND)) {
		rect.size.x = MAX(rect.size.x, container_size.x);
	}
	if (_scroll_view->get_v_size_flags().has_flag(SIZE_EXPAND)) {
		rect.size.y = MAX(rect.size.y, container_size.y);
	}

	fit_child_in_rect(_scroll_view, rect);
}

void MobileScrollContainer::_gui_input(const Ref<InputEvent> &p_event) {
	if (p_event.is_null()) {
		return;
	}

	InputEventScreenTouch *touch = Object::cast_to<InputEventScreenTouch>(*p_event);
	if (touch) {
		if (touch->is_pressed()) {
			_handle_screen_touch_start(touch);
		} 
		else {
			_handle_screen_touch_end(touch);
		}

		return;
	}

	InputEventScreenDrag *drag = Object::cast_to<InputEventScreenDrag>(*p_event);
	if (drag) {
		_handle_screen_drag(drag);
		return;
	}
}

PackedStringArray MobileScrollContainer::_get_configuration_warnings() const {
	PackedStringArray warnings = Container::_get_configuration_warnings();

	int found = 0;

	for (int i = 0; i < get_child_count(); i++) {
		Node *child = get_child(i);
		Control *c = Object::cast_to<Control>(child);
		if (!c || c->is_set_as_top_level() || !c->is_visible()) {
			c = nullptr;
		}

		found++;
	}

	if (found != 1) {
		warnings.push_back("MobileScrollContainer is intended to work with a single child control.\nUse a container as child (VBox, HBox, etc.), or a Control and set the custom minimum size manually.");
	}

	return warnings;
}

void MobileScrollContainer::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_scroll_view = Object::cast_to<Control>(get_child(0));
			_reposition_children();
		} break;
		case NOTIFICATION_SORT_CHILDREN: {
			_reposition_children();
		} break;
	}
}

void MobileScrollContainer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_direction", "direction"), &MobileScrollContainer::set_direction);
	ClassDB::bind_method(D_METHOD("get_direction"), &MobileScrollContainer::get_direction);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "direction", PROPERTY_HINT_ENUM, "Vertical,Horizontal"), "set_direction", "get_direction");


	ADD_SIGNAL(MethodInfo("scroll_start"));
}