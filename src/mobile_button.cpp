#include "mobile_button.h"

#include <godot_cpp/classes/property_tweener.hpp>

void MobileButton::set_touch_disabled(bool p_disabled) {
	touch_disabled = p_disabled;
	queue_redraw();
}

bool MobileButton::get_touch_disabled() const {
	return touch_disabled;
}

void MobileButton::set_toggle_mode(bool p_toggle_mode) {
	bool old_toggle_mode = toggle_mode;

	toggle_mode = p_toggle_mode;

	if (!toggle_mode && old_toggle_mode != toggle_mode) {
		button_pressed = false;
	}
}

bool MobileButton::get_toggle_mode() const {
	return toggle_mode;
}

void MobileButton::set_button_pressed(bool p_pressed) {
	if (!toggle_mode) {
		button_pressed = false;
		queue_redraw();
		return;
	}

	if (p_pressed == button_pressed) {
		return;
	}

	button_pressed = p_pressed;

	if (button_pressed) {
		unpress_group();
		if (button_group.is_valid()) {
			button_group->emit_signal("pressed", this);
		}
	}

	queue_redraw();

	emit_signal("toggled", button_pressed);
}

bool MobileButton::get_button_pressed() const {
	return button_pressed;
}

void MobileButton::set_button_group(const Ref<MobileButtonGroup> &p_button_group) {
	if (button_group.is_valid()) {
		button_group->buttons.erase(this);
	}

	button_group = p_button_group;

	if (button_group.is_valid()) {
		button_group->buttons.insert(this);
	}

	queue_redraw();
}

Ref<MobileButtonGroup> MobileButton::get_button_group() const {
	return button_group;
}

void MobileButton::set_long_press_enabled(bool p_enabled) {
	long_press_enabled = p_enabled;
}

bool MobileButton::get_long_press_enabled() const {
	return long_press_enabled;
}

void MobileButton::set_long_press_activation_time(float p_time) {
	long_press_activation_time = p_time;
}

float MobileButton::get_long_press_activation_time() const {
	return long_press_activation_time;
}

void MobileButton::set_h_pivot_position(PivotPosition p_position) {
	h_pivot_position = p_position;
	set_pivot();
}

MobileButton::PivotPosition MobileButton::get_h_pivot_position() const {
	return h_pivot_position;
}

void MobileButton::set_v_pivot_position(PivotPosition p_position) {
	v_pivot_position = p_position;
	set_pivot();
}

MobileButton::PivotPosition MobileButton::get_v_pivot_position() const {
	return v_pivot_position;
}

void MobileButton::set_animated(bool p_animated) {
	animated = p_animated;
}

bool MobileButton::get_animated() const {
	return animated;
}

void MobileButton::set_animation_duration(float p_duration) {
	animation_duration = p_duration;
}

float MobileButton::get_animation_duration() const {
	return animation_duration;
}

void MobileButton::set_button_down_scale(Vector2 p_scale) {
	button_down_scale = p_scale;
}

Vector2 MobileButton::get_button_down_scale() const {
	return button_down_scale;
}

void MobileButton::set_button_up_scale(Vector2 p_scale) {
	button_up_scale = p_scale;
}

Vector2 MobileButton::get_button_up_scale() const {
	return button_up_scale;
}

void MobileButton::set_pivot() {
	Vector2 pivot_offset = Vector2();

	switch (h_pivot_position) {
		case START:
			pivot_offset.x = 0;
			break;
		case CENTER:
			pivot_offset.x = get_size().x / 2;
			break;
		case END:
			pivot_offset.x = get_size().x;
			break;
	}

	switch (v_pivot_position) {
		case START:
			pivot_offset.y = 0;
			break;
		case CENTER:
			pivot_offset.y = get_size().y / 2;
			break;
		case END:
			pivot_offset.y = get_size().y;
			break;
	}

	set_pivot_offset(pivot_offset);
}

void MobileButton::unpress_group() {
	if (button_group.is_null()) {
		return;
	}

	if (toggle_mode && !button_group->is_allow_unpress()) {
		button_pressed = true;
	}

	for (MobileButton *E : button_group->buttons) {
		if (E == this) {
			continue;
		}

		E->set_button_pressed(false);
	}
}

void MobileButton::play_shrink_animation() {
	if (tween.is_valid()) {
		tween->kill();
	}

	tween = create_tween()
		->set_trans(Tween::TRANS_SINE)
		->set_ease(Tween::EASE_IN_OUT);

	tween->tween_property(this, "scale", button_down_scale, animation_duration);

	tween->play();
}

void MobileButton::play_grow_animation() {
	if (tween.is_valid()) {
		tween->kill();
	}

	tween = create_tween()
		->set_trans(Tween::TRANS_SINE)
		->set_ease(Tween::EASE_IN_OUT);

	tween->tween_property(this, "scale", button_up_scale, animation_duration * 0.5f);
	tween->tween_property(this, "scale", Vector2(1, 1), animation_duration * 0.5f);
	tween->play();
}

