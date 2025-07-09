#include "animated_grid_container.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void AnimatedGridContainer::set_columns(int p_columns) {
	_columns = p_columns;
	queue_sort();
}

void AnimatedGridContainer::set_h_separation(int p_separation) {
	_h_separation = p_separation;
	queue_sort();
}

void AnimatedGridContainer::set_v_separation(int p_separation) {
	_v_separation = p_separation;
	queue_sort();
}

void AnimatedGridContainer::set_order_direction(OrderDirection p_direction) {
	_order_direction = p_direction;
	queue_sort();
}

void AnimatedGridContainer::set_horizontal_alignment(Alignment p_alignment) {
	_horizontal_alignment = p_alignment;
	queue_sort();
}

void AnimatedGridContainer::set_vertical_alignment(Alignment p_alignment) {
	_vertical_alignment = p_alignment;
	queue_sort();
}

void AnimatedGridContainer::set_animate_child_order_disabled(bool p_disabled) {
	_animate_child_order_disabled = p_disabled;
}

void AnimatedGridContainer::set_use_first_row_element_width(bool p_use) {
	_use_first_row_element_width = p_use;
	queue_sort();
}

void AnimatedGridContainer::swap_children(Control *p_child1, Control *p_child2) {
	if (!p_child1 || !p_child2) {
		return;
	}

	int index1 = p_child1->get_index();
	int index2 = p_child2->get_index();

	_update_child_order_disabled = true;

	move_child(p_child1, index2);
	move_child(p_child2, index1);

	_update_child_order_disabled = false;
	queue_sort();
}

TypedArray<Control> AnimatedGridContainer::get_visible_children() {
	TypedArray<Control> visible_children;
	int child_count = get_child_count();

	for (int i = 0; i < child_count; i++) {
		Control *child = Object::cast_to<Control>(get_child(i));
		if (child && child->is_visible_in_tree()) {
			visible_children.push_back(child);
		}
	}

	return visible_children;
}

Vector2 AnimatedGridContainer::get_container_size() {
	int visible_count = get_visible_children_count();
	int rows = Math::ceil(visible_count / float(_columns));

	float height = 0.0f;
	float largest_width = 0.0f;

	for (int row_index = 0; row_index < rows; row_index++) {
		if (row_index != 0) {
			height += _v_separation;
		}

		int row_start_index = row_index * _columns;
		float width = get_row_width(row_start_index);

		if (width > largest_width) {
			largest_width = width;
		}

		height += get_max_height_row(row_start_index);
	}

	return Vector2(largest_width, height);
}

int AnimatedGridContainer::get_visible_children_count() {
	int count = 0;
	int child_count = get_child_count();

	for (int i = 0; i < child_count; i++) {
		Control *child = Object::cast_to<Control>(get_child(i));
		if (child && child->is_visible_in_tree()) {
			count++;
		}
	}

	return count;
}

Control *AnimatedGridContainer::get_visible_child(int p_index) {
	TypedArray<Control> children = get_visible_children();
	int count = children.size();

	if (p_index < 0 || p_index >= count) {
		return nullptr;
	}

	switch (_order_direction) {
		case ORDER_BEGIN:
			return Object::cast_to<Control>(children[p_index]);
		case ORDER_END:
			return Object::cast_to<Control>(children[count - 1 - p_index]);
		default:
			return Object::cast_to<Control>(children[p_index]);
	}
}

int AnimatedGridContainer::convert_index(int p_index) {
	switch (_order_direction) {
		case ORDER_BEGIN:
			return p_index;
		case ORDER_END:
			return get_visible_children_count() - 1 - p_index;
		default:
			return p_index;
	}
}

