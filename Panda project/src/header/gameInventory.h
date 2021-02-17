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
		
		inventory();
		inventory(const unsigned int& slots, const unsigned int& maximumStackSize);

		int remItem(const unsigned int& slot, const unsigned int& items = 1);
		item getItem(const unsigned int& slot) const;
		void setItem(const unsigned int& slot, item item);
		void appendItem(item item);
		void resize(const unsigned int& slots);
		std::vector<item> getItems();

		operator std::string();
	};

}