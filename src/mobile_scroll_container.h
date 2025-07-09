#pragma once

#include <godot_cpp/classes/container.hpp>
#include <godot_cpp/classes/input_event_screen_touch.hpp>
#include <godot_cpp/classes/input_event_screen_drag.hpp>

using namespace godot;

class MobileScrollContainer : public Container {
	GDCLASS(MobileScrollContainer, Container);

public:
	enum Direction {
		VERTICAL,
		HORIZONTAL
	};

	void set_direction(Direction p_direction);
	Direction get_direction() const { return _direction; }

	void _gui_input(const Ref<InputEvent> &p_event) override;
	virtual PackedStringArray _get_configuration_warnings() const override;

protected:
	Control *_scroll_view = nullptr;

	void _notification(int p_what);
	static void _bind_methods();

private:
	Direction _direction = Direction::VERTICAL;

	bool _scroll_started;
	bool _min_drag_distance_reached;
	Vector2 _drag_distance;
	Vector2 _drag_velocity;
	Vector2 _scroll_offset;

	void _handle_screen_touch_start(InputEventScreenTouch *p_touch);
	void _handle_screen_touch_end(InputEventScreenTouch *p_touch);
	void _handle_screen_drag(InputEventScreenDrag *p_drag);

	void _clamp_scroll_view_vertical();
	void _clamp_scroll_view_horizontal();

	void _reposition_children();

};

VARIANT_ENUM_CAST(MobileScrollContainer::Direction);