void AnimatedGridContainer::update_row(int p_row_start_index, float p_row_start_y, float p_size_x) {
	float row_width = get_row_width(p_row_start_index);
	float start_x = get_row_start_x(row_width, p_size_x);
	int visible_children_count = get_visible_children_count();

	for (int child_index = p_row_start_index; child_index < p_row_start_index + _columns; child_index++) {
		if (child_index >= visible_children_count) {
			return;
		}

		if (child_index != p_row_start_index) {
			start_x += _h_separation;
		}

		Control *child = get_visible_child(child_index);
		if (!child) {
			continue;
		}

		Vector2 position = Vector2(start_x, p_row_start_y);

		if (_first_sort_done && !_animate_child_order_disabled) {
			animate_position_change(child, position);
		} 
		else {
			child->set_position(position);
		}

		start_x += child->get_size().x;
	}
}

float AnimatedGridContainer::get_max_height_row(int p_row_start_index) {
	float max_height = 0.0f;
	int visible_children_count = get_visible_children_count();

	for (int i = p_row_start_index; i < p_row_start_index + _columns; i++) {
		if (i >= visible_children_count) {
			return max_height;
		}

		Control *child = get_visible_child(i);
		if (!child)
			continue;

		if (child->get_size().y > max_height) {
			max_height = child->get_size().y;
		}
	}

	return max_height;
}

float AnimatedGridContainer::get_row_width(int p_row_start_index) {
	float row_width = 0.0f;
	float first_element_width = 0.0f;
	int children_count = get_visible_children_count();

	for (int i = p_row_start_index; i < p_row_start_index + _columns; i++) {
		if (i >= children_count) {
			if (_use_first_row_element_width && first_element_width > 0) {
				row_width += first_element_width + _h_separation;
			}
			continue;
		}

		Control *child = get_visible_child(i);
		if (!child)
			continue;

		if (i == p_row_start_index) {
			first_element_width = child->get_size().x;
		}

		row_width += child->get_size().x;

		if (i != p_row_start_index) {
			row_width += _h_separation;
		}
	}

	return row_width;
}

float AnimatedGridContainer::get_row_start_x(float p_row_width, float p_size_x) {
	switch (_horizontal_alignment) {
		case ALIGN_START:
			return 0.0f;
		case ALIGN_CENTER:
			return (p_size_x - p_row_width) / 2.0f;
		case ALIGN_END:
			return p_size_x - p_row_width;
		default:
			return 0.0f;
	}
}

float AnimatedGridContainer::get_start_y(float p_container_size_y, float p_custom_minimum_size_y) {
	switch (_vertical_alignment) {
		case ALIGN_START:
			return 0.0f;
		case ALIGN_CENTER:
			return (p_custom_minimum_size_y - p_container_size_y) / 2.0f;
		case ALIGN_END:
			return p_custom_minimum_size_y - p_container_size_y;
		default:
			return 0.0f;
	}
}

void AnimatedGridContainer::animate_position_change(Control *p_child, Vector2 p_new_position) {
	if (!p_child) {
		return;
	}

	Ref<Tween> tween = p_child->create_tween();
	tween->set_ease(Tween::EASE_IN_OUT);
	tween->set_trans(Tween::TRANS_CUBIC);
	tween->tween_property(p_child, "position", p_new_position, 0.4f);

	_active_tweens.push_back(tween);
}

void AnimatedGridContainer::cancel_all_animations() {
	for (auto &tween : _active_tweens) {
		if (tween.is_valid()) {
			tween->kill();
		}
	}
	
	_active_tweens.clear();
}

AnimatedGridContainer::AnimatedGridContainer() {
	_columns = 1;
	_h_separation = 0;
	_v_separation = 0;
	_order_direction = ORDER_BEGIN;
	_horizontal_alignment = ALIGN_CENTER;
	_vertical_alignment = ALIGN_CENTER;
}

AnimatedGridContainer::~AnimatedGridContainer() {
	cancel_all_animations();
}

