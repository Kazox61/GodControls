#include "stylebox_mobile_button.h"

void StyleboxMobileButton::_draw() {
	current = normal;
	if (!disabled.is_null() && get_touch_disabled()) {
		current = disabled;
	}
	else if (!pressed.is_null() && get_button_pressed()) {
		current = pressed;
	}


	if (current.is_null()) {
		return;
	}

	Rect2 rect = Rect2(Vector2(), get_size());
	draw_style_box(current, rect);
}

void StyleboxMobileButton::set_normal(const Ref<StyleBox> &p_normal) {
	if (normal.is_valid()) {
		normal->disconnect("changed", Callable(this, "queue_redraw"));
	}

	normal = p_normal;

	if (normal.is_valid()) {
		normal->connect("changed", Callable(this, "queue_redraw"));
	}

	queue_redraw();
}

Ref<StyleBox> StyleboxMobileButton::get_normal() const {
	return normal;
}

void StyleboxMobileButton::set_pressed(const Ref<StyleBox> &p_pressed) {
	if (pressed.is_valid()) {
		pressed->disconnect("changed", Callable(this, "queue_redraw"));
	}

	pressed = p_pressed;

	if (pressed.is_valid()) {
		pressed->connect("changed", Callable(this, "queue_redraw"));
	}

	queue_redraw();
}

Ref<StyleBox> StyleboxMobileButton::get_pressed() const {
	return pressed;
}

void StyleboxMobileButton::set_disabled(const Ref<StyleBox> &p_disabled) {
	if (disabled.is_valid()) {
		disabled->disconnect("changed", Callable(this, "queue_redraw"));
	}

	disabled = p_disabled;

	if (disabled.is_valid()) {
		disabled->connect("changed", Callable(this, "queue_redraw"));
	}

	queue_redraw();
}

Ref<StyleBox> StyleboxMobileButton::get_disabled() const {
	return disabled;
}

void StyleboxMobileButton::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_normal", "normal"), &StyleboxMobileButton::set_normal);
	ClassDB::bind_method(D_METHOD("get_normal"), &StyleboxMobileButton::get_normal);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "normal", PROPERTY_HINT_RESOURCE_TYPE, "StyleBox"), "set_normal", "get_normal");

	ClassDB::bind_method(D_METHOD("set_pressed", "pressed"), &StyleboxMobileButton::set_pressed);
	ClassDB::bind_method(D_METHOD("get_pressed"), &StyleboxMobileButton::get_pressed);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "pressed", PROPERTY_HINT_RESOURCE_TYPE, "StyleBox"), "set_pressed", "get_pressed");

	ClassDB::bind_method(D_METHOD("set_disabled", "disabled"), &StyleboxMobileButton::set_disabled);
	ClassDB::bind_method(D_METHOD("get_disabled"), &StyleboxMobileButton::get_disabled);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "disabled", PROPERTY_HINT_RESOURCE_TYPE, "StyleBox"), "set_disabled", "get_disabled");

}