void MobileButton::handle_screen_touch_start(const Ref<InputEventScreenTouch> &p_touch) {
	is_pressing = true;
	touch_duration = 0.0f;
	drag_distance = 0.0f;
	is_canceled = false;

	long_pressed = false;

	emit_signal("touch_down");

	if (animated) {
		play_shrink_animation();
	}
}

void MobileButton::handle_screen_drag(const Ref<InputEventScreenDrag> &p_drag) {
	drag_distance += p_drag->get_relative().length();

	if (long_pressed) {
		emit_signal("touch_long_press_drag", p_drag);
	}

	if (drag_distance > 25.0f || is_canceled) {
		return;
	}

	is_canceled = true;

	if (tween.is_valid()) {
		tween->kill();
		set_scale(Vector2(1, 1));
	}

	emit_signal("touch_cancel");

	if (long_pressed) {
		emit_signal("touch_long_press_cancel");
	}
}

void MobileButton::handle_screen_touch_end(const Ref<InputEventScreenTouch> &p_touch) {
	is_pressing = false;

	if (is_canceled) {
		if (tween.is_valid()) {
			tween->kill();
			set_scale(Vector2(1, 1));
		}
	}
	else {
		if (animated) {
			play_grow_animation();
		}
		else {
			set_scale(Vector2(1, 1));
		}
	}

	if (!is_canceled) {
		if (toggle_mode) {
			set_button_pressed(!button_pressed);
		}

		emit_signal("touch_press");

		if (long_pressed) {
			emit_signal("touch_long_press");
		}
	}

	if (long_pressed) {
		emit_signal("touch_long_press_end", p_touch);
	}

	emit_signal("touch_up");
}

void MobileButton::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			set_pivot();
			break;
		}
		case NOTIFICATION_RESIZED: {
			set_pivot();
			break;
		}
	}
}

void MobileButton::_gui_input(const Ref<InputEvent> &p_event) {
	if (touch_disabled) {
		return;
	}

	Ref<InputEventScreenTouch> touch = p_event;

	if (touch.is_valid()) {
		if (touch->is_pressed()) {
			handle_screen_touch_start(touch);
		} 
		else if (touch->is_released()) {
			handle_screen_touch_end(touch);
		}
	}

	Ref<InputEventScreenDrag> drag = p_event;

	if (drag.is_valid()) {
		handle_screen_drag(drag);
	}
}

void MobileButton::_process(double p_delta) {
	if (!is_pressing) {
		return;
	}

	touch_duration += p_delta;

	if (!long_press_enabled || long_pressed) {
		return;
	}

	if (touch_duration > long_press_activation_time && drag_distance < 25.0f) {
		long_pressed = true;

		emit_signal("touch_long_press_start");
	}
}

