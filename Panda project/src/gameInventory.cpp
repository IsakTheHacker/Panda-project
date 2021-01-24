#include "gameInventory.h"

namespace game {
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
		return this->items[slot];
	}
	void inventory::setItem(const unsigned int& slot, item item) {
		this->items[slot] = item;
	}
}