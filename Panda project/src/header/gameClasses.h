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

			item(std::string configPath, unsigned int stackedItems = 1);
	};

	extern item emptyItem;

	// Inventory class
	class inventory {
		private:
			std::vector<item> items;
		public:
			unsigned int slots;
			unsigned int maximumStackSize; //Set to 0 for infinite

			inventory(const unsigned int slots = 33, unsigned int maximumStackSize = 256) {
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

			int remItem(unsigned int slot, unsigned int items = 1) {
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

			windObject(WindowFramework*& window, PandaFramework& framework, double mx, double my, double mz, double velocity, PN_stdfloat sx = 1, PN_stdfloat sy = 1, PN_stdfloat sz = 1, bool shouldLogInConsole = false, bool shouldLogToFile = false);
			~windObject();
	};
	
	// Object class
	class object {
		protected:
			std::string modelpathIntern;
			std::string oldModelpath;
			bool shouldLogInConsoleIntern;
			bool shouldLogToFileIntern;
			bool modelNotFound;
		public:
			bool empty = false;
			NodePath model;
			static int current_id;
			static int object_quantity;
			unsigned int id;
			std::string name;

			object(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool collidable = true, bool shouldLogInConsole = true, bool shouldLogToFile = false);
			object(WindowFramework*& window, PandaFramework& framework, std::vector<NodePath> subobjects, bool collidable = true, bool shouldLogInConsole = true, bool shouldLogToFile = false);
			object(bool shouldLogInConsole = false, bool shouldLogToFile = false);		//Empty game::object constructor
			~object();

			operator std::string();
	};

	// Entity class
	class entity : public object {
		public:
			entity(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool collidable = true, bool shouldLogInConsole = true, bool shouldLogToFile = false);
			~entity();
	};

	// Player class
	class player : public entity {
		public:
			NodePath collisionNodePath;

			player(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool shouldLogInConsole = true, bool shouldLogToFile = false);
			~player();
	};

	//Chunk class
	class chunk {
	public:
		int x;
		int y;
		std::vector<object> objects;
		static std::set<std::pair<int, int>> index;

		chunk(std::vector<object> objects, int x, int y);
		chunk(int x, int y);
		int reset();
	};

	//Creating vectors for the classes
	extern std::vector<windObject> winds;
	extern std::vector<chunk> chunks;
	extern std::vector<entity> entities;
	extern std::vector<player> players;

	/// <summary> Reads a chunk from the specified path. </summary>
	/// <param name="window">- The window object</param>
	/// <param name="framework">- The framework object</param>
	/// <param name="path">- The path where the chunk file is located</param>
	/// <param name="x">- The chunk's x-pos</param>
	/// <param name="y">- The chunk's y-pos</param>
	/// <returns> 0 if successful, nonzero if not! </returns>
	int readChunk(WindowFramework*& window, PandaFramework& framework, std::string path, int x, int y);

	/// <summary> Saves a specified chunk to it's destination. </summary>
	/// <param name="chunk">- your specifed chunk object</param>
	/// <returns> 0 if successful, nonzero if not! </returns>
	int saveChunk(chunk chunk);

	int generateChunk(WindowFramework*& window, PandaFramework& framework, std::pair<int, int> coords, PerlinNoise3 perlinNoise);
}