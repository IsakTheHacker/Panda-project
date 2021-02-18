#include "gameInventory.h"

namespace game {
	inventory::inventory() {		//Empty constructor
		this->slots = 0;
		this->maximumStackSize = 1;
	}
	inventory::inventory(const unsigned int& slots, const unsigned int& maximumStackSize) {
		this->slots = slots;
		this->maximumStackSize = maximumStackSize;

		for (unsigned int i = 0; i < slots; i++) {
			items.push_back(emptyItem);
		}
	}
	int inventory::remItem(const unsigned int& slot, const unsigned int& items) {
		this->items[slot] = emptyItem;
		return 0;
	}
	item inventory::getItem(const unsigned int& slot) const {
		if (slot >= this->items.size()) {
			return emptyItem;
		} else {
			return this->items[slot];
		}
	}
	void inventory::setItem(const unsigned int& slot, item item) {
		this->items[slot] = item;
	}
	void inventory::appendItem(item item) {
		this->items.push_back(item);
		this->slots = this->items.size();
	}
	void inventory::resize(const unsigned int& slots) {
		this->items = std::vector<game::item>(slots, emptyItem);
		this->slots = this->items.size();
	}
	std::vector<item> inventory::getItems() {
		return this->items;
	}
	inventory::operator std::string() {
		std::string stringifiedItems;
		for (item item : items) {
			stringifiedItems += std::to_string("\n        ");
			stringifiedItems += item;
		}
		std::string stringObject =
			"Stringobject of game::inventory:\n"
			"    Slots: " + std::to_string(slots) + "\n"
			"    Items: " + stringifiedItems + ""
		;
		return stringObject;
	}
}