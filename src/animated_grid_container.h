#pragma once

#include <godot_cpp/classes/container.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <memory>
#include <vector>

using namespace godot;

class AnimatedGridContainer : public Container {
	GDCLASS(AnimatedGridContainer, Container)

public:
	enum OrderDirection {
		ORDER_BEGIN,
		ORDER_END
	};

	enum Alignment {
		ALIGN_START,
		ALIGN_CENTER,
		ALIGN_END
	};

	AnimatedGridContainer();
	~AnimatedGridContainer();

	void set_columns(int p_columns);
	int get_columns() const { return _columns; }

	void set_h_separation(int p_separation);
	int get_h_separation() const { return _h_separation; }

	void set_v_separation(int p_separation);
	int get_v_separation() const { return _v_separation; }

	void set_order_direction(OrderDirection p_direction);
	OrderDirection get_order_direction() const { return _order_direction; }

	void set_horizontal_alignment(Alignment p_alignment);
	Alignment get_horizontal_alignment() const { return _horizontal_alignment; }

	void set_vertical_alignment(Alignment p_alignment);
	Alignment get_vertical_alignment() const { return _vertical_alignment; }

	void set_animate_child_order_disabled(bool p_disabled);
	bool get_animate_child_order_disabled() const { return _animate_child_order_disabled; }

	void set_use_first_row_element_width(bool p_use);
	bool get_use_first_row_element_width() const { return _use_first_row_element_width; }

	Vector2 _get_minimum_size() const override;

	void swap_children(Control *p_child1, Control *p_child2);
	TypedArray<Control> get_visible_children();

protected:
	void _notification(int p_what);
	static void _bind_methods();

private:
	int _columns = 1;
	int _h_separation = 0;
	int _v_separation = 0;
	OrderDirection _order_direction = ORDER_BEGIN;
	Alignment _horizontal_alignment = ALIGN_CENTER;
	Alignment _vertical_alignment = ALIGN_CENTER;
	bool _first_sort_done  = false;
	bool _animate_child_order_disabled;
	bool _use_first_row_element_width = false;
	bool _update_child_order_disabled = false;
	Vector2 _minimum_size;

	std::vector<Ref<Tween>> _active_tweens;

	Vector2 get_container_size();
	int get_visible_children_count();
	Control *get_visible_child(int p_index);
	int convert_index(int p_index);

	void update_row(int p_row_start_index, float p_row_start_y, float p_size_x);
	float get_max_height_row(int p_row_start_index);
	float get_row_width(int p_row_start_index);
	float get_row_start_x(float p_row_width, float p_size_x);
	float get_start_y(float p_container_size_y, float p_custom_minimum_size_y);

	void animate_position_change(Control *p_child, Vector2 p_new_position);
	void cancel_all_animations();
};

VARIANT_ENUM_CAST(AnimatedGridContainer::OrderDirection);
VARIANT_ENUM_CAST(AnimatedGridContainer::Alignment);