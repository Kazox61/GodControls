#include "item_mobile_scroll_container.h"

void ItemMobileScrollContainer::set_visible_items_count(int p_count) {
	if (_visible_items_count == p_count) {
		return;
	}

	_visible_items_count = p_count;

	update_minimum_size();
}

int ItemMobileScrollContainer::_get_first_visible_item_index() const {
	return _first_visible_item_index;
}

int ItemMobileScrollContainer::_get_last_visible_item_index() const {
	return _first_visible_item_index + _visible_items_count - 1;
}

Control *ItemMobileScrollContainer::_get_first_visible_item() const {
	if (!_scroll_view) {
		return nullptr;
	}

	int first_index = _get_first_visible_item_index();
	if (first_index < 0 || first_index >= _scroll_view->get_child_count()) {
		return nullptr;
	}
	
	return Object::cast_to<Control>(_scroll_view->get_child(first_index));
}

Control *ItemMobileScrollContainer::_get_last_visible_item() const {
	if (!_scroll_view) {
		return nullptr;
	}

	int last_index = _get_last_visible_item_index();
	if (last_index < 0 || last_index >= _scroll_view->get_child_count()) {
		return nullptr;
	}
	
	return Object::cast_to<Control>(_scroll_view->get_child(last_index));
}

Vector2 ItemMobileScrollContainer::_get_minimum_size() const {
	Control *first = _get_first_visible_item();
	Control *last = _get_last_visible_item();

	if (!first || !last) {
		return Size2();
	}

	Vector2 min_pos = first->get_position();
	Vector2 max_pos = last->get_position() + last->get_size();

	return max_pos - min_pos;
}

void ItemMobileScrollContainer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_visible_items_count", "count"), &ItemMobileScrollContainer::set_visible_items_count);
	ClassDB::bind_method(D_METHOD("get_visible_items_count"), &ItemMobileScrollContainer::get_visible_items_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "visible_items_count"), "set_visible_items_count", "get_visible_items_count");
}