void MobileButton::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_touch_disabled", "disabled"), &MobileButton::set_touch_disabled);
	ClassDB::bind_method(D_METHOD("get_touch_disabled"), &MobileButton::get_touch_disabled);

	ClassDB::bind_method(D_METHOD("set_toggle_mode", "toggle_mode"), &MobileButton::set_toggle_mode);
	ClassDB::bind_method(D_METHOD("get_toggle_mode"), &MobileButton::get_toggle_mode);

	ClassDB::bind_method(D_METHOD("set_button_pressed", "pressed"), &MobileButton::set_button_pressed);
	ClassDB::bind_method(D_METHOD("get_button_pressed"), &MobileButton::get_button_pressed);

	ClassDB::bind_method(D_METHOD("set_button_group", "button_group"), &MobileButton::set_button_group);
	ClassDB::bind_method(D_METHOD("get_button_group"), &MobileButton::get_button_group);

	ClassDB::bind_method(D_METHOD("set_long_press_enabled", "enabled"), &MobileButton::set_long_press_enabled);
	ClassDB::bind_method(D_METHOD("get_long_press_enabled"), &MobileButton::get_long_press_enabled);

	ClassDB::bind_method(D_METHOD("set_long_press_activation_time", "time"), &MobileButton::set_long_press_activation_time);
	ClassDB::bind_method(D_METHOD("get_long_press_activation_time"), &MobileButton::get_long_press_activation_time);

	ClassDB::bind_method(D_METHOD("set_h_pivot_position", "position"), &MobileButton::set_h_pivot_position);
	ClassDB::bind_method(D_METHOD("get_h_pivot_position"), &MobileButton::get_h_pivot_position);

	ClassDB::bind_method(D_METHOD("set_v_pivot_position", "position"), &MobileButton::set_v_pivot_position);
	ClassDB::bind_method(D_METHOD("get_v_pivot_position"), &MobileButton::get_v_pivot_position);

	ClassDB::bind_method(D_METHOD("set_animated", "animated"), &MobileButton::set_animated);
	ClassDB::bind_method(D_METHOD("get_animated"), &MobileButton::get_animated);

	ClassDB::bind_method(D_METHOD("set_animation_duration", "animation_duration"), &MobileButton::set_animation_duration);
	ClassDB::bind_method(D_METHOD("get_animation_duration"), &MobileButton::get_animation_duration);

	ClassDB::bind_method(D_METHOD("set_button_down_scale", "scale"), &MobileButton::set_button_down_scale);
	ClassDB::bind_method(D_METHOD("get_button_down_scale"), &MobileButton::get_button_down_scale);

	ClassDB::bind_method(D_METHOD("set_button_up_scale", "scale"), &MobileButton::set_button_up_scale);
	ClassDB::bind_method(D_METHOD("get_button_up_scale"), &MobileButton::get_button_up_scale);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "touch_disabled"), "set_touch_disabled", "get_touch_disabled");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "toggle_mode"), "set_toggle_mode", "get_toggle_mode");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "button_pressed"), "set_button_pressed", "get_button_pressed");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "button_group", PROPERTY_HINT_RESOURCE_TYPE, "MobileButtonGroup"), "set_button_group", "get_button_group");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "long_press_enabled"), "set_long_press_enabled", "get_long_press_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "long_press_activation_time"), "set_long_press_activation_time", "get_long_press_activation_time");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "h_pivot_position", PROPERTY_HINT_ENUM, "Start,Center,End"), "set_h_pivot_position", "get_h_pivot_position");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "v_pivot_position", PROPERTY_HINT_ENUM, "Start,Center,End"), "set_v_pivot_position", "get_v_pivot_position");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "animated"), "set_animated", "get_animated");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "animation_duration"), "set_animation_duration", "get_animation_duration");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "button_down_scale"), "set_button_down_scale", "get_button_down_scale");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "button_up_scale"), "set_button_up_scale", "get_button_up_scale");

	ADD_SIGNAL(MethodInfo("touch_down"));
	ADD_SIGNAL(MethodInfo("touch_up"));
	ADD_SIGNAL(MethodInfo("touch_cancel"));
	ADD_SIGNAL(MethodInfo("touch_press"));

	ADD_SIGNAL(MethodInfo("touch_long_press_start"));
	ADD_SIGNAL(MethodInfo("touch_long_press_drag", PropertyInfo(Variant::OBJECT, "drag", PROPERTY_HINT_RESOURCE_TYPE, "InputEventScreenDrag")));
	ADD_SIGNAL(MethodInfo("touch_long_press_end", PropertyInfo(Variant::OBJECT, "touch", PROPERTY_HINT_RESOURCE_TYPE, "InputEventScreenTouch")));
	ADD_SIGNAL(MethodInfo("touch_long_press_cancel"));
	ADD_SIGNAL(MethodInfo("touch_long_press"));

	ADD_SIGNAL(MethodInfo("toggled", PropertyInfo(Variant::BOOL, "toggled_on")));
}

MobileButton::MobileButton() { }

MobileButton::~MobileButton() {
	if (button_group.is_valid()) {
		button_group->buttons.erase(this);
	}
}


void MobileButtonGroup::get_buttons(List<MobileButton *> *r_buttons) {
	for (MobileButton *E : buttons) {
		r_buttons->push_back(E);
	}
}

TypedArray<MobileButton> MobileButtonGroup::_get_buttons() {
	TypedArray<MobileButton> btns;
	for (const MobileButton *E : buttons) {
		btns.push_back(E);
	}

	return btns;
}

MobileButton *MobileButtonGroup::get_pressed_button() {
	for (MobileButton *E : buttons) {
		if (E->get_button_pressed()) {
			return E;
		}
	}

	return nullptr;
}

void MobileButtonGroup::set_allow_unpress(bool p_enabled) {
	allow_unpress = p_enabled;
}
bool MobileButtonGroup::is_allow_unpress() {
	return allow_unpress;
}

void MobileButtonGroup::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_pressed_button"), &MobileButtonGroup::get_pressed_button);
	ClassDB::bind_method(D_METHOD("get_buttons"), &MobileButtonGroup::_get_buttons);
	ClassDB::bind_method(D_METHOD("set_allow_unpress", "enabled"), &MobileButtonGroup::set_allow_unpress);
	ClassDB::bind_method(D_METHOD("is_allow_unpress"), &MobileButtonGroup::is_allow_unpress);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "allow_unpress"), "set_allow_unpress", "is_allow_unpress");

	ADD_SIGNAL(MethodInfo("pressed", PropertyInfo(Variant::OBJECT, "button", PROPERTY_HINT_RESOURCE_TYPE, "MobileButton")));
}

MobileButtonGroup::MobileButtonGroup() {
	set_local_to_scene(true);
}