#pragma once

#include <godot_cpp/classes/margin_container.hpp>

using namespace godot;

class SafeAreaContainer : public MarginContainer {
	GDCLASS(SafeAreaContainer, MarginContainer);

public:
	void _enter_tree() override;
	void _ready() override;

protected:
	static void _bind_methods();

private:
	void configure_full_screen_layout();
	void apply_safe_area_margins();
};