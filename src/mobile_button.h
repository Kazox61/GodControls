#pragma once

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_screen_touch.hpp>
#include <godot_cpp/classes/input_event_screen_drag.hpp>
#include <godot_cpp/classes/tween.hpp>

#include <godot_cpp/templates/hash_set.hpp>

using namespace godot;

// Forward declaration
class MobileButtonGroup;

class MobileButton : public Control {
	GDCLASS(MobileButton, Control);

public:
	enum PivotPosition {
		START = 0,
		CENTER = 1,
		END = 2,
	};

	void set_touch_disabled(bool p_disabled);
	bool get_touch_disabled() const;

	void set_toggle_mode(bool p_toggle_mode);
	bool get_toggle_mode() const;

	void set_button_pressed(bool p_pressed);
	bool get_button_pressed() const;

	void set_button_group(const Ref<MobileButtonGroup> &p_button_group);
	Ref<MobileButtonGroup> get_button_group() const;

	void set_long_press_enabled(bool p_enabled);
	bool get_long_press_enabled() const;

	void set_long_press_activation_time(float p_time);
	float get_long_press_activation_time() const;

	void set_h_pivot_position(PivotPosition p_position);
	PivotPosition get_h_pivot_position() const;

	void set_v_pivot_position(PivotPosition p_position);
	PivotPosition get_v_pivot_position() const;

	void set_animated(bool p_animated);
	bool get_animated() const;

	void set_animation_duration(float p_duration);
	float get_animation_duration() const;

	void set_button_down_scale(Vector2 p_scale);
	Vector2 get_button_down_scale() const;

	void set_button_up_scale(Vector2 p_scale);
	Vector2 get_button_up_scale() const;

	void _gui_input(const Ref<InputEvent> &p_event);

	void _process(double p_delta);

	MobileButton();
	~MobileButton();

protected:
	static void _bind_methods();
	void _notification(int p_what);

private:
	bool touch_disabled = false;

	bool is_pressing = false;
	float touch_duration = 0.0f;
	float drag_distance = 0.0f;
	bool is_canceled = false;

	bool toggle_mode = false;
	bool button_pressed = false;
	Ref<MobileButtonGroup> button_group;

	bool long_press_enabled = false;
	bool long_pressed = false;
	float long_press_activation_time = 0.3f;

	bool animated = true;
	float animation_duration = 0.2f;
	Vector2 button_down_scale = Vector2(0.9, 0.9);
	Vector2 button_up_scale = Vector2(1.05, 1.05);

	PivotPosition h_pivot_position = CENTER;
	PivotPosition v_pivot_position = CENTER;

	Ref<Tween> tween;

	void set_pivot();

	void unpress_group();

	void play_shrink_animation();

	void play_grow_animation();

	void handle_screen_touch_start(const Ref<InputEventScreenTouch> &p_touch);
	void handle_screen_touch_end(const Ref<InputEventScreenTouch> &p_touch);
	void handle_screen_drag(const Ref<InputEventScreenDrag> &p_drag);
};


VARIANT_ENUM_CAST(MobileButton::PivotPosition);

class MobileButtonGroup : public Resource {
	GDCLASS(MobileButtonGroup, Resource);
	friend class MobileButton;
	HashSet<MobileButton *> buttons;
	bool allow_unpress = false;

protected:
	static void _bind_methods();

public:
	MobileButton *get_pressed_button();
	void get_buttons(List<MobileButton *> *r_buttons);
	TypedArray<MobileButton> _get_buttons();
	void set_allow_unpress(bool p_enabled);
	bool is_allow_unpress();
	MobileButtonGroup();
};