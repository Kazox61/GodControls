#pragma once

#include "mobile_scroll_container.h"

using namespace godot;

class ItemMobileScrollContainer : public MobileScrollContainer {
	GDCLASS(ItemMobileScrollContainer, MobileScrollContainer);

public:
	void set_visible_items_count(int p_count);
	int get_visible_items_count() const { return _visible_items_count; }

	Size2 _get_minimum_size() const override;

protected:
	static void _bind_methods();

private:
	int _visible_items_count = 0;
	int _first_visible_item_index = 0;

	int _get_first_visible_item_index() const;
	int _get_last_visible_item_index() const;

	Control *_get_first_visible_item() const;
	Control *_get_last_visible_item() const;
};