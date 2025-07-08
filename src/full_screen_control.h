#pragma once

#include <godot_cpp/classes/control.hpp>

using namespace godot;

class FullScreenControl : public Control {
	GDCLASS(FullScreenControl, Control);

public:
	void _ready() override;

protected:
	static void _bind_methods();
};