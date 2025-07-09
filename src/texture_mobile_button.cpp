#include "texture_mobile_button.h"

void TextureMobileButton::_draw() {
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
	draw_texture_rect(current, rect, false);
}

void TextureMobileButton::set_normal(const Ref<Texture2D> &p_normal) {
	if (normal.is_valid()) {
		normal->disconnect("changed", Callable(this, "queue_redraw"));
	}

	normal = p_normal;

	if (normal.is_valid()) {
		normal->connect("changed", Callable(this, "queue_redraw"));
	}

	queue_redraw();
}

Ref<Texture2D> TextureMobileButton::get_normal() const {
	return normal;
}

void TextureMobileButton::set_pressed(const Ref<Texture2D> &p_pressed) {
	if (pressed.is_valid()) {
		pressed->disconnect("changed", Callable(this, "queue_redraw"));
	}

	pressed = p_pressed;

	if (pressed.is_valid()) {
		pressed->connect("changed", Callable(this, "queue_redraw"));
	}

	queue_redraw();
}

Ref<Texture2D> TextureMobileButton::get_pressed() const {
	return pressed;
}
void TextureMobileButton::set_disabled(const Ref<Texture2D> &p_disabled) {
	if (disabled.is_valid()) {
		disabled->disconnect("changed", Callable(this, "queue_redraw"));
	}

	disabled = p_disabled;

	if (disabled.is_valid()) {
		disabled->connect("changed", Callable(this, "queue_redraw"));
	}

	queue_redraw();
}

Ref<Texture2D> TextureMobileButton::get_disabled() const {
	return disabled;
}

void TextureMobileButton::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_normal", "normal"), &TextureMobileButton::set_normal);
	ClassDB::bind_method(D_METHOD("get_normal"), &TextureMobileButton::get_normal);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "normal", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_normal", "get_normal");

	ClassDB::bind_method(D_METHOD("set_pressed", "pressed"), &TextureMobileButton::set_pressed);
	ClassDB::bind_method(D_METHOD("get_pressed"), &TextureMobileButton::get_pressed);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "pressed", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_pressed", "get_pressed");

	ClassDB::bind_method(D_METHOD("set_disabled", "disabled"), &TextureMobileButton::set_disabled);
	ClassDB::bind_method(D_METHOD("get_disabled"), &TextureMobileButton::get_disabled);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "disabled", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_disabled", "get_disabled");
}