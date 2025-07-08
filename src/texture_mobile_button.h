#pragma once

#include "mobile_button.h"
#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

class TextureMobileButton : public MobileButton {
	GDCLASS(TextureMobileButton, MobileButton);

public:
	void _draw();

	void set_normal(const Ref<Texture2D> &p_normal);
	Ref<Texture2D> get_normal() const;

	void set_pressed(const Ref<Texture2D> &p_pressed);
	Ref<Texture2D> get_pressed() const;

	void set_disabled(const Ref<Texture2D> &p_disabled);
	Ref<Texture2D> get_disabled() const;

protected:
	static void _bind_methods();

private:
	Ref<Texture2D> normal;
	Ref<Texture2D> pressed;
	Ref<Texture2D> disabled;

	Ref<Texture2D> current;
};