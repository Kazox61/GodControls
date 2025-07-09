#pragma once

#include "mobile_button.h"
#include <godot_cpp/classes/style_box.hpp>

using namespace godot;

class StyleboxMobileButton : public MobileButton {
	GDCLASS(StyleboxMobileButton, MobileButton); 

public:
	void _draw();

	void set_normal(const Ref<StyleBox> &p_normal);
	Ref<StyleBox> get_normal() const;

	void set_pressed(const Ref<StyleBox> &p_pressed);
	Ref<StyleBox> get_pressed() const;

	void set_disabled(const Ref<StyleBox> &p_disabled);
	Ref<StyleBox> get_disabled() const;

protected:
	static void _bind_methods();

private:
	Ref<StyleBox> normal;
	Ref<StyleBox> pressed;
	Ref<StyleBox> disabled;

	Ref<StyleBox> current;
};
