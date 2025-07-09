#pragma once

#include <godot_cpp/classes/control.hpp>

using namespace godot;

class FullScreenControl : public Control {
	GDCLASS(FullScreenControl, Control);

public:
	Size2 _get_minimum_size() const override;

protected:
	static void _bind_methods();
	void _notification(int p_what);
};