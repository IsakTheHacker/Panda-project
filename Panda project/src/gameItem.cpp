#include "gameItem.h"

namespace game {
	
	item::item(const std::string& configPath, const unsigned int& stackedItems) {
		std::ifstream file(configPath);
		if (!file) {
			errorOut("Specified a configPath for an item that doesn't exist!");
		}
		std::string line;
		std::string delimiter = "=";
		while (std::getline(file, line)) {
			size_t pos = 0;
			std::string token;
			while ((pos = line.find(delimiter)) != std::string::npos) {
				token = line.substr(0, pos);
				line.erase(0, pos + delimiter.length());
			}

			config[token] = line;
		}
		this->configPath = configPath;
		this->stackedItems = stackedItems;
	}

	item emptyItem("emptyItem", 0);
}