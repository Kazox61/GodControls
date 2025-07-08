#pragma once

#include <godot_cpp/classes/control.hpp>

using namespace godot;

class SafeAreaExpand : public Control {
	GDCLASS(SafeAreaExpand, Control);

public:
	enum ExpandPosition {
		LEFT,
		TOP,
		RIGHT,
		BOTTOM
	};

	void set_expand_position(ExpandPosition p_expand_position);
	ExpandPosition get_expand_position() const { return _expand_position; }

	void _ready() override;

protected:
	static void _bind_methods();

private:
	ExpandPosition _expand_position;
};

VARIANT_ENUM_CAST(SafeAreaExpand::ExpandPosition);