void AnimatedGridContainer::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_SORT_CHILDREN: {
			if (_update_child_order_disabled) {
				break;
			}

			emit_signal("on_child_order_changed");

			cancel_all_animations();

			_minimum_size = get_container_size();
			set_custom_minimum_size(_minimum_size);

			int visible_count = get_visible_children_count();
			int rows = Math::ceil(visible_count / float(_columns));
			float start_y = get_start_y(_minimum_size.y, get_size().y);

			for (int i = 0; i < rows; i++) {
				if (i != 0) {
					start_y += _v_separation;
				}

				int row_start_index = i * _columns;
				update_row(row_start_index, start_y, get_size().x);

				float max_height = get_max_height_row(row_start_index);
				start_y += max_height;
			}

			_first_sort_done = true;
		} break;
	}
}

Vector2 AnimatedGridContainer::_get_minimum_size() const {
	return _minimum_size;
}

void AnimatedGridContainer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_columns", "columns"), &AnimatedGridContainer::set_columns);
	ClassDB::bind_method(D_METHOD("get_columns"), &AnimatedGridContainer::get_columns);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "columns"), "set_columns", "get_columns");

	ClassDB::bind_method(D_METHOD("set_h_separation", "separation"), &AnimatedGridContainer::set_h_separation);
	ClassDB::bind_method(D_METHOD("get_h_separation"), &AnimatedGridContainer::get_h_separation);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "h_separation"), "set_h_separation", "get_h_separation");

	ClassDB::bind_method(D_METHOD("set_v_separation", "separation"), &AnimatedGridContainer::set_v_separation);
	ClassDB::bind_method(D_METHOD("get_v_separation"), &AnimatedGridContainer::get_v_separation);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "v_separation"), "set_v_separation", "get_v_separation");

	ClassDB::bind_method(D_METHOD("set_order_direction", "direction"), &AnimatedGridContainer::set_order_direction);
	ClassDB::bind_method(D_METHOD("get_order_direction"), &AnimatedGridContainer::get_order_direction);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "order_direction", PROPERTY_HINT_ENUM, "Begin,End"), "set_order_direction", "get_order_direction");

	ClassDB::bind_method(D_METHOD("set_horizontal_alignment", "alignment"), &AnimatedGridContainer::set_horizontal_alignment);
	ClassDB::bind_method(D_METHOD("get_horizontal_alignment"), &AnimatedGridContainer::get_horizontal_alignment);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "horizontal_alignment", PROPERTY_HINT_ENUM, "Start,Center,End"), "set_horizontal_alignment", "get_horizontal_alignment");

	ClassDB::bind_method(D_METHOD("set_vertical_alignment", "alignment"), &AnimatedGridContainer::set_vertical_alignment);
	ClassDB::bind_method(D_METHOD("get_vertical_alignment"), &AnimatedGridContainer::get_vertical_alignment);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "vertical_alignment", PROPERTY_HINT_ENUM, "Start,Center,End"), "set_vertical_alignment", "get_vertical_alignment");

	ClassDB::bind_method(D_METHOD("set_animate_child_order_disabled", "disabled"), &AnimatedGridContainer::set_animate_child_order_disabled);
	ClassDB::bind_method(D_METHOD("get_animate_child_order_disabled"), &AnimatedGridContainer::get_animate_child_order_disabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "animate_child_order_disabled"), "set_animate_child_order_disabled", "get_animate_child_order_disabled");

	ClassDB::bind_method(D_METHOD("set_use_first_row_element_width", "use"), &AnimatedGridContainer::set_use_first_row_element_width);
	ClassDB::bind_method(D_METHOD("get_use_first_row_element_width"), &AnimatedGridContainer::get_use_first_row_element_width);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_first_row_element_width"), "set_use_first_row_element_width", "get_use_first_row_element_width");

	ClassDB::bind_method(D_METHOD("swap_children", "child1", "child2"), &AnimatedGridContainer::swap_children);
	ClassDB::bind_method(D_METHOD("get_visible_children"), &AnimatedGridContainer::get_visible_children);

	ADD_SIGNAL(MethodInfo("on_child_order_changed"));
}