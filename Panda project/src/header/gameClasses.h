#pragma once	//Only include this header file once

//C++ built-in libraries
#include <string>
#include <map>
#include <vector>
#include <fstream>

//My libraries
#include "gameFunctions.h"
#include "pandaIncludes.h"

namespace game {

	// Item class
	class item {
		private:
		public:
			std::string configPath;
			unsigned int stackedItems;
			std::map<std::string, std::string> options;

			item(const std::string& configPath, const unsigned int& stackedItems = 1);
	};

	extern item emptyItem;

	// Inventory class
	class inventory {
		private:
			std::vector<item> items;
		public:
			unsigned int slots;
			unsigned int maximumStackSize; //Set to 0 for infinite

			inventory(const unsigned int& slots = 33, const unsigned int& maximumStackSize = 256) {
				this->slots = slots;
				this->maximumStackSize = maximumStackSize;

				for (unsigned int i = 0; i < slots; i++) {
					items.push_back(emptyItem);
				}
			}

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

			int remItem(const unsigned int& slot, const unsigned int& items = 1) {
				this->items[slot] = emptyItem;
				return 0;
			}
	};

	//Class for pauseMenu event parameters
	class pauseMenuEventParameters {
		public:
			WindowFramework* window;
			MouseWatcher* mouseWatcher;
	};

	//Class for collisionRay event parameters
	class collisionRayEventParameters {
		public:
			WindowFramework* window;
			CollisionRay* pickerRay;
			MouseWatcher* mouseWatcher;
	};

	// Class for computer specs
	class computerSpecs {
		public:
			std::string platform;
	};

	// Wind Object class
	class windObject {
		protected:
			bool shouldLogInConsoleIntern;
			bool shouldLogToFileIntern;
		public:
			NodePath model;
			static int current_id;
			static int object_quantity;
			unsigned int id;
			std::string name;
			LPoint3 direction;
			double velocity;

			windObject(WindowFramework*& window, PandaFramework& framework, const double& mx, const double& my, const double& mz, const double& velocity, const PN_stdfloat& sx = 1, const PN_stdfloat& sy = 1, const PN_stdfloat& sz = 1, bool shouldLogInConsole = false, bool shouldLogToFile = false);
			~windObject();
	};

	//Creating vectors for the classes
	extern std::vector<windObject> winds;
}