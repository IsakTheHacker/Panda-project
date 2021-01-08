#pragma once

//C++ built-in libraries
#include <string>
#include <map>
#include <fstream>

//My libraries
#include "gameFunctions.h"

namespace game {

	class item {
		private:
		public:
		std::string configPath;
		unsigned int stackedItems;
		std::map<std::string, std::string> options;

		item(const std::string& configPath, const unsigned int& stackedItems = 1);
	};

	extern item emptyItem;
}