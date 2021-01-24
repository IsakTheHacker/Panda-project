#pragma once

//C++ built-in libraries
#include <vector>

//My libraries
#include "gameItem.h"

namespace game {

	class inventory {
	private:
		std::vector<item> items;
	public:
		unsigned int slots;
		unsigned int maximumStackSize; //Set to 0 for infinite

		inventory(const unsigned int& slots = 33, const unsigned int& maximumStackSize = 256);

		//int addItem(unsigned int slot, std::string configPath, unsigned int stackedItems = 1) {
		//	std::ifstream file(configPath);
		//	if (!file) {
		//		errorOut("Specified a configPath for an item that doesn't exist!");
		//		return 1;
		//	}

		//	if (slot > slots) {
		//		errorOut("Tried to assign an item to a slot that doesn't exist!");
		//		return 1;
		//	} else if (items[slot].itemName != itemName) {
		//		//Another item already assigned at that slot!
		//		return 1;
		//	}
		//	
		//	item item(itemName, texturePath, stackedItems);
		//	items[slot] = item;
		//	return 0;
		//}

		int remItem(const unsigned int& slot, const unsigned int& items = 1);
		item getItem(const unsigned int& slot) const;
		void setItem(const unsigned int& slot, item item);
	};

}